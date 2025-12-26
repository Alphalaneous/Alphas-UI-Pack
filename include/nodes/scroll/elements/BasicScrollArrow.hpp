#pragma once

#include <Geode/cocos/sprite_nodes/CCSprite.h>

#include "../ScrollArrowElement.hpp"
#include "../../../API.hpp"
#include "ccTypes.h"

namespace alpha::ui {

class ALPHAS_UI_PACK_DLL BasicScrollArrow : public ScrollArrowElement {
public:
    BasicScrollArrow();
    ~BasicScrollArrow();

    static BasicScrollArrow* create();
    bool init() override;

    void setArrowColor(const cocos2d::ccColor4B& color);
    void setClickColor(const cocos2d::ccColor4B& color);

    cocos2d::ccColor4B getArrowColor();
    cocos2d::ccColor4B getClickColor();

    virtual void setContentSize(const cocos2d::CCSize& contentSize) override;
    virtual void onClick(const cocos2d::CCPoint& pos) override;
    virtual void onRelease(const cocos2d::CCPoint& pos) override;
    virtual void setDirection(ArrowDirection direction) override;
protected:
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}