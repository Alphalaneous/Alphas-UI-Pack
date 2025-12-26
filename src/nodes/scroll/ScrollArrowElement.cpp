#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct ScrollArrowElement::Impl final {
    ArrowDirection m_direction;
};

ScrollArrowElement::ScrollArrowElement() : m_impl(std::make_unique<Impl>()) {}
ScrollArrowElement::~ScrollArrowElement() = default;

void ScrollArrowElement::setDirection(ArrowDirection direction) {
    m_impl->m_direction = direction;
}

ArrowDirection ScrollArrowElement::getDirection() {
    return m_impl->m_direction;
}

void ScrollArrowElement::handleScroll(float dt) {
    if (m_impl->m_direction == ArrowDirection::UP) {
        getScrollbar()->addToScroll(-20);
    }
    else {
        getScrollbar()->addToScroll(20);
    }
}

void ScrollArrowElement::onClick(const cocos2d::CCPoint& pos) {
    ScrollBarElement::onClick(pos);
    handleScroll(0);
    schedule(schedule_selector(ScrollArrowElement::handleScroll), 50.f/1000.f, kCCRepeatForever, 500.f/1000.f);
}

void ScrollArrowElement::onRelease(const cocos2d::CCPoint& pos) {
    ScrollBarElement::onRelease(pos);
    unschedule(schedule_selector(ScrollArrowElement::handleScroll));
}