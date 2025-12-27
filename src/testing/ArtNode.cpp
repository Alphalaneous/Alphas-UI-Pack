#include <Geode/Geode.hpp>
#include "API.hpp"
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

bool ArtNode::clickBegan(TouchEvent* touch) {
    if (!alpha::utils::isTouchInsideNode(this, touch->getLocation())) return false;
    m_holding = true;

    if (touch->getButton() == MouseButton::LEFT) {
        addPoint(convertToNodeSpace(touch->getLocation()));
    }

    switch (touch->getButton()) {
        case MouseButton::BUTTON3:
            m_brushSize -= 1;
            if (m_brushSize < 1) m_brushSize = 1;
            break;
        case MouseButton::BUTTON4:
            m_brushSize += 1;
            break;
        default:
            break;
    }

    return true;
};

void ArtNode::clickMoved(TouchEvent* touch) {
    if (touch->getButton() == MouseButton::LEFT) {
        addPoint(convertToNodeSpace(touch->getLocation()));
    }
}

void ArtNode::clickEnded(TouchEvent* touch) {
    m_holding = false;
    m_hasLastPoint = false;
}

bool ArtNode::mouseEntered(TouchEvent* touch) { 
    return true; 
}

void ArtNode::mouseExited(TouchEvent* touch) {
    CursorManager::get()->setCursor(Cursor::ARROW);
}

void ArtNode::mouseMoved(TouchEvent* touch) {
    CCPoint local = convertToNodeSpace(touch->getLocation());
    CursorManager::get()->setCursor(Cursor::CROSS);
};

void ArtNode::onEnter() {
    CCLayerColor::onEnter();
    CCTouchDispatcher::get()->addTargetedDelegate(static_cast<TouchDelegate*>(this), -128, true);
}

void ArtNode::onExit() {
    CCLayerColor::onExit();
    CCTouchDispatcher::get()->removeDelegate(static_cast<TouchDelegate*>(this));
    CursorManager::get()->setCursor(Cursor::ARROW);
}