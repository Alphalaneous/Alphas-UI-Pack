#pragma once

#include <Geode/Geode.hpp>
#include "API.hpp"
#include "ArtNode.hpp"
#include "SimpleColorPicker.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct WindowsClassicButton : public CCNodeRGBA, public TouchDelegate {

    static WindowsClassicButton* create() {
        auto ret = new WindowsClassicButton();
        if (ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    };

    bool init() override {
        if (!CCNodeRGBA::init()) return false;
        setContentSize({8.f, 8.f});

        m_closeSpr = CCSprite::createWithSpriteFrameName("particle_18_001.png");
        m_closeSpr->setColor({255, 255, 255});
        m_closeSpr->setScale(0.3f);
        m_closeSpr->setZOrder(1);
        m_closeSpr->setPosition(getContentSize()/2);

        m_background = CCScale9Sprite::create("WindowsClassic.png"_spr);
        m_background->setScale(0.5f);
        m_background->setColor({60, 60, 60});
        m_background->setContentSize(CCSize{8, 8} / m_background->getScale());
        m_background->setPosition(getContentSize()/2);


        addChild(m_closeSpr);
        addChild(m_background);

        return true;
    }

    virtual bool clickBegan(TouchEvent* touch) override { 
        if (!alpha::utils::isTouchInsideNode(this, touch->getLocation())) return false;
        if (touch->getButton() != MouseButton::LEFT) return true;
        m_background->setRotation(180);

        auto offset = CCPoint{0.5f, -0.5f};
        m_closeSpr->setPosition((getContentSize()/2) + offset);

        return true; 
    }

	virtual void clickMoved(TouchEvent* touch) override {
        if (touch->getButton() != MouseButton::LEFT) return;
        if (alpha::utils::isTouchInsideNode(this, touch->getLocation())) {
            auto offset = CCPoint{0.5f, -0.5f};
            m_closeSpr->setPosition((getContentSize()/2) + offset);
            m_background->setRotation(180);
        }
        else {
            m_background->setRotation(0);
            m_closeSpr->setPosition(getContentSize()/2);
        }
    }

	virtual void clickEnded(TouchEvent* touch) override {
        if (!alpha::utils::isTouchInsideNode(this, touch->getLocation())) return;
        if (touch->getButton() != MouseButton::LEFT) return;

        if (m_callback) m_callback();

        m_background->setRotation(0);
        m_closeSpr->setPosition(getContentSize()/2);
    }

    virtual void onEnter() override {
        CCTouchDispatcher::get()->addTargetedDelegate(this, -500, true);
    }

    virtual void onExit() override {
        CCTouchDispatcher::get()->removeDelegate(this);
    }

    void setCallback(const std::function<void()>& callback) {
        m_callback = std::move(callback);
    }

    std::function<void()> m_callback;
    CCScale9Sprite* m_background;
    CCSprite* m_closeSpr;
};

struct ScrollTestLayer : public CCNode, public TouchDelegate {

    CCScale9Sprite* m_background;
    AdvancedScrollLayer* m_scrollLayer;
    RenderNode* m_render;
    CCScale9Sprite* m_renderShadow;
    SimpleColorPicker* m_colorPicker;
    AdvancedScrollBar* m_verticalScrollBar;
    AdvancedScrollBar* m_horizontalScrollBar;
    CCLayerColor* m_artBackground;
    ArtNode* m_artNode;
    CCScale9Sprite* m_artShadow;
    CCSize m_size = {1500, 1500};

    CCNode* m_container;
    CCLayerGradient* m_titleBar;
    WindowsClassicButton* m_closeButton;

    bool m_inited = false;
    bool m_moving;
    CCPoint m_startMove;
    CCPoint m_startPos;

    static ScrollTestLayer* create() {
        auto ret = new ScrollTestLayer();
        if (ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

	virtual bool clickBegan(TouchEvent* touch) override { 
        if (!alpha::utils::isTouchInsideNode(this, touch->getLocation())) return false;
        if (touch->getButton() != MouseButton::LEFT) return true;
        
        if (!alpha::utils::isTouchInsideNode(m_container, touch->getLocation())) {
            if (alpha::utils::isTouchInsideNode(m_titleBar, touch->getLocation())) {
                m_startMove = getParent()->convertToNodeSpace(touch->getLocation());
                m_startPos = getPosition();
                m_moving = true;
            }
            else {

            }
        }

        return true;
    }

    virtual void clickMoved(TouchEvent* touch) override {
        if (touch->getButton() != MouseButton::LEFT) return;

        if (m_moving) {
            auto currentPos = getParent()->convertToNodeSpace(touch->getLocation());
            auto diff = currentPos - m_startMove;

            setPosition(m_startPos + diff);
        }
    }

	virtual void clickEnded(TouchEvent* touch) override {
        if (touch->getButton() != MouseButton::LEFT) return;
        m_moving = false;
    }

    virtual void onEnter() override {
        CCNode::onEnter();
        CCTouchDispatcher::get()->addTargetedDelegate(this, -499, true);
    }   

    virtual void onExit() override {
        CCNode::onExit();
        CCTouchDispatcher::get()->removeDelegate(this);
    }

    void positionAndSize() {
        auto scrollPX = m_scrollLayer->getScrollPoint().x / std::max(m_scrollLayer->getHorizontalMax(), 0.001f);
        auto scrollPY = m_scrollLayer->getScrollPoint().y / std::max(m_scrollLayer->getVerticalMax(), 0.001f);
        
        m_container->setContentSize({getContentWidth() - 4, getContentHeight() - 16});

        m_titleBar->setPosition({2, getContentHeight() - 2});
        m_titleBar->setContentSize({getContentWidth() - 4, 10});

        m_closeButton->setPosition({m_titleBar->getContentWidth() - 1, m_titleBar->getContentHeight() / 2});

        m_background->setContentSize(getContentSize() / m_background->getScale());
        m_background->setPosition(getContentSize()/2.f);

        m_scrollLayer->setPosition({0, 10});
        m_scrollLayer->setContentSize({m_container->getContentWidth() - 92, m_container->getContentHeight() - 10});

        m_render->setPosition({m_container->getContentWidth(), m_container->getContentHeight()});
        m_renderShadow->setPosition(m_render->getPosition() + CCPoint{5, -5});

        m_colorPicker->setPosition({m_container->getContentWidth(), 5});

        auto bounds = m_scrollLayer->boundingBox();

        m_verticalScrollBar->setContentHeight(m_scrollLayer->getScaledContentHeight());
        m_horizontalScrollBar->setContentHeight(m_scrollLayer->getScaledContentWidth());

        m_verticalScrollBar->setPosition({bounds.getMaxX() + m_verticalScrollBar->getScaledContentWidth()/2.f, bounds.getMinY() + m_scrollLayer->getScaledContentHeight()/2.f});
        m_horizontalScrollBar->setPosition({bounds.getMinX() + m_scrollLayer->getScaledContentWidth()/2, bounds.getMinY() - m_horizontalScrollBar->getScaledContentWidth()/2.f});

        auto sizeScaleX = m_size.width / std::max(m_scrollLayer->getContentWidth(), 0.001f);
        auto sizeScaleY = m_size.height / std::max(m_scrollLayer->getContentHeight(), 0.001f);

        auto sizeScale = std::max(sizeScaleX, sizeScaleY);
    
        m_artBackground->setContentSize({(m_scrollLayer->getContentWidth() * sizeScale) * 3, (m_scrollLayer->getContentHeight() * sizeScale) * 3});
        
		m_scrollLayer->setInnerContentSize(m_artBackground->getContentSize());

        m_artNode->setPosition(m_artBackground->getContentSize()/2.f);
        m_artShadow->setContentSize(m_artNode->getContentSize() / m_artShadow->getScale());
        m_artShadow->setPosition(m_artNode->getPosition() + CCPoint{30, -30});

        auto bound = CCSize{80, 80};
        m_renderShadow->setContentSize(bound / m_renderShadow->getScale());

        auto maxS = std::max(std::max(m_scrollLayer->getContentWidth(), m_scrollLayer->getContentHeight()), 0.001f);
        auto maxB = std::max(std::max(m_artBackground->getContentWidth(), m_artBackground->getContentHeight()), 0.001f);

        float newZoom = maxS / maxB;

        m_scrollLayer->setMinZoom(newZoom);
        if (m_scrollLayer->getZoom() < newZoom) {
            m_scrollLayer->setZoom(newZoom);
        }

        if (m_inited) {
            m_scrollLayer->setScrollX(m_scrollLayer->getHorizontalMax() * scrollPX);
            m_scrollLayer->setScrollY(m_scrollLayer->getVerticalMax() * scrollPY);
        }
    }

    bool init() override {
        if (!CCNode::init()) return false;

        setAnchorPoint({0.5f, 0.5f});
        ignoreAnchorPointForPosition(false);

        CCSize winSize = CCDirector::get()->getWinSize();

        m_container = CCNode::create();
        m_container->ignoreAnchorPointForPosition(false);
        m_container->setAnchorPoint({0.f, 0.f});
        m_container->setPosition({2, 2});
        m_container->setZOrder(1);

        addChild(m_container);

        m_titleBar = CCLayerGradient::create({60, 60, 60, 255}, {80, 80, 80, 255});
        m_titleBar->setAnchorPoint({0.f, 1.f});
        m_titleBar->ignoreAnchorPointForPosition(false);
        m_titleBar->setZOrder(1);
        m_titleBar->setVector({1, 0});

		addChild(m_titleBar);

        auto label = CCLabelBMFont::create("Paint Prototype", "chatFont.fnt");
        label->setColor({255, 255, 255});
        label->setPosition({2, 5});
        label->setAnchorPoint({0.f, 0.5f});
        label->setScale(0.4f);

        m_titleBar->addChild(label);

        m_closeButton = WindowsClassicButton::create();
        m_closeButton->setAnchorPoint({1.f, 0.5f});
        m_closeButton->ignoreAnchorPointForPosition(false);
        m_closeButton->setContentSize({8, 8});
        m_closeButton->setCallback([this] {
            removeFromParent();
        });

        m_titleBar->addChild(m_closeButton);

        m_background = CCScale9Sprite::create("WindowsClassic.png"_spr);
        m_background->setScale(0.5f);

        m_background->setColor({30, 30, 30});

		addChild(m_background);

        m_scrollLayer = AdvancedScrollLayer::create({getContentWidth() - 100, getContentHeight() - 10});
		m_scrollLayer->setHorizontalScroll(true);
        m_scrollLayer->setDraggingEnabled(false);
        m_scrollLayer->setAnchorPoint({0.f, 0.f});
        m_scrollLayer->setTouchPriority(-500);
        
		m_scrollLayer->setZOrder(100);
        m_scrollLayer->allowZoom(true);

        m_artBackground = CCLayerColor::create({40, 40, 40, 255});

        m_artBackground->setAnchorPoint({0.f, 1.f});
        m_artBackground->setID("bg");
        
        m_scrollLayer->setMinZoom(m_scrollLayer->getContentWidth() / m_artBackground->getContentWidth());

        m_artNode = ArtNode::create(m_size);
        m_artNode->setAnchorPoint({0.5f, 0.5f});
        m_artNode->setID("art-node");
        m_artNode->setZOrder(1);
        //m_artNode->setTouchPriority(-501);

        m_artShadow = CCScale9Sprite::create("Shadow.png"_spr);
        m_artShadow->setScale(8);
        m_artShadow->setAnchorPoint({0.5f, 0.5f});
        m_artShadow->setColor({0, 0, 0});
        m_artShadow->setOpacity(127);
        m_artBackground->addChild(m_artShadow);

        m_artBackground->addChild(m_artNode);

		m_scrollLayer->addChild(m_artBackground);

		m_container->addChild(m_scrollLayer);

        m_render = RenderNode::create(m_artNode, true);

        auto bound = CCSize{80, 80};
        auto previewScaleX = bound.width / m_render->getContentWidth();
        auto previewScaleY = bound.height / m_render->getContentHeight();

        m_render->setScaleX(previewScaleX);
        m_render->setScaleY(previewScaleY);

        m_render->setAnchorPoint({1.f, 1.f});
        m_render->setZOrder(10);

		m_container->addChild(m_render);

        m_renderShadow = CCScale9Sprite::create("Shadow.png"_spr);
        m_renderShadow->setScale(2);
        m_renderShadow->setAnchorPoint(m_render->getAnchorPoint());
        m_renderShadow->setColor({0, 0, 0});
        m_renderShadow->setOpacity(127);
        m_renderShadow->setZOrder(9);

        m_container->addChild(m_renderShadow);

        m_colorPicker = SimpleColorPicker::create(m_artNode->m_clickColor, [this](const cocos2d::ccColor4B& color) {
            m_artNode->m_clickColor = color;
        });

        m_colorPicker->setAnchorPoint({1.f, 0.f});
        m_colorPicker->setZOrder(10);
        m_colorPicker->setScale(0.55f);

        m_container->addChild(m_colorPicker);

		m_verticalScrollBar = AdvancedScrollBar::create(m_scrollLayer, ScrollOrientation::VERTICAL);
		m_horizontalScrollBar = AdvancedScrollBar::create(m_scrollLayer, ScrollOrientation::HORIZONTAL);

        ccColor4B arrowColor = {255, 255, 255, 255};
        ccColor4B arrowBGColor = {60, 60, 60, 255};
        ccColor4B trackColor = {30, 30, 30, 255};
        ccColor4B trackClickColor = {40, 40, 40, 255};
        ccColor4B handleColor = {60, 60, 60, 255};

        m_verticalScrollBar->setStyle(WindowsClassicScrollStyle());

        auto upArrowV = static_cast<WindowsClassicArrow*>(m_verticalScrollBar->getUpArrow());

        upArrowV->setBackgroundColor(arrowBGColor);
        upArrowV->setArrowColor(arrowColor);

        auto downArrowV = static_cast<WindowsClassicArrow*>(m_verticalScrollBar->getDownArrow());

        downArrowV->setBackgroundColor(arrowBGColor);
        downArrowV->setArrowColor(arrowColor);
        auto trackV = static_cast<WindowsClassicTrack*>(m_verticalScrollBar->getTrack());

        trackV->setBackgroundColor(trackColor);
        trackV->setClickColor(trackClickColor);

        auto handleV = static_cast<WindowsClassicHandle*>(m_verticalScrollBar->getHandle());

        handleV->setBackgroundColor(handleColor);

        m_horizontalScrollBar->setStyle(WindowsClassicScrollStyle());

        auto upArrowH = static_cast<WindowsClassicArrow*>(m_horizontalScrollBar->getUpArrow());

        upArrowH->setBackgroundColor(arrowBGColor);
        upArrowH->setArrowColor(arrowColor);

        auto downArrowH = static_cast<WindowsClassicArrow*>(m_horizontalScrollBar->getDownArrow());

        downArrowH->setBackgroundColor(arrowBGColor);
        downArrowH->setArrowColor(arrowColor);

        auto trackH = static_cast<WindowsClassicTrack*>(m_horizontalScrollBar->getTrack());
        
        trackH->setBackgroundColor(trackColor);
        trackH->setClickColor(trackClickColor);

        auto handleH = static_cast<WindowsClassicHandle*>(m_horizontalScrollBar->getHandle());

        handleH->setBackgroundColor(handleColor);

		m_container->addChild(m_verticalScrollBar);
		m_container->addChild(m_horizontalScrollBar);

        setContentSize({300, 200});

        positionAndSize();

        m_scrollLayer->setZoom(0.25f);

        m_scrollLayer->setScrollX(m_scrollLayer->getHorizontalMax()/2);
        m_scrollLayer->setScrollY(m_scrollLayer->getVerticalMax()/2);

        m_inited = true;

        setPosition(winSize/2);

        return true;
    }

    void setContentSize(const CCSize& size) override {
        CCNode::setContentSize(size);
        if (m_inited) positionAndSize();
    }

    void show() {
        auto scene = CCScene::get();
        setZOrder(scene->getHighestChildZ());
        scene->addChild(this);
    }
};