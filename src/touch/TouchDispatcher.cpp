#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct TouchDispatcher::Impl final {
    std::unordered_map<MouseButton, bool> m_states;
    std::unordered_map<MouseButton, Ref<TouchEvent>> m_clicks;

    CCPoint m_lastPos;
    bool m_shouldRearrangeHandlers;

	CCTouchDispatcher* m_dispatcher;
	Ref<TouchEvent> m_hoverTouch;
};

TouchDispatcher::TouchDispatcher() : m_impl(std::make_unique<Impl>()) {}
TouchDispatcher::~TouchDispatcher() = default;

TouchDispatcher* TouchDispatcher::get() {
    static TouchDispatcher instance;
    static bool initialized = false;
    if (!initialized) {
        instance.init();
        initialized = true;
    }
    return &instance;
}

void TouchDispatcher::init() {
    m_impl->m_dispatcher = CCTouchDispatcher::get();
    m_impl->m_hoverTouch = TouchEvent::create(MouseButton::HOVER);
}

void TouchDispatcher::queueSort() {
    m_impl->m_shouldRearrangeHandlers = true;
}

void TouchDispatcher::rearrangeHandlers() {
    if (m_impl->m_shouldRearrangeHandlers) {
        auto handlers = CCArrayExt<CCTouchHandler*>(m_impl->m_dispatcher->m_pTargetedHandlers);
        std::sort(handlers.begin(), handlers.end(), [](CCTouchHandler* a, CCTouchHandler* b) {
            return a->getPriority() < b->getPriority();
        });
        m_impl->m_shouldRearrangeHandlers = false;
    }
}

void TouchDispatcher::hovers(TouchEvent* touch) {
    bool hoverClaimed = false;

    for (auto handler : CCArrayExt<CCTargetedTouchHandler*>(m_impl->m_dispatcher->m_pTargetedHandlers)) {
        auto node = typeinfo_cast<CCNode*>(handler->getDelegate());
        if (!node) continue;

        const CCPoint location = touch->getLocation();

        bool inside = alpha::utils::isTouchInsideNode(node, location);

        if (auto scrollLayer = static_cast<CCNode*>(node->getUserObject("scroll-layer"_spr))) {
            inside = alpha::utils::isTouchInsideNode(scrollLayer, location) && inside;
        }

        const bool swallows = handler->m_bSwallowsTouches;
        auto claimed = handler->getClaimedTouches();

        auto delegate = typeinfo_cast<TouchDelegate*>(handler->getDelegate());
        if (!delegate) {
            if (swallows && inside) {
                hoverClaimed = true;
            }
            continue;
        }

        bool entered = inside && !hoverClaimed;
        bool isClaimed = claimed->containsObject(touch);

        if (entered && !isClaimed) {
            isClaimed = delegate->mouseEntered(touch);
            if (isClaimed) {
                claimed->addObject(touch);
            }
        }
        if (isClaimed && !entered) {
            delegate->mouseExited(touch);
            claimed->removeObject(touch);
        }

        if (entered && isClaimed) {
            delegate->mouseMoved(touch);
        }

        if (isClaimed && swallows) {
            hoverClaimed = true;
        }
    }
}

void TouchDispatcher::clicks(TouchEvent* touch, TouchType type) {
    if (m_impl->m_dispatcher->m_pTargetedHandlers->count() == 0) return;

    m_impl->m_dispatcher->m_bLocked = true;
    
    const auto button = touch->getButton();
    const CCPoint location = touch->getLocation();

    for (auto handler : CCArrayExt<CCTargetedTouchHandler*>(m_impl->m_dispatcher->m_pTargetedHandlers)) {
        bool touchClaimed = false;
        auto node = typeinfo_cast<CCNode*>(handler->getDelegate());
        bool insideNode = true;
        if (node) insideNode = alpha::utils::isTouchInsideNode(node, location);

        auto delegate = typeinfo_cast<TouchDelegate*>(handler->getDelegate());
        if (!delegate) {
            const bool swallows = handler->m_bSwallowsTouches;

            if (swallows && insideNode) {
                break;
            }
            continue;
        }

        if (node) {
            if (auto scrollLayer = static_cast<CCNode*>(node->getUserObject("scroll-layer"_spr))) {
                bool inside = alpha::utils::isTouchInsideNode(scrollLayer, location) && insideNode;
                if (!inside) continue;
            }
        }

        if (type == TouchType::CLICK_BEGAN) {
            touchClaimed = delegate->clickBegan(touch);

            if (touchClaimed) {
                handler->getClaimedTouches()->addObject(touch);
            }
        }
        else if (handler->getClaimedTouches()->containsObject(touch)) {
            touchClaimed = true;

            switch (type) {
                case TouchType::CLICK_MOVED: 
                    delegate->clickMoved(touch);
                    break;
                case TouchType::CLICK_ENDED: 
                    delegate->clickEnded(touch);
                    break;
                default:
                    break;
            }
        }

        if (touchClaimed && handler->m_bSwallowsTouches) {
            break;
        }
    }
    
    m_impl->m_dispatcher->m_bLocked = false;
}

#ifdef GEODE_IS_WINDOWS
void TouchDispatcher::pollInput(const CCPoint& pos) {
    auto origStates = m_impl->m_states;

    m_impl->m_states[MouseButton::MIDDLE]   = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
    m_impl->m_states[MouseButton::RIGHT]    = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
    m_impl->m_states[MouseButton::BUTTON3]  = GetAsyncKeyState(VK_XBUTTON1) & 0x8000;
    m_impl->m_states[MouseButton::BUTTON4]  = GetAsyncKeyState(VK_XBUTTON2) & 0x8000;

    for (const auto& [button, state] : m_impl->m_states) {
        if (state != origStates[button]) {
            if (state) {
                auto click = TouchEvent::create(button);
                click->setTouchInfo(pos);
                m_impl->m_clicks[button] = click;
                clicks(click, TouchType::CLICK_BEGAN);
            }
            else {
                clicks(m_impl->m_clicks[button], TouchType::CLICK_ENDED);
                m_impl->m_clicks.erase(button);
            }
        }
    }
    
    if (m_impl->m_lastPos != pos) {
        for (const auto& [button, click] : m_impl->m_clicks) {
            click->setTouchInfo(pos);
            clicks(click, TouchType::CLICK_MOVED);
        }
    }

    m_impl->m_lastPos = pos;
}
#endif

void TouchDispatcher::handleHover(const CCPoint& pos) {
    m_impl->m_hoverTouch->setTouchInfo(pos);
    hovers(m_impl->m_hoverTouch);
}

void TouchDispatcher::update(float dt) {
    if (m_impl->m_dispatcher->m_bToAdd) {
        m_impl->m_dispatcher->m_bToAdd = false;
        for (auto handler : CCArrayExt<CCTouchHandler*>(m_impl->m_dispatcher->m_pHandlersToAdd)) {
            m_impl->m_dispatcher->m_pTargetedHandlers->addObject(handler);
            m_impl->m_shouldRearrangeHandlers = true;
        }
    }
    rearrangeHandlers();

    if (m_impl->m_dispatcher->m_bDispatchEvents) {
        auto mousePos = getMousePos();

        handleHover(mousePos);
        pollInput(mousePos);
    }
}

$execute {
	CCScheduler::get()->scheduleUpdateForTarget(TouchDispatcher::get(), INT_MIN, false);
}