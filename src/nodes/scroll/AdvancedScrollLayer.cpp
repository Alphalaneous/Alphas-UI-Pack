#include <Geode/Geode.hpp>
#include "API.hpp"
#include <geode.devtools/include/API.hpp>

using namespace geode::prelude;
using namespace alpha::prelude;

struct AdvancedScrollLayer::Impl final {
    struct PositionSample {
        CCPoint pos;
        double time;
    };

    bool m_dragging = false;
    bool m_holding = false;
    bool m_inertiaActive = false;

    bool m_verticalScroll = true;
    bool m_horizontalScroll = false;

    bool m_verticalScrollWheel = true;
    bool m_horizontalScrollWheel = true;

    bool m_swapScrollDirection = true;

    bool m_allowsZoom = false;

    float m_overshoot = 50.f;
    float m_friction = 0.7f;
    float m_minVelocity = 200.f;

    float m_minZoom = 0.5f;
    float m_maxZoom = 5.0f;

    bool m_cullingEnabled = true;
    bool m_draggingEnabled = true;
    bool m_allowEmptyClickThrough = false;
    bool m_nextScrollSmoothX = false;
    bool m_nextScrollSmoothY = false;
    bool m_waitForBounce = false;
    bool m_touchFixQueued = false;

    int m_touchPrio = 0;

    Ref<CCActionEase> m_verticalBack;
    Ref<CCActionEase> m_horizontalBack;
    Ref<CCActionEase> m_smoothScrollToX;
    Ref<CCActionEase> m_smoothScrollToY;
    std::vector<Ref<CCTouch>> m_activeTouches;
    ScrollContent* m_content;
    CCNode* m_contentContainer;
    CCNode* m_clickNode;
    Ref<CCArray> m_contentArr;
    CCPoint m_scrollPoint;
    CCPoint m_velocity;
    std::vector<PositionSample> m_samples;
    CullingMethod m_cullingMethod;
    Ref<CCClippingNode> m_clippingNode;
    CCLayerColor* m_stencil;
    Ref<TouchBlocker> m_blockLayer;
    CCSize m_stencilSizeOffset;
};

AdvancedScrollLayer::AdvancedScrollLayer() : m_impl(std::make_unique<Impl>()) {}
AdvancedScrollLayer::~AdvancedScrollLayer() = default;

void AdvancedScrollLayer::registerDevTools() {
    devtools::registerNode<AdvancedScrollLayer>([](AdvancedScrollLayer* node) {

        devtools::label(fmt::format("Holding: {}", node->m_impl->m_holding).c_str());
        devtools::label(fmt::format("Dragging: {}", node->m_impl->m_dragging).c_str());
        devtools::label(fmt::format("Velocity: {}", node->m_impl->m_velocity).c_str());
        devtools::label(fmt::format("Has Inertia: {}", node->m_impl->m_inertiaActive).c_str());

        devtools::property("Scroll Position", node->m_impl->m_scrollPoint);

        if (devtools::property("Overshoot", node->m_impl->m_overshoot)) {
            node->m_impl->m_overshoot = std::max(node->m_impl->m_overshoot, 0.f);
        }

        if (devtools::property("Friction", node->m_impl->m_friction)) {
            node->m_impl->m_friction = std::clamp(node->m_impl->m_friction, 0.f, 1.f);
        }

        if (devtools::property("Min Velocity", node->m_impl->m_minVelocity)) {
            node->m_impl->m_minVelocity = std::max(node->m_impl->m_minVelocity, 0.f);
        }

        devtools::property("Should Cull", node->m_impl->m_cullingEnabled);
        devtools::property("Can Drag", node->m_impl->m_draggingEnabled);
        if (devtools::property("Empty Click Through", node->m_impl->m_allowEmptyClickThrough)) {
            node->allowEmptyClickThrough(node->m_impl->m_allowEmptyClickThrough);
        }

        devtools::property("Vertical Scroll", node->m_impl->m_verticalScroll);
        devtools::property("Vertical Scroll Wheel", node->m_impl->m_verticalScrollWheel);

        if (!node->m_impl->m_verticalScroll) {
            devtools::property("Vertical Scroll Wheel for Horizontal Scroll", node->m_impl->m_swapScrollDirection);
        }

        devtools::property("Horizontal Scroll", node->m_impl->m_horizontalScroll);
        if (node->m_impl->m_horizontalScroll) {
            devtools::property("Horizontal Scroll Wheel", node->m_impl->m_horizontalScrollWheel);
        }
    });

    devtools::registerNode<ScrollContent>([](ScrollContent* node) {
        devtools::property("Scroll Position", node->getScrollLayer()->m_impl->m_scrollPoint);
    });
}

AdvancedScrollLayer* AdvancedScrollLayer::create(const CCSize& size, const CullingMethod& cullingMethod) {
    return create(size, CullingMethod(cullingMethod));
}

AdvancedScrollLayer* AdvancedScrollLayer::create(const CCSize& size) {
    return create(size, nullptr);
}

AdvancedScrollLayer* AdvancedScrollLayer::create(const CCSize& size, CullingMethod&& cullingMethod) {
    auto ret = new AdvancedScrollLayer();
    if (ret->init(size, std::move(cullingMethod))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool AdvancedScrollLayer::init(const CCSize& size, CullingMethod cullingMethod) {
    if (!CCNode::init()) return false;

    m_impl->m_content = ScrollContent::create(this);
    m_impl->m_content->setContentSize(size);

    m_impl->m_cullingMethod = std::move(cullingMethod);

    setAnchorPoint({0.5f, 0.5f});
    ignoreAnchorPointForPosition(false);

    m_impl->m_contentArr = CCArray::create();
    m_impl->m_contentArr->addObject(m_impl->m_content);

    m_impl->m_contentContainer = CCNode::create();
    m_impl->m_contentContainer->setAnchorPoint({0.f, 1.f});
    m_impl->m_contentContainer->addChild(m_impl->m_content);

    m_impl->m_stencil = CCLayerColor::create({255, 255, 255, 255});
    m_impl->m_stencil->setAnchorPoint({0.f, 1.f});
    m_impl->m_stencil->setPositionY(size.height);
    m_impl->m_stencil->ignoreAnchorPointForPosition(false);

    m_impl->m_clippingNode = CCClippingNode::create(m_impl->m_stencil);
    m_impl->m_clippingNode->setAnchorPoint({0.5f, 0.5f});
    m_impl->m_clippingNode->addChild(m_impl->m_contentContainer);
    m_impl->m_clippingNode->m_pParent = this;

    m_impl->m_clickNode = this;
    m_impl->m_blockLayer = TouchBlocker::create(this);
    m_impl->m_blockLayer->m_pParent = this; // better touch prio compat
    m_impl->m_blockLayer->setZOrder(-1);

    // You need to allocate it even if it is never used, since cocos never checks if it exists in sortAllChildren
    // which is called by updateLayout. This is normally not an issue, but since devtools updates the layout 
    // when you change content size, it will lead to a crash. 

    m_pChildren = CCArray::create();
    m_pChildren->retain();

    setTouchPriority(-130);
    setContentSize(size);
    scheduleUpdate();

    return true;
}

void AdvancedScrollLayer::setCullingMethod(const CullingMethod& method) {
    m_impl->m_cullingMethod = method;
}

void AdvancedScrollLayer::setCullingMethod(CullingMethod&& method) {
    m_impl->m_cullingMethod = std::move(method);
}

ScrollContent* AdvancedScrollLayer::getContentLayer() {
    return m_impl->m_content;
}

void AdvancedScrollLayer::handleTouchPrio() {
    std::vector<std::pair<CCTouchHandler*, int>> handlers;
    collectHandlers(this, handlers);

    int minPrio = getTouchPriority() + 2;

    if (!handlers.empty()) {
        int maxPrio = std::numeric_limits<int>::min();
        for (auto& [h, p] : handlers) {
            if (p > maxPrio) maxPrio = p;
        }

        for (auto& [h, p] : handlers) {
            int normalized = p - maxPrio;

            if (auto node = typeinfo_cast<CCNode*>(h->getDelegate())) {
                node->setUserObject("scroll-layer"_spr, this);
            }

            int newPrio = getTouchPriority() + normalized - 2;
            CCTouchDispatcher::get()->setPriority(newPrio, h->getDelegate());

            if (newPrio < minPrio) minPrio = newPrio;
        }
    }
    
    if (auto delegate = static_cast<CCTouchDelegate*>(this)) {
        if (auto handler = CCTouchDispatcher::get()->findHandler(delegate)) {
            CCTouchDispatcher::get()->setPriority(minPrio - 1, handler->getDelegate());
        }
    }
}

void AdvancedScrollLayer::setLayout(Layout* layout, bool apply, bool respectAnchor) {
    m_impl->m_content->setLayout(layout, apply, respectAnchor);
}

void AdvancedScrollLayer::updateLayout(bool updateChildOrder) {
    m_impl->m_content->updateLayout(updateChildOrder);
}

void AdvancedScrollLayer::onEnter() {
    CCNode::onEnter();

    CCTouchDispatcher::get()->addTargetedDelegate(this, getTouchPriority(), false);
    ScrollDispatcher::get()->registerScroll(this);

    m_impl->m_clippingNode->onEnter();
    m_impl->m_blockLayer->onEnter();

    update(0);

    m_impl->m_touchFixQueued = true;
}

void AdvancedScrollLayer::onExit() {
    CCNode::onExit();
    CCTouchDispatcher::get()->removeDelegate(this);
    ScrollDispatcher::get()->unregisterScroll(this);

    m_impl->m_clippingNode->onExit();
    m_impl->m_blockLayer->onExit();
}

// We don't actually want AdvancedScrollLayer to have any "real" children for the sake of simplicity.
// The tree would normally be m_clippingNode -> m_contentContainer -> m_content. 
// This also prevents layouts from affecting it, and makes it easier to use getChildBy...
CCArray* AdvancedScrollLayer::getChildren() {
    return m_impl->m_contentArr;
}

// always 1, m_content is the only child with this setup
unsigned int AdvancedScrollLayer::getChildrenCount() const {
    return 1;
}

void AdvancedScrollLayer::addChild(CCNode* child, int zOrder, int tag) {
    m_impl->m_content->addChild(child, zOrder, tag);
}

void AdvancedScrollLayer::removeChild(CCNode* child, bool cleanup) {
    m_impl->m_content->removeChild(child, cleanup);
}

void AdvancedScrollLayer::removeAllChildrenWithCleanup(bool cleanup) {
    m_impl->m_content->removeAllChildrenWithCleanup(cleanup);
}

// I override visit to visit only the clipping node as this node has no "real" children.
void AdvancedScrollLayer::visit() {
    if (!m_bVisible) return;
    kmGLPushMatrix();

    transform();
    draw();
    m_impl->m_clippingNode->visit();

    m_uOrderOfArrival = 0;
    kmGLPopMatrix();
}

void AdvancedScrollLayer::setMinContainerSize() {
    CCSize size;
    if (m_impl->m_contentContainer->getScaledContentWidth() < getContentWidth()) {
        m_impl->m_contentContainer->setContentWidth(getContentWidth() / m_impl->m_contentContainer->getScale());
    }
    if (m_impl->m_contentContainer->getScaledContentHeight() < getContentHeight()) {
        m_impl->m_contentContainer->setContentHeight(getContentHeight() / m_impl->m_contentContainer->getScale());
    }

    m_impl->m_content->setPositionY(m_impl->m_contentContainer->getContentHeight());
}

void AdvancedScrollLayer::setInnerContentSize(const CCSize& size) {
    m_impl->m_content->setContentSize(size);

    m_impl->m_contentContainer->setContentSize(m_impl->m_content->getScaledContentSize());
    setMinContainerSize();
}

void AdvancedScrollLayer::setContentSize(const CCSize& size) {
    CCNode::setContentSize(size);

    if (m_impl->m_contentContainer) setMinContainerSize();

    if (m_impl->m_clippingNode) {
        m_impl->m_clippingNode->setContentSize(size);
        m_impl->m_clippingNode->setPosition(getContentSize()/2);
    }

    update(0);
}

CCTouchHandler* findHandler(CCTouchDelegate* delegate) {
    auto mainNode = typeinfo_cast<CCNode*>(delegate);
    for (auto handler : CCArrayExt<CCTouchHandler*>(CCTouchDispatcher::get()->m_pTargetedHandlers)) {
        if (auto node = typeinfo_cast<CCNode*>(handler->getDelegate())) {
            if (mainNode == node) return handler;
        }
    }
    return nullptr;
}

void AdvancedScrollLayer::collectHandlers(CCNode* node, std::vector<std::pair<CCTouchHandler*, int>>& out) {
    for (auto child : node->getChildrenExt()) {
        if (auto delegate = typeinfo_cast<CCTouchDelegate*>(child)) {
            if (auto handler = findHandler(delegate)) {
                out.emplace_back(handler, handler->getPriority());
            }
        }
        collectHandlers(child, out);
    }
}

int AdvancedScrollLayer::getTouchPriority() {
    return m_impl->m_touchPrio;
}

void AdvancedScrollLayer::setTouchPriority(int prio) {
    m_impl->m_touchPrio = prio;
}

void AdvancedScrollLayer::cancelTouchesRecursive(CCNode* node, CCTouch* touch, CCEvent* event) {
    if (!node) return;

    if (auto delegate = typeinfo_cast<CCTouchDelegate*>(node)) {
        delegate->ccTouchCancelled(touch, event);
    }
    if (auto delegate = typeinfo_cast<TouchDelegate*>(node)) {
        delegate->ccTouchCancelled(touch, event);
    }

    for (auto child : node->getChildrenExt()) {
        cancelTouchesRecursive(child, touch, event);
    }
}

void AdvancedScrollLayer::cancelChildrenTouches(CCTouch* touch, CCEvent* event) {
    cancelTouchesRecursive(m_impl->m_content, touch, event);
}

bool AdvancedScrollLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    if (!nodeIsVisible(this)) return false;
    m_impl->m_activeTouches.push_back(touch);


    if (!alpha::utils::isTouchInsideNode(m_impl->m_clickNode, touch->getLocation())) {
        setVisible(false);
        runAction(CallFuncExt::create([this, touch = Ref(touch), event = Ref(event)] {
            cancelChildrenTouches(touch, event);
            setVisible(true);
        }));
        return false;
    }

    m_impl->m_inertiaActive = false;
    unschedule(schedule_selector(AdvancedScrollLayer::updateInertia));

    stopAction(m_impl->m_verticalBack);
    stopAction(m_impl->m_horizontalBack);
    stopAction(m_impl->m_smoothScrollToX);
    stopAction(m_impl->m_smoothScrollToY);

    m_impl->m_verticalBack = nullptr;
    m_impl->m_horizontalBack = nullptr;
    m_impl->m_smoothScrollToX = nullptr;
    m_impl->m_smoothScrollToY = nullptr;

    m_impl->m_dragging = false;
    m_impl->m_velocity = CCPoint{0.f, 0.f};
    m_impl->m_samples.clear();

    if (m_impl->m_draggingEnabled) m_impl->m_holding = true;

    return true;
}

void AdvancedScrollLayer::ccTouchMoved(CCTouch* touch, CCEvent* event) {
    if (!m_impl->m_draggingEnabled || !nodeIsVisible(this)) return;

    if (m_impl->m_activeTouches.size() == 2 && m_impl->m_allowsZoom) {
        CCTouch* t1 = m_impl->m_activeTouches[0];
        CCTouch* t2 = m_impl->m_activeTouches[1];

        auto prevDist = (t1->getPreviousLocation() - t2->getPreviousLocation()).getLength();
        auto currDist = (t1->getLocation() - t2->getLocation()).getLength();

        float zoomDelta = currDist - prevDist;
        zoom(zoomDelta);
        return;
    }

    if (m_impl->m_activeTouches.size() == 1) {
        CCPoint prev = convertToNodeSpace(touch->getPreviousLocation());
        CCPoint curr = convertToNodeSpace(touch->getLocation());
        CCPoint delta = curr - prev;

        if (!m_impl->m_dragging && delta.getLength() > 0.5f) {
            m_impl->m_dragging = true;
            cancelChildrenTouches(touch, event);
        }

        if (m_impl->m_dragging) {
            CCPoint pos = m_impl->m_contentContainer->getPosition(); 
            if (m_impl->m_horizontalScroll) pos.x += delta.x; 
            if (m_impl->m_verticalScroll) pos.y += delta.y; 

            float minX = getContentWidth() - m_impl->m_contentContainer->getScaledContentWidth() - m_impl->m_overshoot; 
            float maxX = m_impl->m_overshoot; 
            float minY = getContentHeight() - m_impl->m_overshoot; 
            float maxY = m_impl->m_contentContainer->getScaledContentHeight() + m_impl->m_overshoot; 

            pos.x = std::clamp(pos.x, minX, maxX); 
            pos.y = std::clamp(pos.y, minY, maxY); 

            m_impl->m_contentContainer->setPosition(pos);

            m_impl->m_samples.push_back({touch->getLocation(), alpha::utils::nowSeconds()});
            if (m_impl->m_samples.size() > 3) {
                m_impl->m_samples.erase(m_impl->m_samples.begin());
            }
        }
    }
}

void AdvancedScrollLayer::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    if (!nodeIsVisible(this)) return;

    if (m_impl->m_dragging) {
        m_impl->m_dragging = false;

        if (m_impl->m_samples.size() >= 2) {
            const Impl::PositionSample& first = m_impl->m_samples.front();
            const Impl::PositionSample& last  = m_impl->m_samples.back();

            float dt = last.time - first.time;
            if (dt > 0) {
                CCPoint vel = (last.pos - first.pos) / dt;

                const float minSpeed = 400.0f;
                if (vel.getLength() < minSpeed) {
                    m_impl->m_velocity = CCPoint{0.f, 0.f};
                } else {
                    CCNode* parent = m_impl->m_content->getParent();
                    CCPoint worldA = vel;
                    CCPoint worldB = CCPoint{0.f, 0.f};

                    CCPoint localA = parent->convertToNodeSpace(worldA) * parent->getScale();
                    CCPoint localB = parent->convertToNodeSpace(worldB) * parent->getScale();
                    m_impl->m_velocity = localA - localB;
                    if (!m_impl->m_horizontalScroll) m_impl->m_velocity.x = 0;
                    if (!m_impl->m_verticalScroll) m_impl->m_velocity.y = 0;
                }
            }
        }
        
        unschedule(schedule_selector(AdvancedScrollLayer::updateInertia));
        schedule(schedule_selector(AdvancedScrollLayer::updateInertia));
        m_impl->m_inertiaActive = true;
    }

    m_impl->m_holding = false;
    m_impl->m_samples.clear();

    auto it = std::find(m_impl->m_activeTouches.begin(), m_impl->m_activeTouches.end(), touch);
    if (it != m_impl->m_activeTouches.end()) m_impl->m_activeTouches.erase(it);
}

void AdvancedScrollLayer::bounceBack(bool horizontal) {
    if (m_impl->m_dragging) return;

    CCPoint pos = m_impl->m_contentContainer->getPosition();

    constexpr float duration = 0.2f;
    constexpr float rate = 2.f;

    if (horizontal) {
        float minX = getContentWidth() - m_impl->m_contentContainer->getScaledContentWidth();
        float maxX = 0;
        if (pos.x < minX) pos.x = minX;
        if (pos.x > maxX) pos.x = maxX;

        if (!m_impl->m_horizontalBack || m_impl->m_horizontalBack->isDone()) {
            m_impl->m_horizontalBack = CCEaseOut::create(CCMoveToX::create(duration, pos.x), rate);
            m_impl->m_contentContainer->runAction(m_impl->m_horizontalBack);
        }
    }
    else {
        float minY = getContentHeight();
        float maxY = m_impl->m_contentContainer->getScaledContentHeight();
        if (pos.y < minY) pos.y = minY;
        if (pos.y > maxY) pos.y = maxY;

        if (!m_impl->m_verticalBack || m_impl->m_verticalBack->isDone()) {
            m_impl->m_verticalBack = CCEaseOut::create(CCMoveToY::create(duration, pos.y), rate);
            m_impl->m_contentContainer->runAction(m_impl->m_verticalBack);
        }
    }
}

void AdvancedScrollLayer::updateInertia(float dt) {

    bool shouldWait = false;

    bool verticalBackActive = m_impl->m_verticalBack && !m_impl->m_verticalBack->isDone();
    bool horizontalBackActive = m_impl->m_horizontalBack && !m_impl->m_horizontalBack->isDone();

    if (m_impl->m_friction == 1 || m_impl->m_velocity.getLength() <= m_impl->m_minVelocity) {
        m_impl->m_velocity = CCPoint{0.f, 0.f};
        if (!m_impl->m_waitForBounce) {
            shouldWait = true;
            bounceBack(true);
            bounceBack(false);
        }
    }
    
    CCPoint pos = m_impl->m_contentContainer->getPosition();
    pos += m_impl->m_velocity * dt;

    if (m_impl->m_horizontalScroll) {

        float minX = getContentWidth() - m_impl->m_contentContainer->getScaledContentWidth();
        float maxX = 0;

        m_impl->m_velocity.x *= std::pow(1 - m_impl->m_friction, dt / 0.2f);

        pos.x = std::clamp(pos.x, minX - m_impl->m_overshoot, maxX + m_impl->m_overshoot);

        if (pos.x <= minX - m_impl->m_overshoot || pos.x >= maxX + m_impl->m_overshoot) {
            m_impl->m_velocity.x = 0;
            shouldWait = true;
            bounceBack(true);
        }
        else if (std::abs(m_impl->m_velocity.x) < m_impl->m_overshoot && (pos.x < minX || pos.x > maxX)) {
            m_impl->m_velocity.x = 0;
            if (pos.x < minX || pos.x > maxX) {
                shouldWait = true;
                bounceBack(true);
            }
        }
        if (std::abs(m_impl->m_velocity.x) < 0.1f) m_impl->m_velocity.x = 0;
        if (!horizontalBackActive) m_impl->m_contentContainer->setPositionX(pos.x);
    }

    if (m_impl->m_verticalScroll) {
        float minY = getContentHeight();
        float maxY = m_impl->m_contentContainer->getScaledContentHeight();

        m_impl->m_velocity.y *= std::pow(1 - m_impl->m_friction, dt / 0.2f);

        pos.y = std::clamp(pos.y, minY - m_impl->m_overshoot, maxY + m_impl->m_overshoot);

        if (pos.y <= minY - m_impl->m_overshoot || pos.y >= maxY + m_impl->m_overshoot) {
            m_impl->m_velocity.y = 0;
            shouldWait = true;
            bounceBack(false);
        }
        else if (std::abs(m_impl->m_velocity.y) < m_impl->m_overshoot && (pos.y < minY || pos.y > maxY)) {
            m_impl->m_velocity.y = 0;
            if (pos.y < minY || pos.y > maxY) {
                shouldWait = true;
                bounceBack(false);
            }
        }
        if (std::abs(m_impl->m_velocity.y) < 0.1f) m_impl->m_velocity.y = 0;
        if (!verticalBackActive) m_impl->m_contentContainer->setPositionY(pos.y);
    }

    if (!shouldWait) {

        bool hasVelocity = m_impl->m_velocity.x != 0.f || m_impl->m_velocity.y != 0.f;
        m_impl->m_inertiaActive = verticalBackActive || horizontalBackActive || hasVelocity;

        if (!m_impl->m_inertiaActive) {
            m_impl->m_waitForBounce = false;
            unschedule(schedule_selector(AdvancedScrollLayer::updateInertia));
        }
        return;
    }
    m_impl->m_waitForBounce = shouldWait;
}

void AdvancedScrollLayer::ccTouchCancelled(CCTouch* touch, CCEvent* pEvent) {
    if (!nodeIsVisible(this)) return;

    m_impl->m_holding = false;
    m_impl->m_dragging = false;
    m_impl->m_velocity = CCPoint{0.f, 0.f};
    m_impl->m_samples.clear();

    auto it = std::find(m_impl->m_activeTouches.begin(), m_impl->m_activeTouches.end(), touch);
    if (it != m_impl->m_activeTouches.end()) m_impl->m_activeTouches.erase(it);

    constrain();
}

void AdvancedScrollLayer::setScrollX(float x, bool smooth) {
    m_impl->m_scrollPoint.x = std::clamp(x, 0.f, getHorizontalMax());
    if (smooth) {
        stopAction(m_impl->m_smoothScrollToX);
        m_impl->m_smoothScrollToX = nullptr;
        m_impl->m_nextScrollSmoothX = true;
    }
    update(0);
}

void AdvancedScrollLayer::setScrollY(float y, bool smooth) {
    m_impl->m_scrollPoint.y = std::clamp(y, 0.f, getVerticalMax());

    if (smooth) {
        stopAction(m_impl->m_smoothScrollToY);
        m_impl->m_smoothScrollToY = nullptr;
        m_impl->m_nextScrollSmoothY = true;
    }
    update(0);
}

CCPoint AdvancedScrollLayer::getScrollPoint() {
    return m_impl->m_scrollPoint;
}

void AdvancedScrollLayer::setCullingEnabled(bool value) {
    m_impl->m_cullingEnabled = value;
}

bool AdvancedScrollLayer::isCullingEnabled() {
    return m_impl->m_cullingEnabled;
}

void AdvancedScrollLayer::setDraggingEnabled(bool value) {
    m_impl->m_draggingEnabled = value;
}

bool AdvancedScrollLayer::isDraggingEnabled() {
    return m_impl->m_draggingEnabled;
}

void AdvancedScrollLayer::constrain(bool skipInertiaCheck) {
    if (!m_impl->m_contentContainer || m_impl->m_holding || (!skipInertiaCheck && m_impl->m_inertiaActive)) return;

    float maxX = m_impl->m_contentContainer->getScaledContentWidth() - getContentWidth();
    float maxY = m_impl->m_contentContainer->getScaledContentHeight() - getContentHeight();

    bool outside = false;

    if (m_impl->m_scrollPoint.x > maxX) {
        m_impl->m_scrollPoint.x = maxX;
        outside = true;
    }
    else if (m_impl->m_scrollPoint.x < 0) {
        m_impl->m_scrollPoint.x = 0;
        outside = true;
    }

    if (m_impl->m_scrollPoint.y > maxY) {
        m_impl->m_scrollPoint.y = maxY;
        outside = true;
    }
    else if (m_impl->m_scrollPoint.y < 0) {
        m_impl->m_scrollPoint.y = 0;
        outside = true;
    }

    if (outside) {
        m_impl->m_contentContainer->setPosition({-m_impl->m_scrollPoint.x, getContentHeight() + m_impl->m_scrollPoint.y});
    }
}

void AdvancedScrollLayer::cull() {
    if (!m_impl->m_cullingEnabled) return;
    if (m_impl->m_cullingMethod) return m_impl->m_cullingMethod(m_impl->m_content, m_impl->m_scrollPoint);

    auto world = alpha::utils::rectToWorld(this);

    for (auto child : m_impl->m_content->getChildrenExt()) {
        auto childBounds = alpha::utils::rectToWorld(child, 1.5f);
        child->setVisible(childBounds.intersectsRect(world));
    }
}

void AdvancedScrollLayer::update(float dt) {
    if (!m_impl->m_content) return;

    if (m_impl->m_touchFixQueued) {
        handleTouchPrio();
        m_impl->m_touchFixQueued = false;
    }
    
    m_impl->m_contentContainer->setContentSize(m_impl->m_content->getScaledContentSize());
    setMinContainerSize();

    m_impl->m_stencil->setContentWidth(getContentWidth() + m_impl->m_stencilSizeOffset.width / getScaleX());
    m_impl->m_stencil->setContentHeight(getContentHeight() + m_impl->m_stencilSizeOffset.height / getScaleY());

    m_impl->m_stencil->setPositionY(getContentHeight());

    if (!m_impl->m_holding && !m_impl->m_inertiaActive) {
        CCPoint nextPos;

        if (m_impl->m_horizontalScroll) {
            nextPos.x = -m_impl->m_scrollPoint.x;
        }
        if (m_impl->m_verticalScroll) {
            if (m_impl->m_contentContainer->getScaledContentHeight() < getContentHeight()) {
                nextPos.y = getContentHeight();
            }
            else {
                nextPos.y = getContentHeight() + m_impl->m_scrollPoint.y;
            }
        }
        else {
            nextPos.y = getContentHeight();
        }

        if (m_impl->m_nextScrollSmoothX) {
            stopAction(m_impl->m_smoothScrollToX);
            m_impl->m_smoothScrollToX = CCEaseExponentialOut::create(CCMoveToX::create(0.2f, nextPos.x));
            m_impl->m_contentContainer->runAction(m_impl->m_smoothScrollToX);
            m_impl->m_nextScrollSmoothX = false;
        }
        else {
            if (!m_impl->m_smoothScrollToX || m_impl->m_smoothScrollToX->isDone()) {
                m_impl->m_contentContainer->setPositionX(nextPos.x);
            }
        }

        if (m_impl->m_nextScrollSmoothY) {
            stopAction(m_impl->m_smoothScrollToY);
            m_impl->m_smoothScrollToY = CCEaseExponentialOut::create(CCMoveToY::create(0.2f, nextPos.y));
            m_impl->m_contentContainer->runAction(m_impl->m_smoothScrollToY);
            m_impl->m_nextScrollSmoothY = false;
        }
        else {
            if (!m_impl->m_smoothScrollToY || m_impl->m_smoothScrollToY->isDone()) {
                m_impl->m_contentContainer->setPositionY(nextPos.y);
            }
        }

        constrain();
    }

    m_impl->m_scrollPoint.x = -m_impl->m_contentContainer->getPositionX();
    m_impl->m_scrollPoint.y = m_impl->m_contentContainer->getPositionY() - getContentHeight();

    cull();
}

void AdvancedScrollLayer::scroll(float x, float y) {
    if (!nodeIsVisible(this)) return;
    if (m_impl->m_verticalScrollWheel || m_impl->m_horizontalScrollWheel) {
        bool horizontalScrollWheel = m_impl->m_horizontalScrollWheel;
        if (m_impl->m_horizontalScroll && !m_impl->m_verticalScroll) {
            if (m_impl->m_swapScrollDirection) horizontalScrollWheel = m_impl->m_verticalScrollWheel;
            x = y;
        }

        m_impl->m_inertiaActive = false;
        unschedule(schedule_selector(AdvancedScrollLayer::updateInertia));

        stopAction(m_impl->m_verticalBack);
        stopAction(m_impl->m_horizontalBack);
        stopAction(m_impl->m_smoothScrollToX);
        stopAction(m_impl->m_smoothScrollToY);

        m_impl->m_verticalBack = nullptr;
        m_impl->m_horizontalBack = nullptr;
        m_impl->m_smoothScrollToX = nullptr;
        m_impl->m_smoothScrollToY = nullptr;

        if (m_impl->m_verticalScroll && m_impl->m_verticalScrollWheel) m_impl->m_scrollPoint.y += y;
        if (m_impl->m_horizontalScroll && horizontalScrollWheel) m_impl->m_scrollPoint.x += x;

        constrain(true);
    }
}

Result<float> AdvancedScrollLayer::getHorizontalScroll(enumKeyCodes key) {
    auto kb = CCKeyboardDispatcher::get();
    auto shift = kb->getShiftKeyPressed();
    auto ctrl = kb->getControlKeyPressed() || kb->getCommandKeyPressed();
    auto x = m_impl->m_scrollPoint.x;
    auto width = getContentWidth();
    auto max = getHorizontalMax();

    if (ctrl && key == enumKeyCodes::KEY_Space) {
        return Ok(shift ? x - (width - 30) : x + (width - 30));
    }

    if (shift) {
        switch (key) {
            case enumKeyCodes::KEY_PageUp: {
                return Ok(x - (width - 30));
            }
            case enumKeyCodes::KEY_PageDown: {
                return Ok(x + (width - 30));
            }
            case enumKeyCodes::KEY_Home: {
                return Ok(0);
            }
            case enumKeyCodes::KEY_End: {
                return Ok(max);
            }
            default: {
                break;
            }
        }
    }

    switch (key) {
        case enumKeyCodes::KEY_Left: {
            return Ok(ctrl ? 0 : x - 20);
        }
        case enumKeyCodes::KEY_Right: {
            return Ok(ctrl ? max : x + 20);
        }
        default: {
            break;
        }
    }
    
    return Err("Non-scroll Key");
}

Result<float> AdvancedScrollLayer::getVerticalScroll(enumKeyCodes key, bool horizontal) {
    auto kb = CCKeyboardDispatcher::get();
    auto shift = kb->getShiftKeyPressed();
    auto ctrl = kb->getControlKeyPressed() || kb->getCommandKeyPressed();
    auto p = horizontal ? m_impl->m_scrollPoint.x : m_impl->m_scrollPoint.y;
    auto length = horizontal ? getContentWidth() : getContentHeight();
    auto max = horizontal ? getHorizontalMax() : getVerticalMax();

    switch (key) {
        case enumKeyCodes::KEY_Up: {
            return Ok(ctrl ? 0 : p - 20);
        }
        case enumKeyCodes::KEY_Down: {
            return Ok(ctrl ? max : p + 20);
        }
        case enumKeyCodes::KEY_PageUp: {
            return Ok(p - (length - 30));
        }
        case enumKeyCodes::KEY_PageDown: {
            return Ok(p + (length - 30));
        }
        case enumKeyCodes::KEY_Space: {
            return Ok(shift ? p - (length - 30) : p + (length - 30));
        }
        case enumKeyCodes::KEY_Home: {
            return Ok(0);
        }
        case enumKeyCodes::KEY_End: {
            return Ok(max);
        }
        default: {
            break;
        }
    }
    return Err("Non-scroll Key");
}

void AdvancedScrollLayer::keyPress(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat) {
    if (isKeyDown) {
        if (m_impl->m_horizontalScroll && m_impl->m_swapScrollDirection && !m_impl->m_verticalScroll) {
            if (auto scroll = getVerticalScroll(key, true)) {
                setScrollX(scroll.unwrap(), true);
                return;
            }
        }
        if (m_impl->m_horizontalScroll) {
            if (auto scroll = getHorizontalScroll(key)) {
                setScrollX(scroll.unwrap(), true);
                return;
            }
        }
        if (m_impl->m_verticalScroll) {
            if (auto scroll = getVerticalScroll(key)) {
                setScrollY(scroll.unwrap(), true);
                return;
            }
        }
    }
}

void AdvancedScrollLayer::zoom(float zoomDelta) {
    if (!m_impl->m_allowsZoom || m_impl->m_dragging) return;

    m_impl->m_inertiaActive = false;
    unschedule(schedule_selector(AdvancedScrollLayer::updateInertia));

    CCPoint worldPos = getMousePos();

    float zoomFactor = powf(1.0045f, zoomDelta);

    CCPoint before = m_impl->m_contentContainer->convertToNodeSpace(worldPos);

    float newScale = std::clamp(
        m_impl->m_contentContainer->getScale() * zoomFactor, 
        m_impl->m_minZoom,
        m_impl->m_maxZoom
    );

    m_impl->m_contentContainer->setScale(newScale);

    CCPoint after = m_impl->m_contentContainer->convertToNodeSpace(worldPos);
    CCPoint diff = after - before;

    auto pos = m_impl->m_contentContainer->getPosition() + diff * m_impl->m_contentContainer->getScale();

    setScrollX(-pos.x);
    setScrollY(pos.y - getContentHeight());

    update(0);
}

float AdvancedScrollLayer::getHorizontalMax() {
    return std::max(m_impl->m_contentContainer->getScaledContentWidth() - getContentWidth(), 0.f);
}

float AdvancedScrollLayer::getVerticalMax() {
    return std::max(m_impl->m_contentContainer->getScaledContentHeight() - getContentHeight(), 0.f);
}

float AdvancedScrollLayer::getHorizontalScrollPercent() {
    return m_impl->m_scrollPoint.x / getHorizontalMax();
}

float AdvancedScrollLayer::getVerticalScrollPercent() {
    return m_impl->m_scrollPoint.y / getVerticalMax();
}

float AdvancedScrollLayer::getHorizontalPages() {
    return m_impl->m_contentContainer->getScaledContentWidth() / getContentWidth();
}

float AdvancedScrollLayer::getVerticalPages() {
    return m_impl->m_contentContainer->getScaledContentHeight() / getContentHeight();
}

void AdvancedScrollLayer::setVeritcalScroll(bool value) {
    m_impl->m_verticalScroll = value;
}

void AdvancedScrollLayer::setHorizontalScroll(bool value) {
    m_impl->m_horizontalScroll = value;
}

void AdvancedScrollLayer::setVerticalScrollWheel(bool value) {
    m_impl->m_verticalScrollWheel = value;
}

void AdvancedScrollLayer::setHorizontalScrollWheel(bool value) {
    m_impl->m_horizontalScrollWheel = value;
}

void AdvancedScrollLayer::setVerticalScrollForHorizontal(bool value) {
    m_impl->m_swapScrollDirection = value;
}

bool AdvancedScrollLayer::hasVerticalScroll() {
    return m_impl->m_verticalScroll;
}

bool AdvancedScrollLayer::hasHorizontalScroll() {
    return m_impl->m_horizontalScroll;
}

bool AdvancedScrollLayer::hasVerticalScrollWheel() {
    return m_impl->m_verticalScrollWheel;
}

bool AdvancedScrollLayer::hasHorizontalScrollWheel() {
    return m_impl->m_horizontalScrollWheel;
}

bool AdvancedScrollLayer::hasVerticalScrollForHorizontal() {
    return m_impl->m_swapScrollDirection;
}

void AdvancedScrollLayer::setOvershoot(float value) {
    m_impl->m_overshoot = std::max(value, 0.f);
}

float AdvancedScrollLayer::getOvershoot() {
    return m_impl->m_overshoot;
}

void AdvancedScrollLayer::setFriction(float value) {
    m_impl->m_friction = std::clamp(value, 0.f, 1.f);
}

float AdvancedScrollLayer::getFriction() {
    return m_impl->m_friction;
}

void AdvancedScrollLayer::setMinVelocity(float value) {
    m_impl->m_minVelocity = std::max(value, 0.f);
}

float AdvancedScrollLayer::getMinVelocity() {
    return m_impl->m_minVelocity;
}

void AdvancedScrollLayer::allowEmptyClickThrough(bool allow) {
    m_impl->m_allowEmptyClickThrough = allow;
    m_impl->m_clickNode = allow ? static_cast<CCNode*>(m_impl->m_content) : this;
    m_impl->m_blockLayer->setTarget(m_impl->m_clickNode);
}

bool AdvancedScrollLayer::allowsEmptyClickThrough() {
    return m_impl->m_allowEmptyClickThrough;
}

void AdvancedScrollLayer::offsetStencilWidth(float width) {
    m_impl->m_stencilSizeOffset.width = width;
}

void AdvancedScrollLayer::offsetStencilHeight(float height) {
    m_impl->m_stencilSizeOffset.height = height;
}

void AdvancedScrollLayer::allowZoom(bool allow) {
    m_impl->m_allowsZoom = allow;
}

bool AdvancedScrollLayer::allowsZoom() {
    return m_impl->m_allowsZoom;
}

void AdvancedScrollLayer::setMinZoom(float value) {
    m_impl->m_minZoom = value;
}

void AdvancedScrollLayer::setMaxZoom(float value) {
    m_impl->m_maxZoom = value;
}

float AdvancedScrollLayer::getMinZoom() {
    return m_impl->m_minZoom;
}

float AdvancedScrollLayer::getMaxZoom() {
    return m_impl->m_maxZoom;
}

float AdvancedScrollLayer::getZoom() {
    return m_impl->m_contentContainer->getScale();
}

void AdvancedScrollLayer::setZoom(float zoom) {
    m_impl->m_contentContainer->setScale(zoom);
}

$execute {
    devtools::waitForDevTools([] {
        AdvancedScrollLayer::registerDevTools();
    });
}