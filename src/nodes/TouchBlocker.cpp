#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct TouchBlocker::Impl final {
    CCNode* m_target;
    int m_touchPriority;
};

TouchBlocker::TouchBlocker() : m_impl(std::make_unique<Impl>()) {}
TouchBlocker::~TouchBlocker() = default;

TouchBlocker* TouchBlocker::create(CCNode* target) {
    auto ret = new TouchBlocker();
    ret->init(target);
    ret->autorelease();
    return ret;
}

bool TouchBlocker::init(CCNode* target) {
    m_impl->m_target = target;
    return true;
}

void TouchBlocker::onEnter() {
    CCTouchDispatcher::get()->addTargetedDelegate(this, m_impl->m_touchPriority, true);
}

void TouchBlocker::setTouchPriority(int prio) {
    m_impl->m_touchPriority = prio;
}

int TouchBlocker::getTouchPriority() {
    return m_impl->m_touchPriority;
}

void TouchBlocker::onExit() {
    CCTouchDispatcher::get()->removeDelegate(this);
}

bool TouchBlocker::clickBegan(TouchEvent* touch) {
    return alpha::utils::isTouchInsideNode(m_impl->m_target, touch->getLocation());
}

void TouchBlocker::setTarget(CCNode* node) {
    m_impl->m_target = node;
}