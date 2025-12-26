#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct WindowsClassicTrack::Impl final {
    cocos2d::CCLayerColor* m_background;
    ccColor4B m_backgroundColor = {220, 220, 220, 255};
    ccColor4B m_clickColor = {210, 210, 210, 255};
};

WindowsClassicTrack::WindowsClassicTrack() : m_impl(std::make_unique<Impl>()) {}
WindowsClassicTrack::~WindowsClassicTrack() = default;

WindowsClassicTrack* WindowsClassicTrack::create() {
    auto ret = new WindowsClassicTrack();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool WindowsClassicTrack::init() {
    if (!ScrollBarElement::init()) return false;

    m_impl->m_background = CCLayerColor::create();
    m_impl->m_background->ignoreAnchorPointForPosition(false);
    m_impl->m_background->setAnchorPoint({0.f, 0.f});

    m_impl->m_background->setColor({220, 220, 220});
    m_impl->m_background->setOpacity(255);

    addChild(m_impl->m_background);

    return true;
}

void WindowsClassicTrack::setContentSize(const CCSize& contentSize) {
    ScrollBarElement::setContentSize(contentSize);
    m_impl->m_background->setContentSize(contentSize);
}

void WindowsClassicTrack::onClick(const cocos2d::CCPoint& pos) {
    m_impl->m_background->setColor({m_impl->m_clickColor.r, m_impl->m_clickColor.g, m_impl->m_clickColor.b});
    m_impl->m_background->setOpacity(m_impl->m_clickColor.a);
}

void WindowsClassicTrack::onRelease(const cocos2d::CCPoint& pos) {
    m_impl->m_background->setColor({m_impl->m_backgroundColor.r, m_impl->m_backgroundColor.g, m_impl->m_backgroundColor.b});
    m_impl->m_background->setOpacity(m_impl->m_backgroundColor.a);
}

void WindowsClassicTrack::setBackgroundColor(const cocos2d::ccColor4B& color) {
    m_impl->m_background->setColor({color.r, color.g, color.b});
    m_impl->m_background->setOpacity(color.a);
    m_impl->m_backgroundColor = color;
}

void WindowsClassicTrack::setClickColor(const cocos2d::ccColor4B& color) {
    m_impl->m_clickColor = color;
}