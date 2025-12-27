#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>

#include "AdvancedScrollBar.hpp"
#include "ScrollContent.hpp"
#include "AdvancedScrollDelegate.hpp"
#include "../../export.hpp"
#include "../../touch/TouchDelegate.hpp"

namespace alpha::ui {

/**
 * @brief Scrollable container node with advanced features.
 *
 * AdvancedScrollLayer provides a scrollable area with support for:
 * - Vertical and horizontal scrolling
 * - Dragging and inertia-based scrolling
 * - Zooming and zoom limits
 * - Scroll bars
 * - Content culling for performance
 * - Touch and keyboard input handling
 *
 * Inherits from CCNode, TouchDelegate, and AdvancedScrollDelegate.
 */
class ALPHAS_UI_PACK_DLL AdvancedScrollLayer : public cocos2d::CCNode, 
                                               public alpha::dispatcher::TouchDelegate, 
                                               public alpha::dispatcher::AdvancedScrollDelegate {
public:
    /// Type for culling callbacks, receives content node and scroll point.
    using CullingMethod = std::function<void(cocos2d::CCNode* content, const cocos2d::CCPoint& scroll)>;

    using cocos2d::CCNode::addChild;
    using cocos2d::CCNode::removeChild;

    /**
     * @brief Constructs a new AdvancedScrollLayer.
     */
    AdvancedScrollLayer();

    /**
     * @brief Destroys the AdvancedScrollLayer.
     */
    ~AdvancedScrollLayer();

    /**
     * @brief Creates a scroll layer with a viewport size and optional culling callback.
     *
     * @param size Size of the scrollable viewport.
     * @param cullingMethod Optional callback for offscreen content culling.
     * @return Newly created AdvancedScrollLayer instance.
     */
    static AdvancedScrollLayer* create(const cocos2d::CCSize& size, const CullingMethod& cullingMethod);
    static AdvancedScrollLayer* create(const cocos2d::CCSize& size, CullingMethod&& cullingMethod);
    static AdvancedScrollLayer* create(const cocos2d::CCSize& size);

    /**
     * @brief Registers development tools to visualize or debug scroll layers.
     */
    static void registerDevTools();

    /**
     * @brief Initializes the scroll layer with a size and culling method.
     *
     * @param size Viewport size.
     * @param cullingMethod Culling callback.
     * @return True if initialization succeeds.
     */
    bool init(const cocos2d::CCSize& size, CullingMethod cullingMethod);

    /**
     * @brief Returns the internal scrollable content layer.
     *
     * @return Pointer to ScrollContent node.
     */
    ScrollContent* getContentLayer();

    /**
     * @brief Sets horizontal scroll position.
     *
     * @param x Target X offset.
     * @param smooth If true, scroll is animated instead of instant.
     */
    void setScrollX(float x, bool smooth = false);

    /**
     * @brief Sets vertical scroll position.
     *
     * @param y Target Y offset.
     * @param smooth If true, scroll is animated instead of instant.
     */
    void setScrollY(float y, bool smooth = false);

    /**
     * @brief Returns current scroll offset as a CCPoint.
     */
    cocos2d::CCPoint getScrollPoint();

    /**
     * @brief Sets the inner content size for scrolling.
     *
     * @param size Content size, larger than viewport allows scrolling.
     */
    void setInnerContentSize(const cocos2d::CCSize& size);

    /**
     * @brief Applies a layout to the content layer.
     *
     * @param layout Layout object.
     * @param apply Whether to immediately apply it.
     * @param respectAnchor Whether to respect child anchor points.
     */
    void setLayout(geode::Layout* layout, bool apply = true, bool respectAnchor = true);

    /**
     * @brief Updates layout of all children, optionally updating their order.
     *
     * @param updateChildOrder If true, children may be reordered.
     */
    void updateLayout(bool updateChildOrder = true);

    /**
     * @brief Sets or updates the culling function used for performance optimization.
     */
    void setCullingMethod(const CullingMethod& method);
    void setCullingMethod(CullingMethod&& method);

    /// Enables or disables culling of offscreen content.
    void setCullingEnabled(bool value);
    bool isCullingEnabled();

    /// Enables or disables dragging the scroll layer with input.
    void setDraggingEnabled(bool value);
    bool isDraggingEnabled();

    /// Enable/disable vertical or horizontal scrolling.
    void setVeritcalScroll(bool value);
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

    /// Touch priority management.
    void setTouchPriority(int prio);
    int getTouchPriority();

    // CCNode overrides
    virtual void update(float dt) override;
    virtual void setContentSize(const cocos2d::CCSize& size) override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void visit() override;

    virtual void addChild(cocos2d::CCNode* child, int zOrder, int tag) override;
    virtual void removeChild(cocos2d::CCNode* child, bool cleanup) override;
    virtual cocos2d::CCArray* getChildren() override;
    virtual unsigned int getChildrenCount(void) const override;
    virtual void removeAllChildrenWithCleanup(bool cleanup) override;

    // TouchDelegate overrides
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent* event) override;

    // AdvancedScrollDelegate overrides
    virtual void scroll(float x, float y) override;
    virtual void zoom(float zoom) override;

    // Keyboard input
    virtual void keyPress(cocos2d::enumKeyCodes key, bool isKeyDown, bool isKeyRepeat) override;

private:
    /// Constrains scroll offsets within content bounds.
    void constrain(bool skipInertiaCheck = false);

    /// Updates inertia-based scrolling each frame.
    void updateInertia(float dt);

    /// Bounces scroll layer back when overshooting the bounds.
    void bounceBack(bool horizontal);

    /// Collects touch handlers recursively for child nodes.
    void collectHandlers(cocos2d::CCNode* node, std::vector<std::pair<cocos2d::CCTouchHandler*, int>>& out);

    /// Recursively cancels touches for a node and its children.
    void cancelTouchesRecursive(cocos2d::CCNode* node, cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

    /// Cancels touches for all children of this scroll layer.
    void cancelChildrenTouches(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

    /// Sets minimum container size based on content.
    void setMinContainerSize();

    /// Marks the container as dirty so layout/culling is updated.
    void setContainerDirty();

    /// Handles touch priority management internally.
    void handleTouchPrio();

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

    friend class ScrollContent;
    friend class AdvancedScrollBar;

protected:
    /// Internal implementation details.
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}
