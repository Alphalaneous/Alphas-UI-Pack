#pragma once

#include <Geode/cocos/touch_dispatcher/CCTouchDelegateProtocol.h>
#include "TouchEvent.hpp"

#include "../export.hpp"

namespace alpha::dispatcher {

class ALPHAS_UI_PACK_DLL TouchDelegate : public cocos2d::CCTouchDelegate {
public:
	virtual bool clickBegan(TouchEvent* touch) { return false; }
	virtual void clickMoved(TouchEvent* touch) {}
	virtual void clickEnded(TouchEvent* touch) {}

	virtual bool mouseEntered(TouchEvent* touch) { return false; }
	virtual void mouseExited(TouchEvent* touch) {}
	virtual void mouseMoved(TouchEvent* touch) {}
	
    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
};

}