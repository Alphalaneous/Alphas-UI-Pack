#include "touch/TouchDispatcher.hpp"
#include <Geode/Geode.hpp>
#include "API.hpp"
#include "Geode/cocos/CCDirector.h"
#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/robtop/touch_dispatcher/CCTouchClipper.h"
#include "Geode/cocos/touch_dispatcher/CCTouchDelegateProtocol.h"
#include "Geode/loader/Log.hpp"
#include "Geode/utils/cocos.hpp"
#include "Utils.hpp"
#include "touch/Hover.hpp"
#include "touch/TouchDelegate.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct TouchDispatcher::Impl final {
    std::unordered_map<geode::MouseInputData::Button, bool> m_inputStates;
    std::unordered_map<geode::MouseInputData::Button, Ref<Touch>> m_clicks;

    CCPoint m_lastPos;

	CCTouchDispatcher* m_dispatcher;
	Ref<Hover> m_hoverTouch;

    ListenerHandle m_touchListener;
};

TouchDispatcher::TouchDispatcher() : m_impl(std::make_unique<Impl>()) {
    m_impl->m_dispatcher = CCTouchDispatcher::get();
    m_impl->m_hoverTouch = Hover::create();

    m_impl->m_touchListener = MouseInputEvent().listen([this] (geode::MouseInputData& data) {
        bool pressed = data.action == geode::MouseInputData::Action::Press;
        auto origStates = m_impl->m_inputStates;

        switch (data.button) {
            case geode::MouseInputData::Button::Middle: {
                m_impl->m_inputStates[geode::MouseInputData::Button::Middle] = pressed;
                break;
            }
            case geode::MouseInputData::Button::Right: {
                m_impl->m_inputStates[geode::MouseInputData::Button::Right] = pressed;
                break;
            }
            case geode::MouseInputData::Button::Button4: {
                m_impl->m_inputStates[geode::MouseInputData::Button::Button4] = pressed;
                break;
            }
            case geode::MouseInputData::Button::Button5: {
                m_impl->m_inputStates[geode::MouseInputData::Button::Button5] = pressed;
                break;
            }
            default: break;
        }

        auto winSize = CCDirector::get()->getWinSize();
        auto pos = getMousePos();
        for (const auto& [button, state] : m_impl->m_inputStates) {
            if (state != origStates[button]) {
                if (state) {
                    auto click = Touch::create(button);
                    click->setTouchInfo(0, pos.x, winSize.height - pos.y);
                    m_impl->m_clicks[button] = click;
                    clicks(click, TouchType::ClickBegan);
                }
                else {
                    clicks(m_impl->m_clicks[button], TouchType::ClickEnded);
                    m_impl->m_clicks.erase(button);
                }
            }
        }
    });

    schedule(schedule_selector(TouchDispatcher::checkTouch));
    onEnter();
}

TouchDispatcher::~TouchDispatcher() = default;

TouchDispatcher* TouchDispatcher::get() {
    static TouchDispatcher instance;
    return &instance;
}

void TouchDispatcher::checkTouch(float dt) {
    if (m_impl->m_dispatcher->m_bDispatchEvents) {
        auto winSize = CCDirector::get()->getWinSize();
        auto pos = getMousePos();
        m_impl->m_hoverTouch->setTouchInfo(0, pos.x, winSize.height - pos.y);
        hovers(m_impl->m_hoverTouch);
    }

    auto winSize = CCDirector::get()->getWinSize();
    auto pos = getMousePos();
    
    if (m_impl->m_lastPos != pos) {
        for (const auto& [button, click] : m_impl->m_clicks) {
            click->setTouchInfo(0, pos.x, winSize.height - pos.y);
            clicks(click, TouchType::ClickMoved);
        }
    }

    m_impl->m_lastPos = pos;
}

// yoinked from ZOrderTouchPrio.cpp in Geode, modified to support all mouse buttons
namespace {
    struct ParentPath {
        std::vector<CCNode*> m_path;
        CCTargetedTouchHandler* m_handler = nullptr;
        mutable bool m_hasInvalidRoot = false;

        ParentPath(CCTargetedTouchHandler* handler) : m_handler(handler) {}

        ParentPath(ParentPath&& other) = default;
        ParentPath& operator=(ParentPath&& other) = default;

        ParentPath(CCNode* node, CCTargetedTouchHandler* handler) : m_handler(handler) {
            while (node) {
                m_path.push_back(node);
                node = node->getParent();
            }
        }

        static std::optional<ParentPath> filtered(CCNode* node, CCTargetedTouchHandler* handler, CCNode* filter) {
            ParentPath ret{handler};
            bool confirmed = false;
            while (node) {
                ret.m_path.push_back(node);
                if (node == filter) confirmed = true;
                node = node->getParent();
            }
            if (confirmed) return ret;
            return std::nullopt;
        }

        CCNode* leaf() const {
            return m_path.empty() ? nullptr : m_path.front();
        }

        CCNode* root() const {
            return m_path.empty() ? nullptr : m_path.back();
        }

        CCNode* nth(size_t n) const {
            return (n < m_path.size()) ? m_path[m_path.size() - 1 - n] : nullptr;
        }

        bool swallows() const {
            return m_handler->m_bSwallowsTouches;
        }

        bool isStealer(CCNode* node) const {
            if (node->getUserFlag("geode.loader/steals-touch")) return true;

            if (typeinfo_cast<TableView*>(node)) return true;
            if (typeinfo_cast<BoomScrollLayer*>(node)) return true;

            if (auto scrollLayer = typeinfo_cast<ScrollLayer*>(node)) {
                return scrollLayer->isStealingTouches();
            }
            return false;
        }

        bool steals() const {
            if (swallows()) return false;

            if (auto leaf = this->leaf()) {
                if (isStealer(leaf)) return true;
            }

            return false;
        }

        bool hasValidRoot() {
            if (m_hasInvalidRoot) return false;

            auto director = CCDirector::get();
            const auto rootOrder = std::array<CCNode*, 2>{director->m_pRunningScene, director->m_pNotificationNode};
            auto thisRootIt = std::find(rootOrder.begin(), rootOrder.end(), root());

            if (thisRootIt == rootOrder.end()) {
                m_hasInvalidRoot = true;
                return false;
            }
            return true;
        }

        bool compareRoots(const ParentPath& other) const {
            if (root() == other.root()) return false;

            auto director = CCDirector::get();
            const std::array<CCNode*, 2> rootOrder = {director->m_pRunningScene, director->m_pNotificationNode};
            auto thisRootIt = std::find(rootOrder.begin(), rootOrder.end(), root());
            auto otherRootIt = std::find(rootOrder.begin(), rootOrder.end(), other.root());

            if (thisRootIt == rootOrder.end()) {
                m_hasInvalidRoot = true;
                return false;
            }
            if (otherRootIt == rootOrder.end()) {
                other.m_hasInvalidRoot = true;
                return false;
            }

            return thisRootIt > otherRootIt;
        }

        bool operator<(const ParentPath& other) const {
            if (root() != other.root()) {
                return compareRoots(other);
            }

            size_t maxLength = std::max(m_path.size(), other.m_path.size());
            for (size_t divergeIndex = 1; divergeIndex < maxLength; ++divergeIndex) {
                auto thisParent = nth(divergeIndex);
                auto otherParent = other.nth(divergeIndex);

                if (!thisParent && otherParent) {
                    return steals();
                }
                else if (!otherParent && thisParent) {
                    return !other.steals();
                }
                else if (thisParent != otherParent) {
                    if (thisParent->getZOrder() == otherParent->getZOrder()) {
                        return thisParent->getOrderOfArrival() > otherParent->getOrderOfArrival();
                    }
                    return thisParent->getZOrder() > otherParent->getZOrder();
                }
            }
            return false;
        }
    };

    std::vector<ParentPath> getRegisteredPaths(CCArray* handlers) {
        std::vector<ParentPath> paths;
        for (auto handler : CCArrayExt<CCTargetedTouchHandler*>(handlers)) {
            if (!handler) continue;

            auto delegate = handler->getDelegate();
            if (!delegate) continue;

            auto node = typeinfo_cast<CCNode*>(delegate);
            if (!node) continue;

            auto path = ParentPath(node, handler);
            if (path.hasValidRoot()) {
                paths.push_back(std::move(path));
            }
        }
        std::sort(paths.begin(), paths.end());
        return paths;
    }

    bool handleTouch(Touch* touch, TouchType touchType, const std::vector<ParentPath>& registeredPaths) {
        bool touchClaimed = false;

        for (auto& path : registeredPaths) {
            auto delegate = typeinfo_cast<TouchDelegate*>(path.m_handler->getDelegate());
            if (!delegate) continue;

            auto claimedTouches = path.m_handler->m_pClaimedTouches;
            auto swallowsTouches = path.m_handler->m_bSwallowsTouches;

            if (path.m_hasInvalidRoot) continue;

            auto node = typeinfo_cast<CCNode*>(delegate);
            if (!node) continue;
            
            bool claimed = false;
            if (touchType == alpha::dispatcher::TouchType::ClickBegan) {
                claimed = delegate->touchBegan(touch);

                if (claimed) {
                    claimedTouches->addObject(touch);
                }
            }
            else if (claimedTouches->containsObject(touch)) {
                claimed = true;

                switch (touchType) {
                    case alpha::dispatcher::TouchType::ClickMoved: 
                        delegate->touchMoved(touch); 
                        break;
                    case alpha::dispatcher::TouchType::ClickEnded:
                        delegate->touchEnded(touch);
                        claimedTouches->removeObject(touch);
                        break;
                    case alpha::dispatcher::TouchType::ClickCancelled:
                        delegate->touchCancelled(touch);
                        claimedTouches->removeObject(touch);
                        break;
                    default:
                        break;
                }
            }

            if (claimed && swallowsTouches) {
                touchClaimed = true;
                break;
            }
        }
        return touchClaimed;
    }

    bool handleHover(Hover* hover, const std::vector<ParentPath>& registeredPaths) {
        bool hoverClaimed = false;

        for (auto& path : registeredPaths) {
            auto delegate = typeinfo_cast<CCTouchDelegate*>(path.m_handler->getDelegate());

            if (!delegate) continue;

            if (path.m_hasInvalidRoot) continue;

            auto node = typeinfo_cast<CCNode*>(delegate);
            if (!node) continue;

            auto claimedTouches = path.m_handler->m_pClaimedTouches;
            auto swallowsTouches = path.m_handler->m_bSwallowsTouches;

            bool inside = alpha::utils::isPointInsideNode(node, hover->getLocation()) && alpha::utils::canTouch(node, hover->getLocation());

            auto touchDelegate = typeinfo_cast<TouchDelegate*>(delegate);
            if (!touchDelegate) {
                if (swallowsTouches && inside) {
                    hoverClaimed = true;
                }
                continue;
            }

            bool entered = inside && !hoverClaimed;
            bool isClaimed = claimedTouches->containsObject(hover) && nodeIsVisible(node);

            if (inside && !isClaimed) {
                isClaimed = touchDelegate->hoverEntered(hover);
                if (isClaimed) {
                    claimedTouches->addObject(hover);
                }
            }
            if (isClaimed && !entered) {
                touchDelegate->hoverExited(hover);
                claimedTouches->removeObject(hover);
            }

            if (entered && isClaimed) {
                touchDelegate->hoverMoved(hover);
            }

            if (isClaimed && swallowsTouches) {
                hoverClaimed = true;
                break;
            }
        }
        return hoverClaimed;
    }
}

void TouchDispatcher::cleanupHandlers() {
    if (m_impl->m_dispatcher->m_bToRemove) {
        m_impl->m_dispatcher->m_bToRemove = false;
        for (unsigned int i = 0; i < m_impl->m_dispatcher->m_pHandlersToRemove->num; ++i) {
            for (auto handler : CCArrayExt<CCTargetedTouchHandler*>(m_impl->m_dispatcher->m_pTargetedHandlers)) {
                if (handler->getDelegate() == m_impl->m_dispatcher->m_pHandlersToRemove->arr[i]) {
                    m_impl->m_dispatcher->m_pTargetedHandlers->removeObject(handler);
                    break;
                }
            }
            for (auto handler : CCArrayExt<CCStandardTouchHandler*>(m_impl->m_dispatcher->m_pStandardHandlers)) {
                if (handler->getDelegate() == m_impl->m_dispatcher->m_pHandlersToRemove->arr[i]) {
                    m_impl->m_dispatcher->m_pStandardHandlers->removeObject(handler);
                    break;
                }
            }
        }
        m_impl->m_dispatcher->m_pHandlersToRemove->num = 0;
    }

    if (m_impl->m_dispatcher->m_bToAdd) {
        m_impl->m_dispatcher->m_bToAdd = false;
        for (auto handler : CCArrayExt<CCTouchHandler*>(m_impl->m_dispatcher->m_pHandlersToAdd)) {
            if (!handler) continue;

            if (typeinfo_cast<CCTargetedTouchHandler*>(handler)) {
                if (!m_impl->m_dispatcher->m_pTargetedHandlers->containsObject(handler)) {
                    m_impl->m_dispatcher->m_pTargetedHandlers->addObject(handler);
                }
            }
            else {
                if (!m_impl->m_dispatcher->m_pStandardHandlers->containsObject(handler)) {
                    m_impl->m_dispatcher->m_pStandardHandlers->addObject(handler);
                }
            }
        }

        m_impl->m_dispatcher->m_pHandlersToAdd->removeAllObjects();
    }

    if (m_impl->m_dispatcher->m_bToQuit) {
        m_impl->m_dispatcher->m_bToQuit = false;
        m_impl->m_dispatcher->m_pStandardHandlers->removeAllObjects();
        m_impl->m_dispatcher->m_pTargetedHandlers->removeAllObjects();
    }

    m_impl->m_dispatcher->m_bLocked = false;
}

void TouchDispatcher::hovers(Hover* hover) {
    if (!hover) return;

    if (m_impl->m_dispatcher->m_pTargetedHandlers->count() == 0) return;

    m_impl->m_dispatcher->m_bLocked = true;
    
    if (m_impl->m_dispatcher->m_pTargetedHandlers->count() > 0) {
        Ref<CCArray> handlersCopy = CCTouchDispatcher::get()->m_pTargetedHandlers->shallowCopy();
        auto registeredPaths = ::getRegisteredPaths(CCTouchDispatcher::get()->m_pTargetedHandlers);

        ::handleHover(hover, registeredPaths);
    }

    cleanupHandlers();
}

void TouchDispatcher::clicks(Touch* touch, TouchType type) {
    if (m_impl->m_dispatcher->m_pTargetedHandlers->count() == 0) return;

    m_impl->m_dispatcher->m_bLocked = true;

    if (m_impl->m_dispatcher->m_pTargetedHandlers->count() > 0) {
        Ref<CCArray> handlersCopy = CCTouchDispatcher::get()->m_pTargetedHandlers->shallowCopy();
        auto registeredPaths = ::getRegisteredPaths(CCTouchDispatcher::get()->m_pTargetedHandlers);

        ::handleTouch(touch, type, registeredPaths);
    }
    
    cleanupHandlers();
}

$on_mod(Loaded) {
    // init it :3
    TouchDispatcher::get();
}