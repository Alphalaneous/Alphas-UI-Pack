#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct RoundedScrollHandle::Impl final {
    cocos2d::ccColor4B m_backgroundColor = {255, 255, 255, 150};
    cocos2d::ccColor4B m_hoverColor = {255, 255, 255, 205};
    cocos2d::ccColor4B m_clickColor = {255, 255, 255, 220};
};

RoundedScrollHandle::RoundedScrollHandle() : m_impl(std::make_unique<Impl>()) {}
RoundedScrollHandle::~RoundedScrollHandle() = default;

RoundedScrollHandle* RoundedScrollHandle::create() {
    auto ret = new RoundedScrollHandle();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool RoundedScrollHandle::init() {
    if (!ScrollBarElement::init()) return false;

    setColor({255, 255, 255});
    setOpacity(150);

    return true;
}

void RoundedScrollHandle::setContentSize(const CCSize& contentSize) {
    ScrollBarElement::setContentSize(contentSize);
}

void RoundedScrollHandle::onClick(const cocos2d::CCPoint& pos) {
    setColor({m_impl->m_clickColor.r, m_impl->m_clickColor.g, m_impl->m_clickColor.b});
    setOpacity(m_impl->m_clickColor.a);
}

void RoundedScrollHandle::onRelease(const cocos2d::CCPoint& pos) {
    setColor({m_impl->m_backgroundColor.r, m_impl->m_backgroundColor.g, m_impl->m_backgroundColor.b});
    setOpacity(m_impl->m_backgroundColor.a);
}

void RoundedScrollHandle::onMouseEnter(const cocos2d::CCPoint& pos) {
    setColor({m_impl->m_hoverColor.r, m_impl->m_hoverColor.g, m_impl->m_hoverColor.b});
    setOpacity(m_impl->m_hoverColor.a);
}

void RoundedScrollHandle::onMouseExit(const cocos2d::CCPoint& pos) {
    setColor({m_impl->m_backgroundColor.r, m_impl->m_backgroundColor.g, m_impl->m_backgroundColor.b});
    setOpacity(m_impl->m_backgroundColor.a);
}

void RoundedScrollHandle::draw() {
    CCNodeRGBA::draw();

    float w = getContentWidth();
    float h = getContentHeight();
    float v = std::max(std::min(w, h), 0.f);
    float r = v / 2; 
    int segments = std::max(32, (int)(r * 0.5f));

    ccColor3B c = getColor();
    GLubyte opacity = getOpacity();
    float alpha = opacity / 255.0f;
    ccColor4F colorF = {
        (c.r / 255.0f) * alpha,
        (c.g / 255.0f) * alpha,
        (c.b / 255.0f) * alpha,
        alpha
    };

    std::vector<CCPoint> vertices;

    auto bl = CCPoint{r, r};
    for (int i = 0; i <= segments; ++i) {
        float t = M_PI + (1.5f * M_PI - M_PI) * i / segments;
        vertices.push_back({bl.x + r * cosf(t), bl.y + r * sinf(t)});
    }

    auto br = CCPoint{w - r, r};
    for (int i = 0; i <= segments; ++i) {
        float t = 1.5f * M_PI + (2.0f * M_PI - 1.5f * M_PI) * i / segments;
        vertices.push_back({br.x + r * cosf(t), br.y + r * sinf(t)});
    }

    auto tr = CCPoint{w - r, h - r};
    for (int i = 0; i <= segments; ++i) {
        float t = 0.0f + (0.5f * M_PI - 0.0f) * i / segments;
        vertices.push_back({tr.x + r * cosf(t), tr.y + r * sinf(t)});
    }

    auto tl = CCPoint{r, h - r};
    for (int i = 0; i <= segments; ++i) {
        float t = 0.5f * M_PI + (M_PI - 0.5f * M_PI) * i / segments;
        vertices.push_back({tl.x + r * cosf(t), tl.y + r * sinf(t)});
    }

    ccDrawSolidPoly(vertices.data(), vertices.size(), colorF);
}

void RoundedScrollHandle::setBackgroundColor(const cocos2d::ccColor4B& color) {
    setColor({color.r, color.g, color.b});
    setOpacity(color.a);
    m_impl->m_backgroundColor = color;
}

void RoundedScrollHandle::setClickColor(const cocos2d::ccColor4B& color) {
    m_impl->m_clickColor = color;
}

void RoundedScrollHandle::setHoverColor(const cocos2d::ccColor4B& color) {
    m_impl->m_hoverColor = color;
}

cocos2d::ccColor4B RoundedScrollHandle::getBackgroundColor() {
    return m_impl->m_backgroundColor;
}

cocos2d::ccColor4B RoundedScrollHandle::getHoverColor() {
    return m_impl->m_hoverColor;
}

cocos2d::ccColor4B RoundedScrollHandle::getClickColor() {
    return m_impl->m_clickColor;
}