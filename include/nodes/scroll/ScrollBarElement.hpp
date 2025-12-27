#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>

#include "AdvancedScrollBar.hpp"
#include "../../export.hpp"

namespace alpha::ui {

/**
 * @brief Base class for scroll bar elements (handles, tracks, arrows).
 *
 * ScrollBarElement provides interaction handling for scroll bars,
 * including clicks, releases, and mouse hover events. It is intended
 * to be subclassed to provide specific visual elements.
 */
class ALPHAS_UI_PACK_DLL ScrollBarElement : public cocos2d::CCNodeRGBA {
public:
    /**
     * @brief Constructs a ScrollBarElement.
     */
    ScrollBarElement();

    /**
     * @brief Destroys the ScrollBarElement.
     */
    ~ScrollBarElement();

    /**
     * @brief Initializes the element.
     *
     * @return True if initialization succeeds.
     */
    bool init() override;

    /**
     * @brief Checks whether the element is currently being held (clicked).
     *
     * @return True if being held; false otherwise.
     */
    bool isHolding();

    /**
     * @brief Checks whether the mouse is hovering over the element.
     *
     * @return True if hovering; false otherwise.
     */
    bool isHovering();

    /**
     * @brief Sets the content size of the element.
     *
     * Must be implemented by subclasses.
     *
     * @param contentSize New content size.
     */
    virtual void setContentSize(const cocos2d::CCSize& contentSize) override = 0;

    /**
     * @brief Called when the element is clicked.
     *
     * @param pos Position of the click relative to the element.
     */
    virtual void onClick(const cocos2d::CCPoint& pos);

    /**
     * @brief Called when a click is released on the element.
     *
     * @param pos Position of the release relative to the element.
     */
    virtual void onRelease(const cocos2d::CCPoint& pos);

    /**
     * @brief Called when the mouse enters the element area.
     *
     * @param pos Mouse position relative to the element.
     */
    virtual void onMouseEnter(const cocos2d::CCPoint& pos);

    /**
     * @brief Called when the mouse exits the element area.
     *
     * @param pos Mouse position relative to the element.
     */
    virtual void onMouseExit(const cocos2d::CCPoint& pos);

    /**
     * @brief Called when the mouse moves over the element.
     *
     * @param pos Mouse position relative to the element.
     */
    virtual void onMouseMove(const cocos2d::CCPoint& pos);

protected:
    /**
     * @brief Returns the parent scroll bar that owns this element.
     *
     * @return Pointer to the associated AdvancedScrollBar.
     */
    AdvancedScrollBar* getScrollbar();

    /**
     * @brief Sets the parent scroll bar for this element.
     *
     * @param scrollBar Scroll bar to associate with this element.
     */
    virtual void setScrollBar(AdvancedScrollBar* scrollBar);

    /// Internal click/release/mouse event handlers.
    void onClickInternal(const cocos2d::CCPoint& pos);
    void onReleaseInternal(const cocos2d::CCPoint& pos);
    void onMouseEnterInternal(const cocos2d::CCPoint& pos);
    void onMouseExitInternal(const cocos2d::CCPoint& pos);
    void onMouseMoveInternal(const cocos2d::CCPoint& pos);

    /// Internal implementation details.
    struct Impl;
    std::unique_ptr<Impl> m_impl; 

    /// AdvancedScrollBar can access internal members.
    friend class AdvancedScrollBar;
};

}
