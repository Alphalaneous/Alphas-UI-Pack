#pragma once

#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include <Geode/cocos/sprite_nodes/CCSprite.h>

#include "../ScrollArrowElement.hpp"
#include "../../../export.hpp"
#include "ccTypes.h"

namespace alpha::ui {

class ALPHAS_UI_PACK_DLL WindowsClassicArrow : public ScrollArrowElement {
public:
    WindowsClassicArrow();
    ~WindowsClassicArrow();

    static WindowsClassicArrow* create();
    bool init() override;

    void setBackgroundColor(const cocos2d::ccColor4B& color);
    void setArrowColor(const cocos2d::ccColor4B& color);

    virtual void setContentSize(const cocos2d::CCSize& contentSize) override;
    virtual void onClick(const cocos2d::CCPoint& pos) override;
    virtual void onRelease(const cocos2d::CCPoint& pos) override;
    virtual void setDirection(ArrowDirection direction) override;
    virtual void setScrollBar(AdvancedScrollBar* scrollBar) override;
protected:
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}