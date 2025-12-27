#import <Cocoa/Cocoa.h>
#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

void TouchDispatcher::pollInput(const CCPoint& pos) {
    auto origStates = m_impl->m_states;

    m_impl->m_states[MouseButton::MIDDLE]   = CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, kCGMouseButtonCenter);
    m_impl->m_states[MouseButton::RIGHT]    = CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, kCGMouseButtonRight);
    m_impl->m_states[MouseButton::BUTTON3]  = CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, (CGMouseButton)3);
    m_impl->m_states[MouseButton::BUTTON4]  = CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, (CGMouseButton)4);

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