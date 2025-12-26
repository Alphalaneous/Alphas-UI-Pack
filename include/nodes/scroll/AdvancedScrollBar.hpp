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

enum class ScrollOrientation {
    VERTICAL,
    HORIZONTAL
};

class ALPHAS_UI_PACK_DLL AdvancedScrollBar : public cocos2d::CCNode, public alpha::dispatcher::TouchDelegate, public alpha::dispatcher::AdvancedScrollDelegate {
public:
    AdvancedScrollBar();
    ~AdvancedScrollBar();

    static AdvancedScrollBar* create(AdvancedScrollLayer* scrollLayer, ScrollOrientation orientation);
    
    static void registerDevTools();

    bool init(AdvancedScrollLayer* scrollLayer, ScrollOrientation orientation);

    void handleScroll(float y, bool smooth);

    void addToScroll(float amount);

    void lockToScrollLayer(bool lock);
    bool isLockedToScrollLayer();

    void setPadding(const Insets& padding);
    void setPadding(float left, float right, float top, float bottom);
    void setPaddingLeft(float left);
    void setPaddingRight(float right);
    void setPaddingTop(float top);
    void setPaddingBottom(float bottom);

    Insets getPadding();
    float getPaddingLeft();
    float getPaddingRight();
    float getPaddingTop();
    float getPaddingBottom();

    void setMargins(const Insets& margins);
    void setMargins(float left, float right, float top, float bottom);
    void setMarginLeft(float left);
    void setMarginRight(float right);
    void setMarginTop(float top);
    void setMarginBottom(float bottom);

    Insets getMargins();
    float getMarginLeft();
    float getMarginRight();
    float getMarginTop();
    float getMarginBottom();

    void setMinHandleHeight(float height);
    float getMinHandleHeight();

    void setStyle(const ScrollBarStyle& style);

    void setTrack(ScrollBarElement* track);
    void setHandle(ScrollBarElement* handle);

    ScrollBarElement* getTrack();
    ScrollBarElement* getHandle();

    void showArrowButtons(bool show);
    bool hasArrowButtons();

    void setUpArrow(ScrollArrowElement* upArrow);
    void setDownArrow(ScrollArrowElement* downArrow);
    
    ScrollArrowElement* getUpArrow();
    ScrollArrowElement* getDownArrow();

    ScrollOrientation getOrientation();

    void registerWithTouchDispatcher();
    void setTouchPriority(int prio);
    int getTouchPriority();

    virtual void update(float dt) override;
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent* event) override;
    virtual bool mouseEntered(alpha::dispatcher::TouchEvent* touch) override;
    virtual void mouseMoved(alpha::dispatcher::TouchEvent* touch) override;
    virtual void mouseExited(alpha::dispatcher::TouchEvent* touch) override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void scroll(float x, float y) override;
    virtual void keyPress(cocos2d::enumKeyCodes key, bool isKeyDown, bool isKeyRepeat) override;

protected:
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}