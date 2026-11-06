#pragma once

#include <Geode/cocos/touch_dispatcher/CCTouch.h>
#include "TouchDispatcher.hpp"

#include "../export.hpp"

namespace alpha::dispatcher {

class ALPHAS_UI_PACK_DLL Hover : public cocos2d::CCTouch {
public:
	static Hover* create();
};

}
