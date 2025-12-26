#pragma once

#include <Geode/cocos/platform/CCGL.h>
#include <Geode/cocos/sprite_nodes/CCSprite.h>
#include <Geode/utils/cocos.hpp>

#include "../export.hpp"

namespace alpha::ui {

class ALPHAS_UI_PACK_DLL RenderNode : public cocos2d::CCSprite {
public:
    using cocos2d::CCSprite::addChild;
    using cocos2d::CCSprite::removeChild;

    RenderNode();
    ~RenderNode();

    static RenderNode* create(cocos2d::CCNode* node, bool constrain = false);

    bool init(cocos2d::CCNode* node, bool constrain);
    void initFBO();
    void render();

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void update(float dt) override;
    virtual void addChild(cocos2d::CCNode* child, int zOrder, int tag) override;
    virtual void removeChild(cocos2d::CCNode* child, bool cleanup) override;
    virtual cocos2d::CCArray* getChildren() override;
    virtual unsigned int getChildrenCount() const override;
    virtual void removeAllChildrenWithCleanup(bool cleanup) override;

    CCNode* getNode();

protected:
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}