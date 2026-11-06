#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

bool TouchDelegate::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    auto mouseEvent = Touch::create(geode::MouseInputData::Button::Left);
    mouseEvent->setTouchInfo(touch->getID(), touch->m_point.x, touch->m_point.y);
    mouseEvent->setOriginalTouch(touch);
    return touchBegan(mouseEvent);
}

void TouchDelegate::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    auto mouseEvent = Touch::create(geode::MouseInputData::Button::Left);
    mouseEvent->setTouchInfo(touch->getID(), touch->m_point.x, touch->m_point.y);
    mouseEvent->setOriginalTouch(touch);
    touchMoved(mouseEvent);
}

void TouchDelegate::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    auto mouseEvent = Touch::create(geode::MouseInputData::Button::Left);
    mouseEvent->setTouchInfo(touch->getID(), touch->m_point.x, touch->m_point.y);
    mouseEvent->setOriginalTouch(touch);
    touchEnded(mouseEvent);
}

void TouchDelegate::ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    auto mouseEvent = Touch::create(geode::MouseInputData::Button::Left);
    mouseEvent->setTouchInfo(touch->getID(), touch->m_point.x, touch->m_point.y);
    mouseEvent->setOriginalTouch(touch);
    touchCancelled(mouseEvent);
}