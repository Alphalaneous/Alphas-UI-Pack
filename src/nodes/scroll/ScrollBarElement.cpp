#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct ScrollBarElement::Impl final {
    AdvancedScrollBar* m_scrollBar;
    bool m_holding;
    bool m_hovering;
};

ScrollBarElement::ScrollBarElement() : m_impl(std::make_unique<Impl>()) {}
ScrollBarElement::~ScrollBarElement() = default;

bool ScrollBarElement::init() {
    if (!CCNodeRGBA::init()) return false;

    ignoreAnchorPointForPosition(false);

    return true;
}

void ScrollBarElement::setContentSize(const cocos2d::CCSize& contentSize) {
    CCNodeRGBA::setContentSize(contentSize);
}

void ScrollBarElement::setScrollBar(AdvancedScrollBar* scrollBar) {
    m_impl->m_scrollBar = scrollBar;
}

void ScrollBarElement::onClick(const cocos2d::CCPoint& pos) {}

void ScrollBarElement::onRelease(const cocos2d::CCPoint& pos) {}

void ScrollBarElement::onMouseEnter(const cocos2d::CCPoint& pos) {}

void ScrollBarElement::onMouseExit(const cocos2d::CCPoint& pos) {}

void ScrollBarElement::onMouseMove(const cocos2d::CCPoint& pos) {}

AdvancedScrollBar* ScrollBarElement::getScrollbar() {
    return m_impl->m_scrollBar;
}

bool ScrollBarElement::isHolding() {
    return m_impl->m_holding;
}

bool ScrollBarElement::isHovering() {
    return m_impl->m_hovering;
}

void ScrollBarElement::onClickInternal(const cocos2d::CCPoint& pos) {
    m_impl->m_holding = true;
    onClick(pos);
}

void ScrollBarElement::onReleaseInternal(const cocos2d::CCPoint& pos) {
    m_impl->m_holding = false;
    onRelease(pos);
    if (m_impl->m_hovering) onMouseEnter(pos);
}

void ScrollBarElement::onMouseEnterInternal(const cocos2d::CCPoint& pos) {
    m_impl->m_hovering = true;
    if (!m_impl->m_holding) onMouseEnter(pos);
}

void ScrollBarElement::onMouseExitInternal(const cocos2d::CCPoint& pos) {
    m_impl->m_hovering = false;
    if (!m_impl->m_holding) onMouseExit(pos);
}

void ScrollBarElement::onMouseMoveInternal(const cocos2d::CCPoint& pos) {
    onMouseMove(pos);
}