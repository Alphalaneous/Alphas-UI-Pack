#pragma once

#include "../../export.hpp"
#include <Geode/ui/Layout.hpp>
#include <Geode/utils/function.hpp>

namespace alpha::ui {

class ScrollBarElement;
class ScrollArrowElement;

struct ALPHAS_UI_PACK_DLL ScrollBarStyle {
    using BarElement = geode::Function<ScrollBarElement*() const>;
    using ArrowElement = geode::Function<ScrollArrowElement*() const>;

    /// Function that creates the scroll bar handle element.
    BarElement m_handle;

    /// Function that creates the scroll bar track element.
    BarElement m_track;

    /// Function that creates the up/left arrow element.
    ArrowElement m_upArrow;

    /// Function that creates the down/right arrow element.
    ArrowElement m_downArrow;

    /// Inner spacing for the scroll bar elements.
    geode::Padding m_padding;

    /// Outer spacing (margins) around the scroll bar.
    geode::Padding m_margins;

    /// Width of the scroll bar.
    float m_scrollBarWidth;

    /// Height of the arrow buttons.
    float m_arrowButtonHeight;

    /// Distance of the scroll bar from the associated scroll layer.
    float m_distanceFromScrollLayer;

    /// Whether the arrow buttons are displayed.
    bool m_showArrowButtons;

    ScrollBarStyle();

    ScrollBarElement* getHandle() const;
    ScrollBarElement* getTrack() const;
    ScrollArrowElement* getUpArrow() const;
    ScrollArrowElement* getDownArrow() const;
};

}
