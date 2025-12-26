#pragma once

#include <Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h>

#include "../ScrollBarElement.hpp"
#include "../../../export.hpp"

namespace alpha::ui {

class ALPHAS_UI_PACK_DLL BasicScrollHandle : public ScrollBarElement {
public:
    BasicScrollHandle();
    ~BasicScrollHandle();

    static BasicScrollHandle* create();
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
protected:
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}