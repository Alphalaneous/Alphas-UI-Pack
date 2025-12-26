#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>
#include "../touch/TouchDelegate.hpp"

#include "../export.hpp"

namespace alpha::ui {

class AdvancedScrollLayer;

class ALPHAS_UI_PACK_DLL TouchBlocker : public cocos2d::CCNode, public alpha::dispatcher::TouchDelegate {
public:
    TouchBlocker();
    ~TouchBlocker();

    static TouchBlocker* create(cocos2d::CCNode* target);
    bool init(cocos2d::CCNode* target);

    void setTouchPriority(int prio);
    int getTouchPriority();
    void setTarget(cocos2d::CCNode* node);
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool clickBegan(alpha::dispatcher::TouchEvent* touch) override;
protected:
    struct Impl;
    std::unique_ptr<Impl> m_impl; 

    friend class AdvancedScrollLayer;
};

}