#pragma once

#include <Geode/cocos/cocoa/CCObject.h>

#include "../export.hpp"

namespace alpha::dispatcher {

class Touch;
class Hover;

enum class TouchType {
    ClickBegan,
    ClickMoved,
    ClickEnded,
    ClickCancelled
};

class ALPHAS_UI_PACK_DLL TouchDispatcher : public cocos2d::CCNode {
public:
    TouchDispatcher();
    ~TouchDispatcher();

    static TouchDispatcher* get();

protected:
    void hovers(Hover* touch);
    void clicks(Touch* touch, TouchType type);
    void checkTouch(float dt);
    void cleanupHandlers();
    
    struct Impl;
    std::unique_ptr<Impl> m_impl; 

    friend class MyCCTouchDispatcher;
};

}
