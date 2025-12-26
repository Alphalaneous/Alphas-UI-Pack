#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>

#include "AdvancedScrollBar.hpp"
#include "ScrollContent.hpp"
#include "AdvancedScrollDelegate.hpp"
#include "../../export.hpp"
#include "../../touch/TouchDelegate.hpp"

namespace alpha::ui {

class ALPHAS_UI_PACK_DLL AdvancedScrollLayer : public cocos2d::CCNode, public alpha::dispatcher::TouchDelegate, public alpha::dispatcher::AdvancedScrollDelegate {
public:
    using CullingMethod = std::function<void(cocos2d::CCNode* content, const cocos2d::CCPoint& scroll)>;
    using cocos2d::CCNode::addChild;
    using cocos2d::CCNode::removeChild;

    AdvancedScrollLayer();
    ~AdvancedScrollLayer();

    static AdvancedScrollLayer* create(const cocos2d::CCSize& size, const CullingMethod& cullingMethod);
    static AdvancedScrollLayer* create(const cocos2d::CCSize& size, CullingMethod&& cullingMethod);
    static AdvancedScrollLayer* create(const cocos2d::CCSize& size);

    static void registerDevTools();

    bool init(const cocos2d::CCSize& size, CullingMethod cullingMethod);

    ScrollContent* getContentLayer();

    void setScrollX(float x, bool smooth = false);
    void setScrollY(float y, bool smooth = false);

    cocos2d::CCPoint getScrollPoint();

    void setInnerContentSize(const cocos2d::CCSize& size);

    void setLayout(geode::Layout* layout, bool apply = true, bool respectAnchor = true);
    void updateLayout(bool updateChildOrder = true);

    void setCullingMethod(const CullingMethod& method);
    void setCullingMethod(CullingMethod&& method);

    void setCullingEnabled(bool value);
    bool isCullingEnabled();

    void setDraggingEnabled(bool value);
    bool isDraggingEnabled();

    void setVeritcalScroll(bool value);
    void setHorizontalScroll(bool value);

    bool hasVerticalScroll();
    bool hasHorizontalScroll();

    void setVerticalScrollWheel(bool value);
    void setHorizontalScrollWheel(bool value);

    bool hasVerticalScrollWheel();
    bool hasHorizontalScrollWheel();

    void setVerticalScrollForHorizontal(bool value);
    bool hasVerticalScrollForHorizontal();

    void setOvershoot(float value);
    float getOvershoot();

    void setFriction(float value);
    float getFriction();

    void setMinVelocity(float value);
    float getMinVelocity();

    void setMinZoom(float value);
    void setMaxZoom(float value);

    float getMinZoom();
    float getMaxZoom();

    void setZoom(float zoom);
    float getZoom();

    void allowEmptyClickThrough(bool allow);
    bool allowsEmptyClickThrough();

    void allowZoom(bool allow);
    bool allowsZoom();

    float getHorizontalMax();
    float getVerticalMax();

    float getHorizontalScrollPercent();
    float getVerticalScrollPercent();

    float getVerticalPages();
    float getHorizontalPages();

    void setTouchPriority(int prio);
    int getTouchPriority();
    
    virtual void update(float dt) override;
    virtual void setContentSize(const cocos2d::CCSize& size) override;

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void visit() override;

    virtual void addChild(cocos2d::CCNode* child, int zOrder, int tag) override;
    virtual void removeChild(cocos2d::CCNode* child, bool cleanup) override;

    virtual cocos2d::CCArray* getChildren() override;
    virtual unsigned int getChildrenCount(void) const override;
    virtual void removeAllChildrenWithCleanup(bool cleanup) override;

    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent* event) override;
    virtual void scroll(float x, float y) override;
    virtual void zoom(float zoom) override;

    virtual void keyPress(cocos2d::enumKeyCodes key, bool isKeyDown, bool isKeyRepeat) override;

private:
    void constrain(bool skipInertiaCheck = false);
    void updateInertia(float dt);
    void bounceBack(bool horizontal);
    void collectHandlers(cocos2d::CCNode* node, std::vector<std::pair<cocos2d::CCTouchHandler*, int>>& out);
    void cancelTouchesRecursive(cocos2d::CCNode* node, cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void cancelChildrenTouches(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void setMinContainerSize();
    void setContainerDirty();
    void handleTouchPrio();
    void cull();
    void offsetStencilWidth(float width);
    void offsetStencilHeight(float height);
    geode::Result<float> getHorizontalScroll(cocos2d::enumKeyCodes key);
    geode::Result<float> getVerticalScroll(cocos2d::enumKeyCodes key, bool horizontal = false);

    friend class ScrollContent;
    friend class AdvancedScrollBar;
protected:
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}