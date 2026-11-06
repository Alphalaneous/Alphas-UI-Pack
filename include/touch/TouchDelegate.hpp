#pragma once

#include <Geode/cocos/touch_dispatcher/CCTouchDelegateProtocol.h>
#include "Touch.hpp"

#include "../export.hpp"

namespace alpha::dispatcher {

/// Extend this class to handle touch interactions
class ALPHAS_UI_PACK_DLL TouchDelegate : public cocos2d::CCTouchDelegate {
public:
	virtual bool touchBegan(Touch* touch) { return false; }
	virtual void touchMoved(Touch* touch) {}
	virtual void touchEnded(Touch* touch) {}
	virtual void touchCancelled(Touch* touch) {}

	virtual bool hoverEntered(Hover* hover) { return false; }
	virtual void hoverExited(Hover* hover) {}
	virtual void hoverMoved(Hover* hover) {}

    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
};

}
