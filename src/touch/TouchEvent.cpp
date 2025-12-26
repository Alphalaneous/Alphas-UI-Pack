#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct TouchEvent::Impl final {
	MouseButton m_button;
};

TouchEvent::TouchEvent() : m_impl(std::make_unique<Impl>()) {}
TouchEvent::~TouchEvent() = default;

TouchEvent* TouchEvent::create(MouseButton button) {
    auto ret = new TouchEvent();
    if (ret->init(button)){
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool TouchEvent::init(MouseButton button) {
    m_impl->m_button = button;
    return true;
}

MouseButton TouchEvent::getButton() {
    return m_impl->m_button;
}

void TouchEvent::setTouchInfo(const CCPoint& point) {
    auto winSize = CCDirector::get()->getWinSize();

    m_prevPoint = m_point;
    m_point = CCPoint{point.x, winSize.height - point.y};
    if (!m_startPointCaptured) {
        m_startPoint = m_point;
        m_startPointCaptured = true;
    }
}