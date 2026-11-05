#pragma once

#include <Geode/cocos/touch_dispatcher/CCTouchDelegateProtocol.h>
#include "Touch.hpp"

#include "../export.hpp"

namespace alpha::dispatcher {

/// Extend this class to handle touch interactions
class ALPHAS_UI_PACK_DLL TouchDelegate : public cocos2d::CCTouchDelegate {
public:
	virtual bool clickBegan(Touch* touch) { return false; }
	virtual void clickMoved(Touch* touch) {}
	virtual void clickEnded(Touch* touch) {}
	virtual void clickCancelled(Touch* touch) {}

	virtual bool mouseEntered(Hover* hover) { return false; }
	virtual void mouseExited(Hover* hover) {}
	virtual void mouseMoved(Hover* hover) {}

    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
};

}
