#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct BasicScrollArrow::Impl final {
    cocos2d::CCSprite* m_arrow;
    cocos2d::ccColor4B m_arrowColor = {255, 255, 255, 180};
    cocos2d::ccColor4B m_clickColor = {255, 255, 255, 220};
};

BasicScrollArrow::BasicScrollArrow() : m_impl(std::make_unique<Impl>()) {}
BasicScrollArrow::~BasicScrollArrow() = default;

BasicScrollArrow* BasicScrollArrow::create() {
    auto ret = new BasicScrollArrow();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool BasicScrollArrow::init() {
    if (!ScrollArrowElement::init()) return false;

    m_impl->m_arrow = CCSprite::createWithSpriteFrameName("colorSpike_01_color_001.png");

    setContentHeight(10.f);

    addChild(m_impl->m_arrow);

    m_impl->m_arrow->setColor({255, 255, 255});
    m_impl->m_arrow->setOpacity(180);

    return true;
}

void BasicScrollArrow::setContentSize(const CCSize& contentSize) {
    ScrollArrowElement::setContentSize(contentSize);
    m_impl->m_arrow->setPosition(getContentSize()/2);

    auto factorX = contentSize.width / (m_impl->m_arrow->getContentWidth() + 25);
    auto factorY = contentSize.height / (m_impl->m_arrow->getContentHeight() + 25);

    m_impl->m_arrow->setScaleX(factorX);
    m_impl->m_arrow->setScaleY(factorY);
}

void BasicScrollArrow::onClick(const cocos2d::CCPoint& pos) {
    ScrollArrowElement::onClick(pos);
    m_impl->m_arrow->setColor({m_impl->m_clickColor.r, m_impl->m_clickColor.g, m_impl->m_clickColor.b});
    m_impl->m_arrow->setOpacity(m_impl->m_clickColor.a);
}

void BasicScrollArrow::onRelease(const cocos2d::CCPoint& pos) {
    ScrollArrowElement::onRelease(pos);
    m_impl->m_arrow->setColor({m_impl->m_arrowColor.r, m_impl->m_arrowColor.g, m_impl->m_arrowColor.b});
    m_impl->m_arrow->setOpacity(m_impl->m_arrowColor.a);
}

void BasicScrollArrow::setDirection(ArrowDirection direction) {
    ScrollArrowElement::setDirection(direction);
    if (direction == ArrowDirection::DOWN) {
        m_impl->m_arrow->setFlipY(true);
    }
}

void BasicScrollArrow::setArrowColor(const cocos2d::ccColor4B& color) {
    m_impl->m_arrow->setColor({color.r, color.g, color.b});
    m_impl->m_arrow->setOpacity(color.a);
    m_impl->m_arrowColor = color;
}

void BasicScrollArrow::setClickColor(const cocos2d::ccColor4B& color) {
    m_impl->m_clickColor = color;
}

cocos2d::ccColor4B BasicScrollArrow::getArrowColor() {
    return m_impl->m_arrowColor;
}

cocos2d::ccColor4B BasicScrollArrow::getClickColor() {
    return m_impl->m_clickColor;
}