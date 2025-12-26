#pragma once

#include <Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h>

#include "../ScrollBarElement.hpp"
#include "../../../export.hpp"

namespace alpha::ui {

class ALPHAS_UI_PACK_DLL WindowsClassicTrack : public ScrollBarElement {
public:
    WindowsClassicTrack();
    ~WindowsClassicTrack();

    static WindowsClassicTrack* create();
    bool init() override;

    void setBackgroundColor(const cocos2d::ccColor4B& color);
    void setClickColor(const cocos2d::ccColor4B& color);

    virtual void setContentSize(const cocos2d::CCSize& contentSize) override;
    virtual void onClick(const cocos2d::CCPoint& pos) override;
    virtual void onRelease(const cocos2d::CCPoint& pos) override;
protected:
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}