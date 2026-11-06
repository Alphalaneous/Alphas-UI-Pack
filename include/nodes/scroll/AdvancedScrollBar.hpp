#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/cocos/robtop/mouse_dispatcher/CCMouseDelegate.h>
#include "ScrollBarStyle.hpp"
#include "../../touch/TouchDelegate.hpp"
#include "../../touch/Touch.hpp"
#include "../../export.hpp"

namespace alpha::ui {

class AdvancedScrollLayer;
class ScrollBarElement;
class ScrollArrowElement;

enum class ScrollOrientation {
    Vertical,   
    Horizontal  
};

class ALPHAS_UI_PACK_DLL AdvancedScrollBar : public cocos2d::CCNode,
                                             public alpha::dispatcher::TouchDelegate,
                                             public cocos2d::CCMouseDelegate {
public:
    AdvancedScrollBar();
    ~AdvancedScrollBar();

    static AdvancedScrollBar* create(AdvancedScrollLayer* scrollLayer, ScrollOrientation orientation);
    static void registerDevTools();

    void handleScroll(float y, bool smooth);

    /// Adjusts scroll position by a relative amount.
    void addToScroll(float amount);

    /// Locks or unlocks the scroll bar to its scroll layer.
    void lockToScrollLayer(bool lock);

    /// Returns true if the scroll bar is locked to the scroll layer.
    bool isLockedToScrollLayer();

    /// Sets padding around the scroll bar.
    void setPadding(const geode::Padding& padding);
    void setPadding(float left, float right, float top, float bottom);
    void setPaddingLeft(float left);
    void setPaddingRight(float right);
    void setPaddingTop(float top);
    void setPaddingBottom(float bottom);

    /// Returns the current padding values.
    geode::Padding getPadding();
    float getPaddingLeft();
    float getPaddingRight();
    float getPaddingTop();
    float getPaddingBottom();

    /// Sets margins around the scroll bar.
    void setMargins(const geode::Padding& margins);
    void setMargins(float left, float right, float top, float bottom);
    void setMarginLeft(float left);
    void setMarginRight(float right);
    void setMarginTop(float top);
    void setMarginBottom(float bottom);

    /// Returns the current margin values.
    geode::Padding getMargins();
    float getMarginLeft();
    float getMarginRight();
    float getMarginTop();
    float getMarginBottom();
    
    /// Sets minimum handle height.
    void setMinHandleHeight(float height);

    /// Returns minimum handle height.
    float getMinHandleHeight();

    /// Sets height of arrow buttons.
    void setArrowButtonHeight(float height);

    /// Returns arrow button height.
    float getArrowButtonHeight();

    /// Sets scroll bar style.
    void setStyle(const ScrollBarStyle& style);

    /// Sets or replaces the track element.
    void setTrack(ScrollBarElement* track);

    /// Sets or replaces the handle element.
    void setHandle(ScrollBarElement* handle);

    /// Returns the track element.
    ScrollBarElement* getTrack();

    /// Returns the handle element.
    ScrollBarElement* getHandle();

    /// Shows or hides arrow buttons.
    void showArrowButtons(bool show);

    /// Returns true if arrow buttons are visible.
    bool hasArrowButtons();

    /// Sets the up arrow element.
    void setUpArrow(ScrollArrowElement* upArrow);

    /// Sets the down arrow element.
    void setDownArrow(ScrollArrowElement* downArrow);

    /// Returns the up arrow element.
    ScrollArrowElement* getUpArrow();

    /// Returns the down arrow element.
    ScrollArrowElement* getDownArrow();

    /// Returns scroll bar orientation.
    ScrollOrientation getOrientation();

    void registerWithTouchDispatcher();

    virtual void update(float dt) override;
    virtual void onEnter() override;
    virtual void onExit() override;

    virtual bool touchBegan(alpha::dispatcher::Touch* touch) override;
    virtual void touchMoved(alpha::dispatcher::Touch* touch) override;
    virtual void touchEnded(alpha::dispatcher::Touch* touch) override;
    virtual void touchCancelled(alpha::dispatcher::Touch* touch) override;

    virtual bool hoverEntered(alpha::dispatcher::Hover* hover) override;
    virtual void hoverMoved(alpha::dispatcher::Hover* hover) override;
    virtual void hoverExited(alpha::dispatcher::Hover* hover) override;

    virtual bool scrollWheel(float y, float x) override;

protected:
    bool init(AdvancedScrollLayer* scrollLayer, ScrollOrientation orientation);

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}
