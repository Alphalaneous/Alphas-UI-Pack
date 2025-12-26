#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct WindowsClassicArrow::Impl final {
    cocos2d::extension::CCScale9Sprite* m_background;
    cocos2d::CCSprite* m_arrow;
    bool m_holding = false;
};

WindowsClassicArrow::WindowsClassicArrow() : m_impl(std::make_unique<Impl>()) {}
WindowsClassicArrow::~WindowsClassicArrow() = default;

WindowsClassicArrow* WindowsClassicArrow::create() {
    auto ret = new WindowsClassicArrow();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool WindowsClassicArrow::init() {
    if (!ScrollArrowElement::init()) return false;

    m_impl->m_arrow = CCSprite::createWithSpriteFrameName("colorSpike_01_color_001.png");
    m_impl->m_arrow->setColor({0, 0, 0});
    m_impl->m_arrow->setZOrder(1);

    m_impl->m_background = CCScale9Sprite::create("WindowsClassic.png"_spr);
    m_impl->m_background->setScale(0.5f);

    setContentHeight(10.f);

    addChild(m_impl->m_arrow);
    addChild(m_impl->m_background);

    return true;
}

void WindowsClassicArrow::setContentSize(const CCSize& contentSize) {
    ScrollArrowElement::setContentSize(contentSize);

    CCPoint offset = {0, 0};

    if (m_impl->m_holding) {
        if (getScrollbar()->getOrientation() == ScrollOrientation::HORIZONTAL) {
            offset = CCPoint{-0.5f, -0.5f};
        }
        else {
            offset = CCPoint{0.5f, -0.5f};
        }
    }

    m_impl->m_arrow->setPosition((getContentSize()/2) + offset);

    auto factorX = contentSize.width / (m_impl->m_arrow->getContentWidth() + 25);
    auto factorY = contentSize.height / (m_impl->m_arrow->getContentHeight() + 60);

    m_impl->m_arrow->setScaleX(factorX);
    m_impl->m_arrow->setScaleY(factorY);

    m_impl->m_background->setContentSize(contentSize * 2);
    m_impl->m_background->setPosition(getContentSize()/2);
}

void WindowsClassicArrow::onClick(const cocos2d::CCPoint& pos) {
    ScrollArrowElement::onClick(pos);
    m_impl->m_holding = true;
    m_impl->m_background->setRotation(180);
}

void WindowsClassicArrow::onRelease(const cocos2d::CCPoint& pos) {
    ScrollArrowElement::onRelease(pos);
    m_impl->m_holding = false;
    m_impl->m_background->setRotation(0);
}

void WindowsClassicArrow::setDirection(ArrowDirection direction) {
    ScrollArrowElement::setDirection(direction);
    if (direction == ArrowDirection::DOWN) {
        m_impl->m_arrow->setFlipY(true);
    }
}

void WindowsClassicArrow::setScrollBar(AdvancedScrollBar* scrollBar) {
    ScrollBarElement::setScrollBar(scrollBar);

    if (scrollBar->getOrientation() == ScrollOrientation::HORIZONTAL) {
        m_impl->m_background->setScaleX(-0.5);
    }
}

void WindowsClassicArrow::setBackgroundColor(const cocos2d::ccColor4B& color) {
    m_impl->m_background->setColor({color.r, color.g, color.b});
    m_impl->m_background->setOpacity(color.a);
}

void WindowsClassicArrow::setArrowColor(const cocos2d::ccColor4B& color) {
    m_impl->m_arrow->setColor({color.r, color.g, color.b});
    m_impl->m_arrow->setOpacity(color.a);
}