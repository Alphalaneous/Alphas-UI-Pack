#include "API.hpp"

using namespace alpha::prelude;

BasicScrollStyle::BasicScrollStyle() {
    m_padding = {2, 2, 2, 2};
    m_margins = {2, 2, 2, 2};
    m_handle = [] { return BasicScrollHandle::create(); };
    m_track = [] { return BasicScrollTrack::create(); };
    m_upArrow = [] { return BasicScrollArrow::create(); };
    m_downArrow = [] { return BasicScrollArrow::create(); };
    m_scrollBarWidth = 14.f;
    m_arrowButtonHeight = 10.f;
    m_distanceFromScrollLayer = 5.f;
    m_showArrowButtons = false;
}

RoundedScrollStyle::RoundedScrollStyle() {
    m_padding = {2, 2, 2, 2};
    m_margins = {2, 2, 2, 2};
    m_handle = [] { return RoundedScrollHandle::create(); };
    m_track = [] { return RoundedScrollTrack::create(); };
    m_upArrow = [] { return BasicScrollArrow::create(); };
    m_downArrow = [] { return BasicScrollArrow::create(); };
    m_scrollBarWidth = 12.f;
    m_arrowButtonHeight = 10.f;
    m_distanceFromScrollLayer = 5.f;
    m_showArrowButtons = false;
}

WindowsClassicScrollStyle::WindowsClassicScrollStyle() {
    m_padding = {0, 0, 0, 0};
    m_margins = {0, 0, 0, 0};
    m_handle = [] { return WindowsClassicHandle::create(); };
    m_track = [] { return WindowsClassicTrack::create(); };
    m_upArrow = [] { return WindowsClassicArrow::create(); };
    m_downArrow = [] { return WindowsClassicArrow::create(); };
    m_scrollBarWidth = 10.f;
    m_arrowButtonHeight = 10.f;
    m_distanceFromScrollLayer = 0.f;
    m_showArrowButtons = true;
}