#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct BasicScrollTrack::Impl final {
    cocos2d::CCLayerColor* m_background;
    cocos2d::ccColor4B m_backgroundColor = {0, 0, 0, 180};
    cocos2d::ccColor4B m_clickColor = {25, 25, 25, 255};
};

BasicScrollTrack::BasicScrollTrack() : m_impl(std::make_unique<Impl>()) {}
BasicScrollTrack::~BasicScrollTrack() = default;

BasicScrollTrack* BasicScrollTrack::create() {
    auto ret = new BasicScrollTrack();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool BasicScrollTrack::init() {
    if (!ScrollBarElement::init()) return false;

    m_impl->m_background = CCLayerColor::create();
    m_impl->m_background->ignoreAnchorPointForPosition(false);
    m_impl->m_background->setAnchorPoint({0.f, 0.f});

    m_impl->m_background->setColor({0, 0, 0});
    m_impl->m_background->setOpacity(180);

    addChild(m_impl->m_background);

    return true;
}

void BasicScrollTrack::setContentSize(const CCSize& contentSize) {
    ScrollBarElement::setContentSize(contentSize);
    m_impl->m_background->setContentSize(contentSize);
}

void BasicScrollTrack::onClick(const cocos2d::CCPoint& pos) {
    m_impl->m_background->setColor({m_impl->m_clickColor.r, m_impl->m_clickColor.g, m_impl->m_clickColor.b});
    m_impl->m_background->setOpacity(m_impl->m_clickColor.a);
}

void BasicScrollTrack::onRelease(const cocos2d::CCPoint& pos) {
    m_impl->m_background->setColor({m_impl->m_backgroundColor.r, m_impl->m_backgroundColor.g, m_impl->m_backgroundColor.b});
    m_impl->m_background->setOpacity(m_impl->m_backgroundColor.a);
}

void BasicScrollTrack::setBackgroundColor(const cocos2d::ccColor4B& color) {
    m_impl->m_background->setColor({color.r, color.g, color.b});
    m_impl->m_background->setOpacity(color.a);
    m_impl->m_backgroundColor = color;
}

void BasicScrollTrack::setClickColor(const cocos2d::ccColor4B& color) {
    m_impl->m_clickColor = color;
}

cocos2d::ccColor4B BasicScrollTrack::getBackgroundColor() {
    return m_impl->m_backgroundColor;
}

cocos2d::ccColor4B BasicScrollTrack::getClickColor() {
    return m_impl->m_clickColor;
}