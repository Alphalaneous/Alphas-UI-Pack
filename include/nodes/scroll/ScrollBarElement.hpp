#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>

#include "AdvancedScrollBar.hpp"
#include "../../export.hpp"

namespace alpha::ui {

class ALPHAS_UI_PACK_DLL ScrollBarElement : public cocos2d::CCNodeRGBA {
public:
    ScrollBarElement();
    ~ScrollBarElement();

    bool init() override;

    bool isHolding();
    bool isHovering();

    virtual void setContentSize(const cocos2d::CCSize& contentSize) override = 0;
    virtual void onClick(const cocos2d::CCPoint& pos);
    virtual void onRelease(const cocos2d::CCPoint& pos);
    virtual void onMouseEnter(const cocos2d::CCPoint& pos);
    virtual void onMouseExit(const cocos2d::CCPoint& pos);
    virtual void onMouseMove(const cocos2d::CCPoint& pos);
protected:
    AdvancedScrollBar* getScrollbar();
    virtual void setScrollBar(AdvancedScrollBar* scrollBar);
    void onClickInternal(const cocos2d::CCPoint& pos);
    void onReleaseInternal(const cocos2d::CCPoint& pos);
    void onMouseEnterInternal(const cocos2d::CCPoint& pos);
    void onMouseExitInternal(const cocos2d::CCPoint& pos);
    void onMouseMoveInternal(const cocos2d::CCPoint& pos);

    struct Impl;
    std::unique_ptr<Impl> m_impl; 

    friend class AdvancedScrollBar;
};

}