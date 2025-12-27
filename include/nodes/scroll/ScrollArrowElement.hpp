#pragma once

#include "AdvancedScrollBar.hpp"
#include "ScrollBarElement.hpp"
#include "../../export.hpp"

namespace alpha::ui {

/**
 * @brief Direction for a scroll arrow element.
 */
enum class ArrowDirection {
    /// Upward or leftward arrow.
    UP,

    /// Downward or rightward arrow.
    DOWN
};

/**
 * @brief Scroll bar element representing an arrow button.
 *
 * ScrollArrowElement handles click and release events and can repeatedly
 * scroll content while held. It is intended to be subclassed to provide
 * specific visual styles.
 */
class ALPHAS_UI_PACK_DLL ScrollArrowElement : public ScrollBarElement {
public:
    /**
     * @brief Constructs a ScrollArrowElement.
     */
    ScrollArrowElement();

    /**
     * @brief Destroys the ScrollArrowElement.
     */
    ~ScrollArrowElement();

    /**
     * @brief Returns the arrow direction of this element.
     *
     * @return ArrowDirection::UP or ArrowDirection::DOWN.
     */
    ArrowDirection getDirection();

    /**
     * @brief Called when the arrow is clicked.
     *
     * @param pos Click position relative to the element.
     */
    virtual void onClick(const cocos2d::CCPoint& pos) override;

    /**
     * @brief Called when a click is released on the arrow.
     *
     * @param pos Release position relative to the element.
     */
    virtual void onRelease(const cocos2d::CCPoint& pos) override;

private: 
    /// Handles continuous scrolling while the arrow is held.
    void handleScroll(float dt);

protected:
    /**
     * @brief Sets the direction of the arrow.
     *
     * Must be implemented by subclasses.
     *
     * @param direction ArrowDirection to set.
     */
    virtual void setDirection(ArrowDirection direction) = 0;

    /// Internal implementation details.
    struct Impl;
    std::unique_ptr<Impl> m_impl; 

    /// AdvancedScrollBar can access internal members.
    friend class AdvancedScrollBar;
};

}
