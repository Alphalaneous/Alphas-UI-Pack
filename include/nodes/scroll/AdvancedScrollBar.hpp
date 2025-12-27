#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>
#include "AdvancedScrollDelegate.hpp"
#include "ScrollBarStyle.hpp"
#include "../../touch/TouchDelegate.hpp"
#include "../../touch/TouchEvent.hpp"
#include "../../export.hpp"

namespace alpha::ui {

class AdvancedScrollLayer;
class ScrollBarElement;
class ScrollArrowElement;

/**
 * @brief Orientation of the scroll bar.
 */
enum class ScrollOrientation {
    /// Vertical scroll bar
    VERTICAL,   

    /// Horizontal scroll bar
    HORIZONTAL  
};

/**
 * @brief Scroll bar for AdvancedScrollLayer.
 *
 * Handles touch/keyboard input, arrow buttons, padding/margins, styling, and can
 * be locked to a scroll layer.
 */
class ALPHAS_UI_PACK_DLL AdvancedScrollBar : public cocos2d::CCNode,
                                             public alpha::dispatcher::TouchDelegate,
                                             public alpha::dispatcher::AdvancedScrollDelegate {
public:
    /**
     * @brief Constructs a new AdvancedScrollBar.
     */
    AdvancedScrollBar();

    /**
     * @brief Destroys the AdvancedScrollBar.
     */
    ~AdvancedScrollBar();

    /**
     * @brief Creates a scroll bar for a scroll layer with given orientation.
     * @param scrollLayer Target scroll layer.
     * @param orientation Vertical or horizontal scroll bar.
     * @return Pointer to the created AdvancedScrollBar.
     */
    static AdvancedScrollBar* create(AdvancedScrollLayer* scrollLayer, ScrollOrientation orientation);

    /**
     * @brief Registers developer tools for debugging scroll bars.
     */
    static void registerDevTools();

    /**
     * @brief Initializes the scroll bar.
     * @param scrollLayer Target scroll layer.
     * @param orientation Scroll orientation.
     * @return True if initialization succeeded.
     */
    bool init(AdvancedScrollLayer* scrollLayer, ScrollOrientation orientation);

    /**
     * @brief Updates scroll bar to match the scroll layer's scroll position.
     * @param y Vertical scroll offset.
     * @param smooth Whether the update should be animated.
     */
    void handleScroll(float y, bool smooth);

    /**
     * @brief Adjusts scroll position by a relative amount.
     * @param amount Amount to add to current scroll.
     */
    void addToScroll(float amount);

    /// Locks or unlocks the scroll bar to its scroll layer.
    void lockToScrollLayer(bool lock);

    /// Returns true if the scroll bar is locked to the scroll layer.
    bool isLockedToScrollLayer();

    /// Sets padding around the scroll bar.
    void setPadding(const Insets& padding);
    void setPadding(float left, float right, float top, float bottom);
    void setPaddingLeft(float left);
    void setPaddingRight(float right);
    void setPaddingTop(float top);
    void setPaddingBottom(float bottom);

    /// Returns the current padding values.
    Insets getPadding();
    float getPaddingLeft();
    float getPaddingRight();
    float getPaddingTop();
    float getPaddingBottom();

    /// Sets margins around the scroll bar.
    void setMargins(const Insets& margins);
    void setMargins(float left, float right, float top, float bottom);
    void setMarginLeft(float left);
    void setMarginRight(float right);
    void setMarginTop(float top);
    void setMarginBottom(float bottom);

    /// Returns the current margin values.
    Insets getMargins();
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

    /// Registers this scroll bar with the touch dispatcher.
    void registerWithTouchDispatcher();

    /// Sets touch priority.
    void setTouchPriority(int prio);

    /// Returns touch priority.
    int getTouchPriority();

    // CCNode overrides
    virtual void update(float dt) override;
    virtual void onEnter() override;
    virtual void onExit() override;

    // TouchDelegate overrides
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

    virtual bool mouseEntered(alpha::dispatcher::TouchEvent* touch) override;
    virtual void mouseMoved(alpha::dispatcher::TouchEvent* touch) override;
    virtual void mouseExited(alpha::dispatcher::TouchEvent* touch) override;

    // AdvancedScrollDelegate overrides
    virtual void scroll(float x, float y) override;
    virtual void keyPress(cocos2d::enumKeyCodes key, bool isKeyDown, bool isKeyRepeat) override;

protected:
    /// Internal implementation details.
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}
