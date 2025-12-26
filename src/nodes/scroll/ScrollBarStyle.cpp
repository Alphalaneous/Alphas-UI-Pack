#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace alpha::prelude;

ScrollBarStyle::ScrollBarStyle() {}

ScrollBarElement* ScrollBarStyle::getHandle() const {
    return m_handle ? m_handle() : nullptr;
}

ScrollBarElement* ScrollBarStyle::getTrack() const {
    return m_track ? m_track() : nullptr;
}

ScrollArrowElement* ScrollBarStyle::getUpArrow() const {
    return m_upArrow ? m_upArrow() : nullptr;
}

ScrollArrowElement* ScrollBarStyle::getDownArrow() const {
    return m_downArrow ? m_downArrow() : nullptr;
}

