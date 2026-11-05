#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

bool TouchDelegate::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    auto mouseEvent = Touch::create(geode::MouseInputData::Button::Left);
    mouseEvent->setTouchInfo(touch->m_point);
    return clickBegan(mouseEvent);
}

void TouchDelegate::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    auto mouseEvent = Touch::create(geode::MouseInputData::Button::Left);
    mouseEvent->setTouchInfo(touch->m_point);
    clickMoved(mouseEvent);
}

void TouchDelegate::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    auto mouseEvent = Touch::create(geode::MouseInputData::Button::Left);
    mouseEvent->setTouchInfo(touch->m_point);
    clickEnded(mouseEvent);
}

void TouchDelegate::ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    auto mouseEvent = Touch::create(geode::MouseInputData::Button::Left);
    mouseEvent->setTouchInfo(touch->m_point);
    clickCancelled(mouseEvent);
}