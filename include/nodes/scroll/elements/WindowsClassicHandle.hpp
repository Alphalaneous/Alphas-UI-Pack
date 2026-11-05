#pragma once

#include <Geode/ui/NineSlice.hpp>
#include "../ScrollBarElement.hpp"
#include "../../../export.hpp"

namespace alpha::ui {

class ALPHAS_UI_PACK_DLL WindowsClassicHandle : public ScrollBarElement {
public:
    WindowsClassicHandle();
    ~WindowsClassicHandle();

    static WindowsClassicHandle* create();

    void setBackgroundColor(const cocos2d::ccColor4B& color);

    virtual void setContentSize(const cocos2d::CCSize& contentSize) override;
    virtual void onClick(const cocos2d::CCPoint& pos) override;
    virtual void onRelease(const cocos2d::CCPoint& pos) override;
    virtual void setScrollBar(AdvancedScrollBar* scrollBar) override;

protected:
    bool init() override;

    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}
