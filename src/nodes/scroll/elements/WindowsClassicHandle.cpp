#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct WindowsClassicHandle::Impl final {
    cocos2d::extension::CCScale9Sprite* m_background;
};

WindowsClassicHandle::WindowsClassicHandle() : m_impl(std::make_unique<Impl>()) {}
WindowsClassicHandle::~WindowsClassicHandle() = default;

WindowsClassicHandle* WindowsClassicHandle::create() {
    auto ret = new WindowsClassicHandle();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool WindowsClassicHandle::init() {
    if (!ScrollBarElement::init()) return false;

    m_impl->m_background = CCScale9Sprite::create("WindowsClassic.png"_spr);
    m_impl->m_background->setScale(0.5f);

    addChild(m_impl->m_background);

    return true;
}

void WindowsClassicHandle::setContentSize(const CCSize& contentSize) {
    ScrollBarElement::setContentSize(contentSize);
    m_impl->m_background->setContentSize(contentSize * 2);
    m_impl->m_background->setPosition(getContentSize()/2);
}

void WindowsClassicHandle::onClick(const cocos2d::CCPoint& pos) {
    m_impl->m_background->setRotation(180);
}

void WindowsClassicHandle::onRelease(const cocos2d::CCPoint& pos) {
    m_impl->m_background->setRotation(0);
}

void WindowsClassicHandle::setScrollBar(AdvancedScrollBar* scrollBar) {
    ScrollBarElement::setScrollBar(scrollBar);

    if (scrollBar->getOrientation() == ScrollOrientation::HORIZONTAL) {
        m_impl->m_background->setScaleX(-0.5);
    }
}

void WindowsClassicHandle::setBackgroundColor(const cocos2d::ccColor4B& color) {
    m_impl->m_background->setColor({color.r, color.g, color.b});
    m_impl->m_background->setOpacity(color.a);
}