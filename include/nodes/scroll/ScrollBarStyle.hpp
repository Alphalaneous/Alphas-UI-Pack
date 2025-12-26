#pragma once

#include <functional>

#include "../../export.hpp"

namespace alpha::ui {

struct ALPHAS_UI_PACK_DLL Insets {
    float left, right, top, bottom;
}; 

class ScrollBarElement;
class ScrollArrowElement;

struct ALPHAS_UI_PACK_DLL ScrollBarStyle {
    using BarElement = std::function<ScrollBarElement*()>;
    using ArrowElement = std::function<ScrollArrowElement*()>;

    BarElement m_handle;
    BarElement m_track;

    ArrowElement m_upArrow;
    ArrowElement m_downArrow;

    Insets m_padding;
    Insets m_margins;

    float m_scrollBarWidth;
    float m_arrowButtonHeight;
    float m_distanceFromScrollLayer;

    bool m_showArrowButtons;

    ScrollBarStyle();

    ScrollBarElement* getHandle() const;
    ScrollBarElement* getTrack() const;
    ScrollArrowElement* getUpArrow() const;
    ScrollArrowElement* getDownArrow() const;
};

}