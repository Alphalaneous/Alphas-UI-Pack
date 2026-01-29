#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

ScrollDispatcher* ScrollDispatcher::s_instance;

struct ScrollDispatcher::Impl final {
    std::vector<AdvancedScrollDelegate*> m_scrollDelegates;
};

ScrollDispatcher::ScrollDispatcher() : m_impl(std::make_unique<Impl>()) {}
ScrollDispatcher::~ScrollDispatcher() = default;

ScrollDispatcher* ScrollDispatcher::get() {
    if (!s_instance) s_instance = new ScrollDispatcher();
    return s_instance;
}

void ScrollDispatcher::registerScroll(AdvancedScrollDelegate* delegate) {
    m_impl->m_scrollDelegates.insert(m_impl->m_scrollDelegates.begin(), delegate);
}

void ScrollDispatcher::unregisterScroll(AdvancedScrollDelegate* delegate) {
    m_impl->m_scrollDelegates.erase(std::find(m_impl->m_scrollDelegates.begin(), m_impl->m_scrollDelegates.end(), delegate));
}

const std::vector<AdvancedScrollDelegate*>& ScrollDispatcher::getDelegates() {
    return m_impl->m_scrollDelegates;
}

#ifndef GEODE_IS_IOS
#include <Geode/modify/CCMouseDispatcher.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>

class $modify(ScrollCCMouseDispatcher, cocos2d::CCMouseDispatcher) {
	bool dispatchScrollMSG(float y, float x) {
        for (AdvancedScrollDelegate* scrollDelegate : ScrollDispatcher::get()->getDelegates()) {
            if (!alpha::utils::isPointInsideNode(typeinfo_cast<CCNode*>(scrollDelegate), getMousePos())) continue;

            bool shouldScroll = true;

            if (CCKeyboardDispatcher::get()->getControlKeyPressed() || CCKeyboardDispatcher::get()->getCommandKeyPressed()) {
                scrollDelegate->zoom(-y);
                shouldScroll = false;
            }

            if (shouldScroll) {
                if (CCKeyboardDispatcher::get()->getShiftKeyPressed()) {
                    std::swap(x, y);
                }

                scrollDelegate->scroll(x, y);
            }
            break;
        }
        return CCMouseDispatcher::dispatchScrollMSG(y, x);
    }
};

class $modify(ScrollCCKeyboardDispatcher, cocos2d::CCKeyboardDispatcher) {
	bool dispatchKeyboardMSG(cocos2d::enumKeyCodes key, bool isKeyDown, bool isKeyRepeat, double t) {
        for (AdvancedScrollDelegate* scrollDelegate : ScrollDispatcher::get()->getDelegates()) {
            if (!alpha::utils::isPointInsideNode(typeinfo_cast<CCNode*>(scrollDelegate), getMousePos())) continue;

            if (CCKeyboardDispatcher::get()->getControlKeyPressed() || CCKeyboardDispatcher::get()->getCommandKeyPressed()) {
                if (isKeyDown) {
                    if (key == enumKeyCodes::KEY_OEMEqual) {
                        scrollDelegate->zoom(25);
                    }
                    else if (key == enumKeyCodes::KEY_OEMMinus) {
                        scrollDelegate->zoom(-25);
                    }
                }
            }

            scrollDelegate->keyPress(key, isKeyDown, isKeyRepeat);
            break;
        }
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, isKeyDown, isKeyRepeat, t);
    }
};
#endif