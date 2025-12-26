#pragma once

#include "../ScrollBarElement.hpp"
#include "../../../export.hpp"

namespace alpha::ui {

class ALPHAS_UI_PACK_DLL RoundedScrollHandle : public ScrollBarElement {
public:
    RoundedScrollHandle();
    ~RoundedScrollHandle();

    static RoundedScrollHandle* create();
    bool init() override;

    void setBackgroundColor(const cocos2d::ccColor4B& color);
    void setHoverColor(const cocos2d::ccColor4B& color);
    void setClickColor(const cocos2d::ccColor4B& color);

    cocos2d::ccColor4B getBackgroundColor();
    cocos2d::ccColor4B getHoverColor();
    cocos2d::ccColor4B getClickColor();

    virtual void setContentSize(const cocos2d::CCSize& contentSize) override;
    virtual void onClick(const cocos2d::CCPoint& pos) override;
    virtual void onRelease(const cocos2d::CCPoint& pos) override;
    virtual void onMouseEnter(const cocos2d::CCPoint& pos) override;
    virtual void onMouseExit(const cocos2d::CCPoint& pos) override;
    virtual void draw() override;
protected:
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}