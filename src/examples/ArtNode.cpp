#include <Geode/Geode.hpp>
#include "API.hpp"
#include "Geode/loader/Log.hpp"
#include "Geode/utils/Keyboard.hpp"
#include "Utils.hpp"
#include "ArtNode.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

ArtNode* ArtNode::create(const CCSize& size) {
    auto ret = new ArtNode();
    if (ret->init(size)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool ArtNode::init(const CCSize& size) {
    if (!CCLayerColor::initWithColor({255, 255, 255, 255})) return false;

    setContentSize(size);
    ignoreAnchorPointForPosition(false);
    setAnchorPoint({0.5f, 0.5f});
    setZOrder(100);

    m_canvas = CCRenderTexture::create(size.width, size.height);
    m_canvas->setContentSize(getContentSize());
    m_canvas->ignoreAnchorPointForPosition(false);
    m_canvas->setAnchorPoint({0.f, 0.f});
    m_canvas->setPosition(getContentSize()/2);
    addChild(m_canvas);

    return true;
}

void ArtNode::drawRectBetweenPoints(const CCPoint& a, const CCPoint& b, float width, const ccColor4B& color) {
    CCPoint dir = b - a;
    float len = dir.getLength();
    if (len <= 0.001f) return;

    dir = dir * (1.0f / len);

    CCPoint n = {-dir.y, dir.x};
    CCPoint off = (n * width) / 2.f;

    CCPoint verts[4] = {
        a + off,
        a - off,
        b - off,
        b + off
    };

    ccDrawSolidPoly(verts, 4, ccc4FFromccc4B(color));
}

void ArtNode::addPoint(const CCPoint& p) {
    if (!m_hasLastPoint) {
        m_canvas->begin();

        CCPoint half = CCPoint{m_brushSize, m_brushSize} / 2.f;

        CCPoint verts[4] = {
            p - half,
            {p.x + half.x, p.y - half.y},
            p + half,
            {p.x - half.x, p.y + half.y}
        };

        ccDrawSolidPoly(verts, 4, ccc4FFromccc4B(m_clickColor));

        m_canvas->end();

        m_lastPoint = p;
        m_hasLastPoint = true;
        return;
    }

    const float step = m_brushSize / 2.f;
    CCPoint delta = p - m_lastPoint;
    float dist = delta.getLength();

    if (dist < step) return;

    CCPoint dir = delta * (1.0f / dist);

    m_canvas->begin();

    for (float d = 0; d < dist; d += step) {
        CCPoint a = m_lastPoint + (dir * d);
        CCPoint b = m_lastPoint + (dir * (d + step));
        drawRectBetweenPoints(a, b, m_brushSize, m_clickColor);
    }

    m_canvas->end();
    m_lastPoint = p;
}

bool ArtNode::touchBegan(Touch* touch) {
    if (!alpha::utils::canTouch(this, touch->getLocation())) return false;
    if (!alpha::utils::isPointInsideNode(this, touch->getLocation())) return false;
    m_holding = true;

    if (touch->getButton() == MouseInputData::Button::Left) {
        addPoint(convertToNodeSpace(touch->getLocation()));
    }
    if (touch->getButton() == MouseInputData::Button::Right) {
        log::info("right clicked!!");
    }

    switch (touch->getButton()) {
        case MouseInputData::Button::Button4:
            m_brushSize -= 1;
            if (m_brushSize < 1) m_brushSize = 1;
            break;
        case MouseInputData::Button::Button5:
            m_brushSize += 1;
            break;
        default:
            break;
    }

    return true;
};

void ArtNode::touchMoved(Touch* touch) {
    if (touch->getButton() == MouseInputData::Button::Left) {
        addPoint(convertToNodeSpace(touch->getLocation()));
    }
}

void ArtNode::touchEnded(Touch* touch) {
    m_holding = false;
    m_hasLastPoint = false;
}

bool ArtNode::hoverEntered(Hover* touch) { 
    //log::info("entered: {}", touch->getLocation());
    return true; 
}

void ArtNode::hoverExited(Hover* touch) {
    //log::info("exited: {}", touch->getLocation());
}

void ArtNode::hoverMoved(Hover* touch) {
    //log::info("moved: {}", touch->getLocation());
};

void ArtNode::onEnter() {
    CCLayerColor::onEnter();
    CCTouchDispatcher::get()->addTargetedDelegate(static_cast<TouchDelegate*>(this), 0, true);
}

void ArtNode::onExit() {
    CCLayerColor::onExit();
    CCTouchDispatcher::get()->removeDelegate(static_cast<TouchDelegate*>(this));

}