#pragma once

#include "../../export.hpp"

namespace alpha::dispatcher {

class ALPHAS_UI_PACK_DLL AdvancedScrollDelegate {
public:
    virtual void scroll(float x, float y) {};
    virtual void zoom(float zoom) {};
    virtual void keyPress(cocos2d::enumKeyCodes key, bool isKeyDown, bool isKeyRepeat) {};
};

}