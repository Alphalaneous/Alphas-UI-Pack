#pragma once

#include <Geode/cocos/platform/CCGL.h>
#include <Geode/cocos/sprite_nodes/CCSprite.h>
#include <Geode/cocos/touch_dispatcher/CCTouch.h>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/function.hpp>
#include "../export.hpp"
#include "../Image.hpp"
#include "../touch/TouchDelegate.hpp"

namespace alpha::ui {

class ALPHAS_UI_PACK_DLL MirrorNode : public cocos2d::CCSprite, public dispatcher::TouchDelegate {
public:
    MirrorNode();
    ~MirrorNode();

    /// Create a MirrorNode with another node. Set ownsNode to true for this to store the node as a Ref.
    static MirrorNode* create(cocos2d::CCNode* node, bool ownsNode = false);

    /// Initializes the MirrorNode.
    bool init(cocos2d::CCNode* node, bool ownsNode);

    /// Initializes the framebuffer object.
    void initFBO();

    /// Renders the node (called every frame).
    void render();

    /// Resets the framebuffer object.
    void resetFBO();

    /// Touch overrides for forwarding.
    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
	bool mouseEntered(dispatcher::TouchEvent* touch) override;
	void mouseExited(dispatcher::TouchEvent* touch) override;
	void mouseMoved(dispatcher::TouchEvent* touch) override;

    void update(float dt) override;
    void onEnter() override;
    void onExit() override;
    
    virtual void registerWithTouchDispatcher();

    /// Sets the touch priority.
    void setTouchPriority(int priority);
    int getTouchPriority();

    /// Sets if the original node passed in should still be visible in the node tree, setting to false will force setVisible(false) on it every frame.
    void showOriginal(bool show);
    bool showingOriginal();

    /// Set the node being mirrored.
    void setNode(CCNode* node, bool ownsNode = false);
    CCNode* getNode();

    /// Set an additional node transform to the node being mirrored only.
    void setNodeTransform(const cocos2d::CCAffineTransform& additionalTransform);
    const cocos2d::CCAffineTransform& getNodeTransform();

    /// Set whether this node owns the node being mirrored as a Ref.
    void setOwnsNode(bool ownsNode);
    bool ownsNode();

    /// Sets the viewport size, by default it is the window size.
    void setViewportSize(const cocos2d::CCSize& size);
    const cocos2d::CCSize& getViewportSize();

    /// Capture the current frame as a texture.
    cocos2d::CCTexture2D* captureTexture();

    /// Capture the current frame as a sprite.
    cocos2d::CCSprite* captureSprite();

    /// Capture the current frame as an image.
    Image* captureImage();

protected:

    /// runs touch forwarding on node
    template <class Delegate>
    void forwardTouch(geode::Function<void(Delegate* delegate)> callback) {
        if (!geode::cocos::nodeIsVisible(this)) return;

        auto node = getNode();
        if (!node) return;

        auto delegate = geode::cast::typeinfo_cast<Delegate*>(node);
        if (!delegate) return;

        auto parent = node->getParent();
        auto pos = node->getPosition();
        node->m_pParent = this;
        node->setPosition(getContentSize() / 2.f);
        node->setVisible(true);

        callback(delegate);

        node->m_pParent = parent;
        node->setPosition(pos);
        node->setVisible(showingOriginal());
    }

    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}
