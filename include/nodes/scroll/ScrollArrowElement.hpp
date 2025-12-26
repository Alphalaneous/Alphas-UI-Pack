#pragma once

#include "AdvancedScrollBar.hpp"
#include "ScrollBarElement.hpp"
#include "../../export.hpp"

namespace alpha::ui {

enum class ArrowDirection {
    UP,
    DOWN
};

class ALPHAS_UI_PACK_DLL ScrollArrowElement : public ScrollBarElement {
public:
    ScrollArrowElement();
    ~ScrollArrowElement();

    ArrowDirection getDirection();
    virtual void onClick(const cocos2d::CCPoint& pos);
    virtual void onRelease(const cocos2d::CCPoint& pos);
private: 
    void handleScroll(float dt);
protected:
    virtual void setDirection(ArrowDirection direction) = 0;
    struct Impl;
    std::unique_ptr<Impl> m_impl; 

    friend class AdvancedScrollBar;
};

}