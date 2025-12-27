#pragma once

#include <functional>

#include "../../export.hpp"

namespace alpha::ui {

/**
 * @brief Represents spacing around or inside UI elements.
 */
struct ALPHAS_UI_PACK_DLL Insets {
    /// Space on the left side.
    float left;

    /// Space on the right side.
    float right;

    /// Space at the top.
    float top;

    /// Space at the bottom.
    float bottom;
}; 

class ScrollBarElement;
class ScrollArrowElement;

/**
 * @brief Base style for scroll bars.
 *
 * Defines the appearance, layout, and behavior of a scroll bar.
 * Can be extended or used with different UI themes.
 */
struct ALPHAS_UI_PACK_DLL ScrollBarStyle {
    using BarElement = std::function<ScrollBarElement*()>;
    using ArrowElement = std::function<ScrollArrowElement*()>;

    /// Function that creates the scroll bar handle element.
    BarElement m_handle;

    /// Function that creates the scroll bar track element.
    BarElement m_track;

    /// Function that creates the up/left arrow element.
    ArrowElement m_upArrow;

    /// Function that creates the down/right arrow element.
    ArrowElement m_downArrow;

    /// Inner spacing for the scroll bar elements.
    Insets m_padding;

    /// Outer spacing (margins) around the scroll bar.
    Insets m_margins;

    /// Width of the scroll bar.
    float m_scrollBarWidth;

    /// Height of the arrow buttons.
    float m_arrowButtonHeight;

    /// Distance of the scroll bar from the associated scroll layer.
    float m_distanceFromScrollLayer;

    /// Whether the arrow buttons are displayed.
    bool m_showArrowButtons;

    /**
     * @brief Constructs a default ScrollBarStyle.
     *
     * Initializes default values for all members.
     */
    ScrollBarStyle();

    /**
     * @brief Returns a new scroll bar handle element.
     *
     * @return Pointer to a ScrollBarElement representing the handle.
     */
    ScrollBarElement* getHandle() const;

    /**
     * @brief Returns a new scroll bar track element.
     *
     * @return Pointer to a ScrollBarElement representing the track.
     */
    ScrollBarElement* getTrack() const;

    /**
     * @brief Returns a new up/left arrow element.
     *
     * @return Pointer to a ScrollArrowElement for the up arrow.
     */
    ScrollArrowElement* getUpArrow() const;

    /**
     * @brief Returns a new down/right arrow element.
     *
     * @return Pointer to a ScrollArrowElement for the down arrow.
     */
    ScrollArrowElement* getDownArrow() const;
};

}
