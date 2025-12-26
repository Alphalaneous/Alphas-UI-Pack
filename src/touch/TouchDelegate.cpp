#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

bool TouchDelegate::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    auto mouseEvent = TouchEvent::create(MouseButton::TOUCH);
    if (auto node = typeinfo_cast<CCNode*>(this)) {
        if (auto scrollLayer = static_cast<CCNode*>(node->getUserObject("scroll-layer"_spr))) {
            bool insideScroll = alpha::utils::isTouchInsideNode(scrollLayer, touch->getLocation());
            bool insideNode = alpha::utils::isTouchInsideNode(node, touch->getLocation());

            if (!insideScroll || !insideNode) return false;
        }
    }
    
    mouseEvent->setTouchInfo(touch->getLocation());
    return clickBegan(mouseEvent);
}

void TouchDelegate::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    auto mouseEvent = TouchEvent::create(MouseButton::TOUCH);
    mouseEvent->setTouchInfo(touch->getLocation());
    clickMoved(mouseEvent);
}

void TouchDelegate::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    auto mouseEvent = TouchEvent::create(MouseButton::TOUCH);
    mouseEvent->setTouchInfo(touch->getLocation());
    clickEnded(mouseEvent);
}

void TouchDelegate::ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    auto mouseEvent = TouchEvent::create(MouseButton::TOUCH);
    mouseEvent->setTouchInfo(touch->getLocation());
    clickEnded(mouseEvent);
}