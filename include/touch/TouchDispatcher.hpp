#pragma once

#include <Geode/cocos/cocoa/CCObject.h>

#include "../export.hpp"

namespace alpha::dispatcher {

class TouchEvent;

enum class TouchType {
	CLICK_BEGAN,
	CLICK_MOVED,
	CLICK_ENDED
};

enum class MouseButton : unsigned int {
	LEFT = 0,
    TOUCH = 0,
	MIDDLE,
	RIGHT,
    BUTTON3,
    BUTTON4,
    HOVER
};

class ALPHAS_UI_PACK_DLL TouchDispatcher : public cocos2d::CCObject {
public:
    TouchDispatcher();
    ~TouchDispatcher();
    
    static TouchDispatcher* get();
    void queueSort();
protected:
	void init();
	void hovers(TouchEvent* touch);
    void clicks(TouchEvent* touch, TouchType type);
	void pollInput(const cocos2d::CCPoint& pos);
	void handleHover(const cocos2d::CCPoint& pos);
    void rearrangeHandlers();
	virtual void update(float dt) override;

    struct Impl;
    std::unique_ptr<Impl> m_impl; 

    friend class MyCCTouchDispatcher;
};

}