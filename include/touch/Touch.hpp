#pragma once

#include <Geode/cocos/touch_dispatcher/CCTouch.h>
#include "TouchDispatcher.hpp"

#include "../export.hpp"

namespace alpha::dispatcher {

class ALPHAS_UI_PACK_DLL Touch : public cocos2d::CCTouch {
public:
    Touch();
    ~Touch();

	static Touch* create(geode::MouseInputData::Button button);

	void setTouchInfo(const cocos2d::CCPoint& point);
    geode::MouseInputData::Button getButton();
    
protected:
	bool init(geode::MouseInputData::Button button);

    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}
