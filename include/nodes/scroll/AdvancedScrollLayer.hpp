#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/ui/NineSlice.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/cocos/robtop/touch_dispatcher/CCTouchClipper.h>
#include "ScrollContent.hpp"
#include "../../export.hpp"
#include "../../touch/TouchDelegate.hpp"
#include "nodes/scroll/AdvancedScrollBar.hpp"

namespace alpha::ui {

/**
 * Vertical and horizontal scrolling
 * Dragging and inertia-based scrolling
 * Zooming and zoom limits
 * Scroll bars
 * Content culling for performance
 * Touch and keyboard input handling
 */
class ALPHAS_UI_PACK_DLL AdvancedScrollLayer : public cocos2d::CCNode, 
                                               public alpha::dispatcher::TouchDelegate, 
                                               public cocos2d::CCMouseDelegate,
                                               public cocos2d::CCTouchClipper {
public:
    /// Type for culling callbacks, receives content node and scroll point.
    using CullingMethod = geode::Function<void(cocos2d::CCNode* content, const cocos2d::CCPoint& scroll)>;

    using cocos2d::CCNode::addChild;
    using cocos2d::CCNode::removeChild;

    class ScrollEvent : public geode::GlobalEvent<ScrollEvent, bool(float x, float y), AdvancedScrollLayer*> {
        using GlobalEvent::GlobalEvent;
    };
    
    AdvancedScrollLayer();
    ~AdvancedScrollLayer();

    /// Creates a scroll layer with a viewport size and optional culling callback.
    /// @param cullingMethod Optional callback for offscreen content culling.
    static AdvancedScrollLayer* create(const cocos2d::CCSize& size, CullingMethod cullingMethod);
    static AdvancedScrollLayer* create(const cocos2d::CCSize& size);

    static void registerDevTools();

    ScrollContent* getContentLayer();

    /// Sets horizontal scroll position.
    /// @param smooth If true, scroll is animated instead of instant.
    void setScrollX(float x, bool smooth = false);

    /// Sets vertical scroll position.
    /// @param smooth If true, scroll is animated instead of instant.
    void setScrollY(float y, bool smooth = false);


    /// Offsets horizontal scroll position.
    void offsetScrollX(float x);

    /// Offsets vertical scroll position.
    void offsetScrollY(float y);

    /// Offsets scroll position.
    void offsetScroll(float x, float y);

    void forceCull();
    void setInnerContentSize(const cocos2d::CCSize& size);

    cocos2d::CCPoint getScrollPoint();

    /// Sets or updates the culling function used for performance optimization.
    void setCullingMethod(CullingMethod method);

    /// Enables or disables culling of offscreen content.
    void setCullingEnabled(bool value);
    bool isCullingEnabled();

    /// Enables or disables dragging the scroll layer with input.
    void setDraggingEnabled(bool value);
    bool isDraggingEnabled();

    /// Enables or disables using keyboard scrolling on the scroll layer.
    void setKeyboardEnabled(bool value);
    bool isKeyboardEnabled();

    /// Enable/disable vertical or horizontal scrolling.
    void setVerticalScroll(bool value);
    void setHorizontalScroll(bool value);
    bool hasVerticalScroll();
    bool hasHorizontalScroll();

    /// Enable/disable scroll wheel input.
    void setVerticalScrollWheel(bool value);
    void setHorizontalScrollWheel(bool value);
    bool hasVerticalScrollWheel();
    bool hasHorizontalScrollWheel();

    /// Enable vertical scrolling when horizontal input occurs (optional behavior).
    void setVerticalScrollForHorizontal(bool value);
    bool hasVerticalScrollForHorizontal();

    /// Inertia and overshoot configuration.
    void setOvershoot(float value);
    float getOvershoot();
    void setFriction(float value);
    float getFriction();
    void setMinVelocity(float value);
    float getMinVelocity();

    /// Zoom configuration.
    void setMinZoom(float value);
    void setMaxZoom(float value);
    float getMinZoom();
    float getMaxZoom();
    void setZoom(float zoom);
    float getZoom();
    void allowZoom(bool allow);
    bool allowsZoom();

    /// Delta configuration.
    void setScrollDelta(float value);
    float getScrollDelta();

    /// Allows clicks to pass through empty areas.
    void allowEmptyClickThrough(bool allow);
    bool allowsEmptyClickThrough();

    /// Scrollable bounds and scroll percentages.
    float getHorizontalMax();
    float getVerticalMax();
    float getHorizontalScrollPercent();
    float getVerticalScrollPercent();
    float getVerticalPages();
    float getHorizontalPages();

    void zoom(float zoom);

    virtual void update(float dt) override;
    virtual void setContentSize(const cocos2d::CCSize& size) override;
    virtual void onEnter() override;
    virtual void onExit() override;

    virtual void addChild(cocos2d::CCNode* child, int zOrder, int tag) override;
    virtual void removeChild(cocos2d::CCNode* child, bool cleanup) override;
    virtual cocos2d::CCArray* getChildren() override;
    virtual unsigned int getChildrenCount(void) const override;
    virtual void removeAllChildrenWithCleanup(bool cleanup) override;

    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent* event) override;

    virtual bool scrollWheel(float y, float x) override;

    virtual bool touchInsideClip(const cocos2d::CCPoint& pos) override;

private:
    /// Constrains scroll offsets within content bounds.
    void constrain(bool skipInertiaCheck = false);

    /// Updates inertia-based scrolling each frame.
    void updateInertia(float dt);

    /// Bounces scroll layer back when overshooting the bounds.
    void bounceBack(bool horizontal);

    /// Recursively cancels touches for a node and its children.
    void cancelTouchesRecursive(cocos2d::CCNode* node, cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

    /// Cancels touches for all children of this scroll layer.
    void cancelChildrenTouches(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

    /// Sets minimum container size based on content.
    void setMinContainerSize();

    /// Marks the container as dirty so layout/culling is updated.
    void setContainerDirty();

    /// Performs culling of offscreen child nodes for performance.
    void cull();

    /// Adjust stencil width for masking operations.
    void offsetStencilWidth(float width);

    /// Adjust stencil height for masking operations.
    void offsetStencilHeight(float height);

    /// Calculates horizontal scroll delta from key input.
    geode::Result<float> getHorizontalScroll(cocos2d::enumKeyCodes key);

    /// Calculates vertical scroll delta from key input, optionally using horizontal scroll.
    geode::Result<float> getVerticalScroll(cocos2d::enumKeyCodes key, bool horizontal = false);

    void setSmoothScrollEnabled(bool enabled);
    bool isSmoothScrollEnabled();

    friend class ScrollContent;
    friend class AdvancedScrollBar;

protected:
    bool init(const cocos2d::CCSize& size, CullingMethod cullingMethod);

    void updateScrollDecay(float dt);

    void setVerticalScrollBar(AdvancedScrollBar* bar);
    void setHorizontalScrollBar(AdvancedScrollBar* bar);

    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}
