#include <Geode/Geode.hpp>
#include "API.hpp"
#include <geode.devtools/include/API.hpp>

using namespace geode::prelude;
using namespace alpha::prelude;

struct ScrollContent::Impl final {
    AdvancedScrollLayer* m_scrollLayer;
};

ScrollContent::ScrollContent() : m_impl(std::make_unique<Impl>()) {}
ScrollContent::~ScrollContent() = default;

ScrollContent* ScrollContent::create(AdvancedScrollLayer* scrollLayer) {
    auto ret = new ScrollContent();
    ret->init(scrollLayer);
    ret->autorelease();
    return ret;
}

bool ScrollContent::init(AdvancedScrollLayer* scrollLayer) {
    m_impl->m_scrollLayer = scrollLayer;
    setAnchorPoint({0.f, 1.f});
    return true;
}

AdvancedScrollLayer* ScrollContent::getScrollLayer() {
    return m_impl->m_scrollLayer;
}

void ScrollContent::setAnchorPoint(const cocos2d::CCPoint& anchor) {
    CCNode::setAnchorPoint(anchor);
}

void ScrollContent::ignoreAnchorPointForPosition(bool ignore) {
    CCNode::ignoreAnchorPointForPosition(ignore);
}

void ScrollContent::setPosition(const cocos2d::CCPoint& position) {
    CCNode::setPosition(position);
}

void ScrollContent::setPositionX(float x) {
    CCNode::setPositionX(x);
}

void ScrollContent::setPositionY(float y) {
    CCNode::setPositionY(y);
}