#pragma once

#include <Geode/cocos/actions/CCActionInterval.h>

#include "export.hpp"

namespace alpha::ui {

class ALPHAS_UI_PACK_DLL CCMoveToX : public cocos2d::CCActionInterval {
public:
    static CCMoveToX* create(float duration, float x);
    bool initWithDuration(float duration, float x);
    virtual void startWithTarget(cocos2d::CCNode* target) override;
    virtual void update(float t) override;
private:
    float m_startX;
    float m_endX;
    float m_deltaX;
};

class ALPHAS_UI_PACK_DLL CCMoveToY : public cocos2d::CCActionInterval {
public:
    static CCMoveToY* create(float duration, float y);
    bool initWithDuration(float duration, float y);
    virtual void startWithTarget(cocos2d::CCNode* target) override;
    virtual void update(float t) override;
private:
    float m_startY;
    float m_endY;
    float m_deltaY;
};

}