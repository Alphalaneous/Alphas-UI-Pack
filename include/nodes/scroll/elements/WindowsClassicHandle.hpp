#pragma once

#include <GUI/CCControlExtension/CCScale9Sprite.h>

#include "../ScrollBarElement.hpp"
#include "../../../export.hpp"

namespace alpha::ui {

class ALPHAS_UI_PACK_DLL WindowsClassicHandle : public ScrollBarElement {
public:
    WindowsClassicHandle();
    ~WindowsClassicHandle();

    static WindowsClassicHandle* create();
    bool init() override;

    void setBackgroundColor(const cocos2d::ccColor4B& color);

    virtual void setContentSize(const cocos2d::CCSize& contentSize) override;
    virtual void onClick(const cocos2d::CCPoint& pos) override;
    virtual void onRelease(const cocos2d::CCPoint& pos) override;
    virtual void setScrollBar(AdvancedScrollBar* scrollBar) override;
protected:
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}