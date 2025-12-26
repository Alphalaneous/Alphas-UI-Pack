#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct BasicScrollHandle::Impl final {
    cocos2d::CCLayerColor* m_background;
    cocos2d::ccColor4B m_backgroundColor = {255, 255, 255, 180};
    cocos2d::ccColor4B m_hoverColor = {255, 255, 255, 205};
    cocos2d::ccColor4B m_clickColor = {255, 255, 255, 220};
};

BasicScrollHandle::BasicScrollHandle() : m_impl(std::make_unique<Impl>()) {}
BasicScrollHandle::~BasicScrollHandle() = default;

BasicScrollHandle* BasicScrollHandle::create() {
    auto ret = new BasicScrollHandle();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool BasicScrollHandle::init() {
    if (!ScrollBarElement::init()) return false;

    m_impl->m_background = CCLayerColor::create();
    m_impl->m_background->ignoreAnchorPointForPosition(false);
    m_impl->m_background->setAnchorPoint({0.f, 0.f});

    m_impl->m_background->setColor({255, 255, 255});
    m_impl->m_background->setOpacity(180);

    addChild(m_impl->m_background);

    return true;
}

void BasicScrollHandle::setContentSize(const CCSize& contentSize) {
    ScrollBarElement::setContentSize(contentSize);
    m_impl->m_background->setContentSize(contentSize);
}

void BasicScrollHandle::onClick(const cocos2d::CCPoint& pos) {
    m_impl->m_background->setColor({m_impl->m_clickColor.r, m_impl->m_clickColor.g, m_impl->m_clickColor.b});
    m_impl->m_background->setOpacity(m_impl->m_clickColor.a);
}

void BasicScrollHandle::onRelease(const cocos2d::CCPoint& pos) {
    m_impl->m_background->setColor({m_impl->m_backgroundColor.r, m_impl->m_backgroundColor.g, m_impl->m_backgroundColor.b});
    m_impl->m_background->setOpacity(m_impl->m_backgroundColor.a);
}

void BasicScrollHandle::onMouseEnter(const cocos2d::CCPoint& pos) {
    m_impl->m_background->setColor({m_impl->m_hoverColor.r, m_impl->m_hoverColor.g, m_impl->m_hoverColor.b});
    m_impl->m_background->setOpacity(m_impl->m_hoverColor.a);
}

void BasicScrollHandle::onMouseExit(const cocos2d::CCPoint& pos) {
    m_impl->m_background->setColor({m_impl->m_backgroundColor.r, m_impl->m_backgroundColor.g, m_impl->m_backgroundColor.b});
    m_impl->m_background->setOpacity(m_impl->m_backgroundColor.a);
}

void BasicScrollHandle::setBackgroundColor(const cocos2d::ccColor4B& color) {
    m_impl->m_background->setColor({color.r, color.g, color.b});
    m_impl->m_background->setOpacity(color.a);
    m_impl->m_backgroundColor = color;
}

void BasicScrollHandle::setHoverColor(const cocos2d::ccColor4B& color) {
    m_impl->m_hoverColor = color;
}

void BasicScrollHandle::setClickColor(const cocos2d::ccColor4B& color) {
    m_impl->m_clickColor = color;
}

cocos2d::ccColor4B BasicScrollHandle::getBackgroundColor() {
    return m_impl->m_backgroundColor;
}

cocos2d::ccColor4B BasicScrollHandle::getHoverColor() {
    return m_impl->m_hoverColor;
}

cocos2d::ccColor4B BasicScrollHandle::getClickColor() {
    return m_impl->m_clickColor;
}