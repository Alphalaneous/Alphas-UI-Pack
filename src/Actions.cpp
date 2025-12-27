#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

CCMoveToX* CCMoveToX::create(float duration, float x) {
    CCMoveToX* ret = new CCMoveToX();
    if (ret->initWithDuration(duration, x)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CCMoveToX::initWithDuration(float duration, float x) {
    if (!CCActionInterval::initWithDuration(duration)) return false;
    m_endX = x;
    return true;
}

void CCMoveToX::startWithTarget(CCNode* target) {
    CCActionInterval::startWithTarget(target);
    m_startX = target->getPositionX();
    m_deltaX = m_endX - m_startX;
}

void CCMoveToX::update(float t) {
    if (m_pTarget) {
        m_pTarget->setPositionX(m_startX + m_deltaX * t);
    }
}

CCMoveToY* CCMoveToY::create(float duration, float y) {
    CCMoveToY* ret = new CCMoveToY();
    if (ret->initWithDuration(duration, y)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CCMoveToY::initWithDuration(float duration, float y) {
    if (!CCActionInterval::initWithDuration(duration)) return false;
    m_endY = y;
    return true;
}

void CCMoveToY::startWithTarget(CCNode* target) {
    CCActionInterval::startWithTarget(target);
    m_startY = target->getPositionY();
    m_deltaY = m_endY - m_startY;
}

void CCMoveToY::update(float t) {
    if (m_pTarget) {
        m_pTarget->setPositionY(m_startY + m_deltaY * t);
    }
}