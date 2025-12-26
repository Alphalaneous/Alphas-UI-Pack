#include <Geode/Geode.hpp>
#include "API.hpp"
#include <geode.devtools/include/API.hpp>

using namespace geode::prelude;
using namespace alpha::prelude;

struct AdvancedScrollBar::Impl final {
    Ref<AdvancedScrollLayer> m_scrollLayer;
    ScrollOrientation m_orientation;

    ScrollBarStyle m_style;

    Ref<ScrollBarElement> m_track;
    Ref<ScrollBarElement> m_handle;
    Ref<ScrollArrowElement> m_upArrow;
    Ref<ScrollArrowElement> m_downArrow;
    
    ScrollBarElement* m_touchedElement;
    ScrollBarElement* m_hoveredElement;

    int m_touchPrio;

    bool m_dragging = false;
    float m_touchOffset = 0.f;
    bool m_setNotEmpty = false;

    bool m_lockToScrollLayer = false;

    float m_minHandleHeight = 5.f;
};

void AdvancedScrollBar::registerDevTools() {
    devtools::registerNode<AdvancedScrollBar>([](AdvancedScrollBar* node) {
        devtools::property("Lock to ScrollLayer", node->m_impl->m_lockToScrollLayer);
        devtools::property("Min Handle Height", node->m_impl->m_minHandleHeight);

        devtools::separator();

        if (node->m_impl->m_orientation == ScrollOrientation::VERTICAL) {
            devtools::label("Padding:");

            devtools::property("Left##padding-left", node->m_impl->m_style.m_padding.left);
            devtools::property("Right##padding-right", node->m_impl->m_style.m_padding.right);
            devtools::property("Top##padding-top", node->m_impl->m_style.m_padding.top);
            devtools::property("Bottom##padding-bottom", node->m_impl->m_style.m_padding.bottom);

            devtools::separator();
            devtools::label("Margins:");

            devtools::property("Left##margins-left", node->m_impl->m_style.m_margins.left);
            devtools::property("Right##margins-right", node->m_impl->m_style.m_margins.right);
            devtools::property("Top##margins-top", node->m_impl->m_style.m_margins.top);
            devtools::property("Bottom##margins-bottom", node->m_impl->m_style.m_margins.bottom);
        }
        else {
            devtools::label("Padding:");

            devtools::property("Left##padding-left", node->m_impl->m_style.m_padding.top);
            devtools::property("Right##padding-right", node->m_impl->m_style.m_padding.bottom);
            devtools::property("Top##padding-top", node->m_impl->m_style.m_padding.right);
            devtools::property("Bottom##padding-bottom", node->m_impl->m_style.m_padding.left);

            devtools::separator();
            devtools::label("Margins:");

            devtools::property("Left##margins-left", node->m_impl->m_style.m_margins.top);
            devtools::property("Right##margins-right", node->m_impl->m_style.m_margins.bottom);
            devtools::property("Top##margins-top", node->m_impl->m_style.m_margins.right);
            devtools::property("Bottom##margins-bottom", node->m_impl->m_style.m_margins.left);
        }
    });
}

AdvancedScrollBar::AdvancedScrollBar() : m_impl(std::make_unique<Impl>()) {}
AdvancedScrollBar::~AdvancedScrollBar() = default;

AdvancedScrollBar* AdvancedScrollBar::create(AdvancedScrollLayer* scrollLayer, ScrollOrientation orientation) {
    auto ret = new AdvancedScrollBar();
    if (ret->init(scrollLayer, orientation)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool AdvancedScrollBar::init(AdvancedScrollLayer* scrollLayer, ScrollOrientation orientation) {
    if (!CCNode::init()) return false;
    if (!scrollLayer) return false;

    m_impl->m_orientation = orientation;
    m_impl->m_scrollLayer = scrollLayer;
    setTouchPriority(scrollLayer->getTouchPriority());
    setZOrder(scrollLayer->getZOrder());
    setAnchorPoint({0.5f, 0.5f});

    ignoreAnchorPointForPosition(false);

    setStyle(RoundedScrollStyle());

    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        setContentHeight(scrollLayer->getScaledContentHeight());
    }
    else {
        setRotation(-90);
        setContentHeight(scrollLayer->getScaledContentWidth());
    }

    scheduleUpdate();
    update(0);

    return true;
}

void AdvancedScrollBar::update(float dt) {

    if (m_impl->m_lockToScrollLayer && m_impl->m_scrollLayer) {
        setVisible(m_impl->m_scrollLayer->isVisible());
        
        float pageCount = 1.f;

        if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
            pageCount = m_impl->m_scrollLayer->getVerticalPages();
        }
        else {
            pageCount = m_impl->m_scrollLayer->getHorizontalPages();
        }

        pageCount = std::max(pageCount, 1.f);

        CCPoint scrollLayerBL = {m_impl->m_scrollLayer->boundingBox().getMinX(), m_impl->m_scrollLayer->boundingBox().getMinY()};

        if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
            if ((pageCount <= 1 || !m_impl->m_scrollLayer->hasVerticalScroll()) && m_impl->m_setNotEmpty) {
                m_impl->m_setNotEmpty = false;
                setVisible(false);
                m_impl->m_scrollLayer->offsetStencilWidth(0);
                m_impl->m_scrollLayer->setContentWidth(m_impl->m_scrollLayer->getContentWidth() + getScaledContentWidth());
                m_impl->m_scrollLayer->setPositionX(m_impl->m_scrollLayer->getPositionX() + getScaledContentWidth() * m_impl->m_scrollLayer->getAnchorPoint().x);
            }
            else if (pageCount > 1 && !m_impl->m_setNotEmpty) {
                m_impl->m_setNotEmpty = true;
                setVisible(true);
                m_impl->m_scrollLayer->offsetStencilWidth(getScaledContentWidth());
                m_impl->m_scrollLayer->setContentWidth(m_impl->m_scrollLayer->getScaledContentWidth() - getScaledContentWidth());
                m_impl->m_scrollLayer->setPositionX(m_impl->m_scrollLayer->getPositionX() - getScaledContentWidth() * m_impl->m_scrollLayer->getAnchorPoint().x);
            }
            setContentHeight(m_impl->m_scrollLayer->getScaledContentHeight());
            setPosition({scrollLayerBL.x + m_impl->m_scrollLayer->getScaledContentWidth() + getScaledContentWidth() / 2.f, scrollLayerBL.y + m_impl->m_scrollLayer->getScaledContentHeight()/2});
        }
        else {
            if ((pageCount <= 1 || !m_impl->m_scrollLayer->hasHorizontalScroll()) && m_impl->m_setNotEmpty) {
                m_impl->m_setNotEmpty = false;
                setVisible(false);
                m_impl->m_scrollLayer->offsetStencilHeight(0);
                m_impl->m_scrollLayer->setContentHeight(m_impl->m_scrollLayer->getContentHeight() + getScaledContentWidth());
                m_impl->m_scrollLayer->setPositionY(m_impl->m_scrollLayer->getPositionY() - getScaledContentWidth() * m_impl->m_scrollLayer->getAnchorPoint().y);
            }
            else if (pageCount > 1 && !m_impl->m_setNotEmpty) {
                m_impl->m_setNotEmpty = true;
                setVisible(true);
                m_impl->m_scrollLayer->offsetStencilHeight(getScaledContentWidth());
                m_impl->m_scrollLayer->setContentHeight(m_impl->m_scrollLayer->getContentHeight() - getScaledContentWidth());
                m_impl->m_scrollLayer->setPositionY(m_impl->m_scrollLayer->getPositionY() + getScaledContentWidth() * m_impl->m_scrollLayer->getAnchorPoint().y);
            }
            setContentHeight(m_impl->m_scrollLayer->getScaledContentWidth());
            setPosition({scrollLayerBL.x + m_impl->m_scrollLayer->getScaledContentWidth()/2, scrollLayerBL.y - getScaledContentWidth()/2.f});
        }
    }

    float pageCount = 1.f;
    float percent   = 0.f;

    if (m_impl->m_scrollLayer) {
        if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
            pageCount = m_impl->m_scrollLayer->getVerticalPages();
            percent = m_impl->m_scrollLayer->getVerticalScrollPercent();
        }
        else {
            pageCount = m_impl->m_scrollLayer->getHorizontalPages();
            percent = m_impl->m_scrollLayer->getHorizontalScrollPercent();
        }
    }

    percent = std::clamp(percent, 0.f, 1.f);
    pageCount = std::max(pageCount, 1.f);

    const float paddingTop    = m_impl->m_style.m_padding.top;
    const float paddingBottom = m_impl->m_style.m_padding.bottom;
    const float paddingLeft   = m_impl->m_style.m_padding.left;
    const float paddingRight  = m_impl->m_style.m_padding.right;

    const float marginTop    = m_impl->m_style.m_margins.top;
    const float marginBottom = m_impl->m_style.m_margins.bottom;
    const float marginLeft   = m_impl->m_style.m_margins.left;
    const float marginRight  = m_impl->m_style.m_margins.right;

    float bottomOffset = 0.f;
    float topOffset = 0.f;

    if (m_impl->m_style.m_showArrowButtons) {
        if (m_impl->m_downArrow) bottomOffset = m_impl->m_downArrow->getContentHeight();
        if (m_impl->m_upArrow) topOffset = m_impl->m_upArrow->getContentHeight();
    }

    const float contentW = getContentWidth();
    const float contentH = getContentHeight();

    const float width  = contentW - (paddingLeft + paddingRight + marginLeft + marginRight);
    const float height = std::max((contentH - (paddingTop + paddingBottom + marginTop + marginBottom + bottomOffset + topOffset)) / pageCount, m_impl->m_minHandleHeight);

    const float x = paddingLeft + marginLeft;

    const float y =
        (contentH - paddingTop - paddingBottom - marginTop - marginBottom - topOffset - bottomOffset) * (1 - percent) +
        height * percent +
        paddingBottom + marginBottom + bottomOffset;

    if (m_impl->m_handle) {
        m_impl->m_handle->setContentSize({width, height});
        m_impl->m_handle->setPosition({x, y});
        m_impl->m_handle->setVisible(std::fabs(pageCount - 1.0) >= 1e-6);
    }

    if (m_impl->m_track) {
        m_impl->m_track->setContentSize({getContentWidth() - marginLeft - marginRight, getContentHeight() - marginBottom - marginTop});
        m_impl->m_track->setPosition({marginLeft, marginBottom});
    }

    if (m_impl->m_downArrow) {
        m_impl->m_downArrow->setVisible(m_impl->m_style.m_showArrowButtons);
        m_impl->m_downArrow->setContentWidth(getContentWidth() - marginLeft - marginRight);
        m_impl->m_downArrow->setPosition({marginLeft, marginBottom});
    }

    if (m_impl->m_upArrow) {
        m_impl->m_upArrow->setVisible(m_impl->m_style.m_showArrowButtons);
        m_impl->m_upArrow->setContentWidth(getContentWidth() - marginLeft - marginRight);
        m_impl->m_upArrow->setPosition({marginLeft, getContentHeight() - marginTop});
    }
}

void AdvancedScrollBar::onEnter() {
    CCNode::onEnter();
    registerWithTouchDispatcher();
    ScrollDispatcher::get()->registerScroll(this);
    update(0);
}

void AdvancedScrollBar::onExit() {
    CCNode::onExit();
    CCTouchDispatcher::get()->removeDelegate(this);
    ScrollDispatcher::get()->unregisterScroll(this);
}

void AdvancedScrollBar::handleScroll(float y, bool smooth) {
    float bottomOffset = m_impl->m_style.m_showArrowButtons ? m_impl->m_downArrow->getContentHeight() : 0.f;
    float topOffset = m_impl->m_style.m_showArrowButtons ? m_impl->m_upArrow->getContentHeight() : 0.f;

    float min = m_impl->m_handle->getContentHeight()/2 + m_impl->m_style.m_padding.bottom + m_impl->m_style.m_margins.bottom + bottomOffset;
    float max = getContentHeight() - m_impl->m_handle->getContentHeight()/2 - (m_impl->m_style.m_padding.top + m_impl->m_style.m_margins.top + topOffset);
    
    float clamp = std::clamp(y, min, max);

    float percent = 1 - (clamp - min) / (max - min);
 
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        float pos = m_impl->m_scrollLayer->getVerticalMax() * percent;
        m_impl->m_scrollLayer->setScrollY(pos, smooth);
    }
    else {
        float pos = m_impl->m_scrollLayer->getHorizontalMax() * percent;
        m_impl->m_scrollLayer->setScrollX(pos, smooth);
    }
}

void AdvancedScrollBar::addToScroll(float amount) {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        m_impl->m_scrollLayer->setScrollY(m_impl->m_scrollLayer->getScrollPoint().y + amount, true);
    }
    else {
        m_impl->m_scrollLayer->setScrollX(m_impl->m_scrollLayer->getScrollPoint().x + amount, true);
    }
}

bool AdvancedScrollBar::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    if (!nodeIsVisible(this)) return false;
    CCPoint local = convertToNodeSpace(touch->getLocation());

    bool wasTouched = false;

    auto prevWidth = m_impl->m_handle->getContentWidth();
    m_impl->m_handle->setContentWidth(getContentWidth());
    m_impl->m_handle->setPositionX(0);

    if (alpha::utils::isTouchInsideNode(m_impl->m_handle, touch->getLocation())) {
        m_impl->m_dragging = true;
        m_impl->m_handle->onClickInternal(local);
        m_impl->m_touchOffset = local.y - m_impl->m_handle->getPositionY() + m_impl->m_handle->getContentHeight() / 2.f;
        m_impl->m_touchedElement = m_impl->m_handle;
        wasTouched = true;
    }
    else if (m_impl->m_style.m_showArrowButtons && alpha::utils::isTouchInsideNode(m_impl->m_upArrow, touch->getLocation())) {
        m_impl->m_upArrow->onClickInternal(local);
        m_impl->m_touchedElement = m_impl->m_upArrow;
        wasTouched = true;
    }
    else if (m_impl->m_style.m_showArrowButtons && alpha::utils::isTouchInsideNode(m_impl->m_downArrow, touch->getLocation())) {
        m_impl->m_downArrow->onClickInternal(local);
        m_impl->m_touchedElement = m_impl->m_downArrow;
        wasTouched = true;
    }
    else if (alpha::utils::isTouchInsideNode(m_impl->m_track, touch->getLocation())) {
        m_impl->m_track->onClickInternal(local);
        handleScroll(local.y, true);
        m_impl->m_touchedElement = m_impl->m_track;
        wasTouched = true;
    }

    m_impl->m_handle->setContentWidth(prevWidth);
    m_impl->m_handle->setPositionX(m_impl->m_style.m_padding.left + m_impl->m_style.m_margins.left);

    return wasTouched;
}

void AdvancedScrollBar::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    if (!m_impl->m_dragging) return;
    CCPoint local = convertToNodeSpace(touch->getLocation());

    handleScroll(local.y - m_impl->m_touchOffset, false);
}

void AdvancedScrollBar::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    CCPoint local = convertToNodeSpace(touch->getLocation());
    if (m_impl->m_touchedElement) m_impl->m_touchedElement->onReleaseInternal(local);
    m_impl->m_dragging = false;
}

void AdvancedScrollBar::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent* event) {
    CCPoint local = convertToNodeSpace(touch->getLocation());
    if (m_impl->m_touchedElement) m_impl->m_touchedElement->onReleaseInternal(local);
    m_impl->m_dragging = false;
}

bool AdvancedScrollBar::mouseEntered(TouchEvent* touch) {
    if (!nodeIsVisible(this)) return false;
    CCPoint local = convertToNodeSpace(touch->getLocation());

    if (alpha::utils::isTouchInsideNode(m_impl->m_handle, touch->getLocation())) {
        m_impl->m_hoveredElement = m_impl->m_handle;
    }
    else if (m_impl->m_style.m_showArrowButtons && alpha::utils::isTouchInsideNode(m_impl->m_upArrow, touch->getLocation())) {
        m_impl->m_hoveredElement = m_impl->m_upArrow;
    }
    else if (m_impl->m_style.m_showArrowButtons && alpha::utils::isTouchInsideNode(m_impl->m_downArrow, touch->getLocation())) {
        m_impl->m_hoveredElement = m_impl->m_downArrow;
    }
    else if (alpha::utils::isTouchInsideNode(m_impl->m_track, touch->getLocation())) {
        m_impl->m_hoveredElement = m_impl->m_track;
    }

    if (m_impl->m_hoveredElement) m_impl->m_hoveredElement->onMouseEnterInternal(local);

    return true;
}

void AdvancedScrollBar::mouseMoved(TouchEvent* touch) {
    CCPoint local = convertToNodeSpace(touch->getLocation());

    auto currentHover = m_impl->m_hoveredElement;

    if (alpha::utils::isTouchInsideNode(m_impl->m_handle, touch->getLocation())) {
        m_impl->m_hoveredElement = m_impl->m_handle;
    }
    else if (m_impl->m_style.m_showArrowButtons && alpha::utils::isTouchInsideNode(m_impl->m_upArrow, touch->getLocation())) {
        m_impl->m_hoveredElement = m_impl->m_upArrow;
    }
    else if (m_impl->m_style.m_showArrowButtons && alpha::utils::isTouchInsideNode(m_impl->m_downArrow, touch->getLocation())) {
        m_impl->m_hoveredElement = m_impl->m_downArrow;
    }
    else if (alpha::utils::isTouchInsideNode(m_impl->m_track, touch->getLocation())) {
        m_impl->m_hoveredElement = m_impl->m_track;
    }

    if (currentHover && currentHover != m_impl->m_hoveredElement) {
        currentHover->onMouseExitInternal(local);
        m_impl->m_hoveredElement->onMouseEnterInternal(local);
    }

    if (m_impl->m_hoveredElement) m_impl->m_hoveredElement->onMouseMoveInternal(local);
}

void AdvancedScrollBar::mouseExited(TouchEvent* touch) {
    CCPoint local = convertToNodeSpace(touch->getLocation());
    if (m_impl->m_hoveredElement) m_impl->m_hoveredElement->onMouseExitInternal(local);
}

void AdvancedScrollBar::setTouchPriority(int prio) {
    m_impl->m_touchPrio = prio;
}

int AdvancedScrollBar::getTouchPriority() {
    return m_impl->m_touchPrio;
}

void AdvancedScrollBar::registerWithTouchDispatcher() {
    CCTouchDispatcher::get()->addTargetedDelegate(this, getTouchPriority(), true);
}

void AdvancedScrollBar::scroll(float x, float y) {
    m_impl->m_scrollLayer->scroll(x, y);
}

void AdvancedScrollBar::keyPress(cocos2d::enumKeyCodes key, bool isKeyDown, bool isKeyRepeat) {
    m_impl->m_scrollLayer->keyPress(key, isKeyDown, isKeyRepeat);
}

void AdvancedScrollBar::lockToScrollLayer(bool lock) {
    m_impl->m_lockToScrollLayer = lock;
    update(0);
}

bool AdvancedScrollBar::isLockedToScrollLayer() {
    return m_impl->m_lockToScrollLayer;
}

Insets AdvancedScrollBar::getPadding() {
    if (m_impl->m_orientation == ScrollOrientation::HORIZONTAL) {
        return alpha::utils::rotateInsetsCCW(m_impl->m_style.m_padding);
    }
    return m_impl->m_style.m_padding;
}

void AdvancedScrollBar::setPadding(const Insets& padding) {
    if (m_impl->m_orientation == ScrollOrientation::HORIZONTAL) {
        m_impl->m_style.m_padding = alpha::utils::rotateInsetsCW(padding);
        return;
    }
    m_impl->m_style.m_padding = padding;
}

void AdvancedScrollBar::setPadding(float left, float right, float top, float bottom) {
    Insets insets = {left, right, top, bottom};
    setPadding(insets);
}

void AdvancedScrollBar::setPaddingLeft(float left) {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        m_impl->m_style.m_padding.left = left;
    }
    else {
        m_impl->m_style.m_padding.top = left;
    }
}

void AdvancedScrollBar::setPaddingRight(float right) {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        m_impl->m_style.m_padding.right = right;
    }
    else {
        m_impl->m_style.m_padding.bottom = right;
    }
}

void AdvancedScrollBar::setPaddingTop(float top) {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        m_impl->m_style.m_padding.top = top;
    }
    else {
        m_impl->m_style.m_padding.right = top;
    }
}

void AdvancedScrollBar::setPaddingBottom(float bottom) {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        m_impl->m_style.m_padding.bottom = bottom;
    }
    else {
        m_impl->m_style.m_padding.left = bottom;
    }
}

float AdvancedScrollBar::getPaddingLeft() {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        return m_impl->m_style.m_padding.left;
    }
    return m_impl->m_style.m_padding.top;
}

float AdvancedScrollBar::getPaddingRight() {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        return m_impl->m_style.m_padding.right;
    }
    return m_impl->m_style.m_padding.bottom;
}

float AdvancedScrollBar::getPaddingTop() {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        return m_impl->m_style.m_padding.top;
    }
    return m_impl->m_style.m_padding.right;
}

float AdvancedScrollBar::getPaddingBottom() {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        return m_impl->m_style.m_padding.bottom;
    }
    return m_impl->m_style.m_padding.left;
}

Insets AdvancedScrollBar::getMargins() {
    if (m_impl->m_orientation == ScrollOrientation::HORIZONTAL) {
        return alpha::utils::rotateInsetsCCW(m_impl->m_style.m_margins);
    }
    return m_impl->m_style.m_margins;
}

void AdvancedScrollBar::setMargins(const Insets& margins) {
    if (m_impl->m_orientation == ScrollOrientation::HORIZONTAL) {
        m_impl->m_style.m_margins = alpha::utils::rotateInsetsCW(margins);
        return;
    }
    m_impl->m_style.m_margins = margins;
}

void AdvancedScrollBar::setMargins(float left, float right, float top, float bottom) {
    Insets insets = {left, right, top, bottom};
    setMargins(insets);
}

void AdvancedScrollBar::setMarginLeft(float left) {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        m_impl->m_style.m_margins.left = left;
    }
    else {
        m_impl->m_style.m_margins.top = left;
    }
}

void AdvancedScrollBar::setMarginRight(float right) {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        m_impl->m_style.m_margins.right = right;
    }
    else {
        m_impl->m_style.m_margins.bottom = right;
    }
}

void AdvancedScrollBar::setMarginTop(float top) {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        m_impl->m_style.m_margins.top = top;
    }
    else {
        m_impl->m_style.m_margins.right = top;
    }
}

void AdvancedScrollBar::setMarginBottom(float bottom) {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        m_impl->m_style.m_margins.bottom = bottom;
    }
    else {
        m_impl->m_style.m_margins.left = bottom;
    }
}

float AdvancedScrollBar::getMarginLeft() {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        return m_impl->m_style.m_margins.left;
    }
    return m_impl->m_style.m_margins.top;
}

float AdvancedScrollBar::getMarginRight() {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        return m_impl->m_style.m_margins.right;
    }
    return m_impl->m_style.m_margins.bottom;
}

float AdvancedScrollBar::getMarginTop() {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        return m_impl->m_style.m_margins.top;
    }
    return m_impl->m_style.m_margins.right;
}

float AdvancedScrollBar::getMarginBottom() {
    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        return m_impl->m_style.m_margins.bottom;
    }
    return m_impl->m_style.m_margins.left;
}

void AdvancedScrollBar::setStyle(const ScrollBarStyle& style) {
    setTrack(style.getTrack());
    setHandle(style.getHandle());
    setUpArrow(style.getUpArrow());
    setDownArrow(style.getDownArrow());
    setMargins(style.m_margins);
    setPadding(style.m_padding);
    showArrowButtons(style.m_showArrowButtons);

    m_impl->m_upArrow->setContentHeight(style.m_arrowButtonHeight);
    m_impl->m_downArrow->setContentHeight(style.m_arrowButtonHeight);

    setContentWidth(style.m_scrollBarWidth);

    auto bounds = m_impl->m_scrollLayer->boundingBox();

    if (m_impl->m_orientation == ScrollOrientation::VERTICAL) {
        setPosition({bounds.getMaxX() + getScaledContentWidth()/2.f + style.m_distanceFromScrollLayer, bounds.getMinY() + m_impl->m_scrollLayer->getScaledContentHeight()/2.f});
    }
    else {
        setPosition({bounds.getMinX() + m_impl->m_scrollLayer->getScaledContentWidth()/2, bounds.getMinY() - getScaledContentWidth()/2.f - style.m_distanceFromScrollLayer});
    }
}

void AdvancedScrollBar::setTrack(ScrollBarElement* track) {
    if (m_impl->m_track) m_impl->m_track->removeFromParent();
    if (!track) {
        track = BasicScrollTrack::create();
    }
    track->setScrollBar(this);
    track->setAnchorPoint({0.f, 0.f});
    addChild(track);

    m_impl->m_track = track;

    update(0);
}

void AdvancedScrollBar::setHandle(ScrollBarElement* handle) {
    if (m_impl->m_handle) m_impl->m_handle->removeFromParent();
    if (!handle) {
        handle = BasicScrollHandle::create();
    }
    handle->setScrollBar(this);
    handle->setZOrder(1);
    handle->setAnchorPoint({0.f, 1.f});
    addChild(handle);

    m_impl->m_handle = handle;

    update(0);
}

void AdvancedScrollBar::setMinHandleHeight(float height) {
    m_impl->m_minHandleHeight = height;
}

float AdvancedScrollBar::getMinHandleHeight() {
    return m_impl->m_minHandleHeight;
}

ScrollBarElement* AdvancedScrollBar::getTrack() {
    return m_impl->m_track;
}

ScrollBarElement* AdvancedScrollBar::getHandle() {
    return m_impl->m_handle;
}

void AdvancedScrollBar::showArrowButtons(bool show) {
    m_impl->m_style.m_showArrowButtons = show;
}

bool AdvancedScrollBar::hasArrowButtons() {
    return m_impl->m_style.m_showArrowButtons;
}

void AdvancedScrollBar::setUpArrow(ScrollArrowElement* upArrow) {
    if (m_impl->m_upArrow) m_impl->m_upArrow->removeFromParent();
    if (!upArrow) {
        upArrow = BasicScrollArrow::create();
    }
    upArrow->setScrollBar(this);
    upArrow->setZOrder(1);
    upArrow->setAnchorPoint({0.f, 1.f});
    upArrow->setDirection(ArrowDirection::UP);
    addChild(upArrow);

    m_impl->m_upArrow = upArrow;

    update(0);
}

void AdvancedScrollBar::setDownArrow(ScrollArrowElement* downArrow) {
    if (m_impl->m_downArrow) m_impl->m_downArrow->removeFromParent();
    if (!downArrow) {
        downArrow = BasicScrollArrow::create();
    }
    downArrow->setScrollBar(this);
    downArrow->setZOrder(1);
    downArrow->setAnchorPoint({0.f, 0.f});
    downArrow->setDirection(ArrowDirection::DOWN);
    addChild(downArrow);

    m_impl->m_downArrow = downArrow;

    update(0);
}

ScrollArrowElement* AdvancedScrollBar::getUpArrow() {
    return m_impl->m_upArrow;
}

ScrollArrowElement* AdvancedScrollBar::getDownArrow() {
    return m_impl->m_downArrow;
}

ScrollOrientation AdvancedScrollBar::getOrientation() {
    return m_impl->m_orientation;
}

$execute {
    devtools::waitForDevTools([] {
        AdvancedScrollBar::registerDevTools();
    });
}