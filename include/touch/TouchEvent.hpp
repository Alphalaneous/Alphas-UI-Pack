#pragma once

#include <Geode/cocos/touch_dispatcher/CCTouch.h>
#include "TouchDispatcher.hpp"

#include "../export.hpp"

namespace alpha::dispatcher {

class ALPHAS_UI_PACK_DLL TouchEvent : public cocos2d::CCTouch {
public:
    TouchEvent();
    ~TouchEvent();

	static TouchEvent* create(MouseButton button);
	bool init(MouseButton button);
	void setTouchInfo(const cocos2d::CCPoint& point);
    MouseButton getButton();

protected:
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}