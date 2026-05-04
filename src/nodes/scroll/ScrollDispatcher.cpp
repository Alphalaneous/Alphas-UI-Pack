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

#ifdef GEODE_IS_MACOS
#include <CoreFoundation/CoreFoundation.h>
bool isNaturalScrollEnabled() {
    CFPropertyListRef value =
        CFPreferencesCopyAppValue(
            CFSTR("com.apple.swipescrolldirection"),
            kCFPreferencesAnyApplication
        );

    if (value && CFGetTypeID(value) == CFBooleanGetTypeID()) {
        bool result = CFBooleanGetValue((CFBooleanRef)value);
        CFRelease(value);
        return result;
    }

    if (value) CFRelease(value);
    return true;
}
#else
bool isNaturalScrollEnabled() {
    return false;
}
#endif

$on_mod(Loaded) {
    ScrollWheelEvent().listen([] (double x, double y) {
        #ifdef GEODE_IS_MACOS
        int naturalMult = isNaturalScrollEnabled() ? 1 : -1;
        float xMult = 1 * naturalMult;
        float yMult = 1 * naturalMult;
        #else
        float xMult = 1;
        float yMult = -1;
        #endif

        float multiplier = 12;

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

                scrollDelegate->scroll((x * multiplier) * xMult, (y * multiplier) * yMult);
            }
            break;
        }
    }).leak();

    KeyboardInputEvent().listen([] (KeyboardInputData& data) {
        bool repeat = data.action == KeyboardInputData::Action::Repeat;
        bool down = data.action == geode::KeyboardInputData::Action::Press || repeat;
        for (AdvancedScrollDelegate* scrollDelegate : ScrollDispatcher::get()->getDelegates()) {
            if (!alpha::utils::isPointInsideNode(typeinfo_cast<CCNode*>(scrollDelegate), getMousePos())) continue;

            if (CCKeyboardDispatcher::get()->getControlKeyPressed() || CCKeyboardDispatcher::get()->getCommandKeyPressed()) {
                if (down) {
                    if (data.key == enumKeyCodes::KEY_OEMEqual) {
                        scrollDelegate->zoom(25);
                    }
                    else if (data.key == enumKeyCodes::KEY_OEMMinus) {
                        scrollDelegate->zoom(-25);
                    }
                }
            }

            scrollDelegate->keyPress(data.key, down, repeat);
            break;
        }
    }).leak();
}