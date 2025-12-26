#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct RoundedScrollTrack::Impl final {
    cocos2d::ccColor4B m_backgroundColor = {0, 0, 0, 180};
    cocos2d::ccColor4B m_clickColor = {25, 25, 25, 180};
};

RoundedScrollTrack::RoundedScrollTrack() : m_impl(std::make_unique<Impl>()) {}
RoundedScrollTrack::~RoundedScrollTrack() = default;

RoundedScrollTrack* RoundedScrollTrack::create() {
    auto ret = new RoundedScrollTrack();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool RoundedScrollTrack::init() {
    if (!ScrollBarElement::init()) return false;

    setColor({0, 0, 0});
    setOpacity(180);

    return true;
}

void RoundedScrollTrack::setContentSize(const CCSize& contentSize) {
    ScrollBarElement::setContentSize(contentSize);
}

void RoundedScrollTrack::onClick(const cocos2d::CCPoint& pos) {
    setColor({m_impl->m_clickColor.r, m_impl->m_clickColor.g, m_impl->m_clickColor.b});
    setOpacity(m_impl->m_clickColor.a);
}

void RoundedScrollTrack::onRelease(const cocos2d::CCPoint& pos) {
    setColor({m_impl->m_backgroundColor.r, m_impl->m_backgroundColor.g, m_impl->m_backgroundColor.b});
    setOpacity(m_impl->m_backgroundColor.a);
}

void RoundedScrollTrack::draw() {
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

void RoundedScrollTrack::setBackgroundColor(const cocos2d::ccColor4B& color) {
    setColor({color.r, color.g, color.b});
    setOpacity(color.a);
    m_impl->m_backgroundColor = color;
}

void RoundedScrollTrack::setClickColor(const cocos2d::ccColor4B& color) {
    m_impl->m_clickColor = color;
}

cocos2d::ccColor4B RoundedScrollTrack::getBackgroundColor() {
    return m_impl->m_backgroundColor;
}

cocos2d::ccColor4B RoundedScrollTrack::getClickColor() {
    return m_impl->m_clickColor;
}