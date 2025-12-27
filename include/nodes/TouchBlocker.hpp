#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>
#include "../touch/TouchDelegate.hpp"

#include "../export.hpp"

namespace alpha::ui {

class AdvancedScrollLayer;

/**
 * @brief Node that blocks touch input for other nodes.
 *
 * Can be used to intercept clicks or touches and block them
 * based on a specific target node. Inherits from CCNode and TouchDelegate.
 */
class ALPHAS_UI_PACK_DLL TouchBlocker : public cocos2d::CCNode, public alpha::dispatcher::TouchDelegate {
public:
    /**
     * @brief Constructs a TouchBlocker.
     */
    TouchBlocker();

    /**
     * @brief Destroys the TouchBlocker.
     */
    ~TouchBlocker();

    /**
     * @brief Creates a TouchBlocker for the specified target node.
     *
     * @param target Node that will be checked to block touches.
     * @return Newly created TouchBlocker instance.
     */
    static TouchBlocker* create(cocos2d::CCNode* target);

    /**
     * @brief Initializes the TouchBlocker with a target node.
     *
     * @param target Node that will be checked to block touches.
     * @return True if initialization succeeds; false otherwise.
     */
    bool init(cocos2d::CCNode* target);

    /**
     * @brief Sets the touch priority for this blocker.
     *
     * Lower values receive touches first.
     *
     * @param prio Priority value.
     */
    void setTouchPriority(int prio);

    /**
     * @brief Returns the current touch priority.
     *
     * @return Touch priority value.
     */
    int getTouchPriority();

    /**
     * @brief Sets the target node for this blocker.
     *
     * @param node Node that will be checked to block touches.
     */
    void setTarget(cocos2d::CCNode* node);

    /**
     * @brief Called when the node enters the scene.
     *
     * Registers touch handling.
     */
    virtual void onEnter() override;

    /**
     * @brief Called when the node exits the scene.
     *
     * Unregisters touch handling.
     */
    virtual void onExit() override;

    /**
     * @brief Handles the beginning of a click event.
     *
     * Intercepts the touch based on the target node.
     *
     * @param touch TouchEvent associated with the click.
     * @return True if the touch is claimed.
     */
    virtual bool clickBegan(alpha::dispatcher::TouchEvent* touch) override;

protected:
    /// Internal implementation details.
    struct Impl;
    std::unique_ptr<Impl> m_impl; 

    /// AdvancedScrollLayer is allowed to access internal members.
    friend class AdvancedScrollLayer;
};

}