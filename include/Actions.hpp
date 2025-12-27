#pragma once

#include <Geode/cocos/actions/CCActionInterval.h>

#include "export.hpp"

namespace alpha::ui {

/**
 * @brief Action that moves a node horizontally to a target X coordinate.
 *
 * The Y position of the target node remains unchanged.
 */
class ALPHAS_UI_PACK_DLL CCMoveToX : public cocos2d::CCActionInterval {
public:
    /**
     * @brief Creates a CCMoveToX action.
     *
     * @param duration Duration of the action in seconds.
     * @param x Target X coordinate.
     * @return Newly created CCMoveToX action.
     */
    static CCMoveToX* create(float duration, float x);

    /**
     * @brief Initializes the action with a duration and target X coordinate.
     *
     * @param duration Duration of the action in seconds.
     * @param x Target X coordinate.
     * @return True if initialization succeeds; false otherwise.
     */
    bool initWithDuration(float duration, float x);

    /**
     * @brief Starts the action with the specified target node.
     *
     * Records the initial X position of the target node.
     *
     * @param target Node to apply the action to.
     */
    virtual void startWithTarget(cocos2d::CCNode* target) override;

    /**
     * @brief Updates the action state.
     *
     * @param t Normalized time in the range [0, 1].
     */
    virtual void update(float t) override;

private:
    float m_startX;
    float m_endX;
    float m_deltaX;
};

/**
 * @brief Action that moves a node vertically to a target Y coordinate.
 *
 * The X position of the target node remains unchanged.
 */
class ALPHAS_UI_PACK_DLL CCMoveToY : public cocos2d::CCActionInterval {
public:
    /**
     * @brief Creates a CCMoveToY action.
     *
     * @param duration Duration of the action in seconds.
     * @param y Target Y coordinate.
     * @return Newly created CCMoveToY action.
     */
    static CCMoveToY* create(float duration, float y);

    /**
     * @brief Initializes the action with a duration and target Y coordinate.
     *
     * @param duration Duration of the action in seconds.
     * @param y Target Y coordinate.
     * @return True if initialization succeeds; false otherwise.
     */
    bool initWithDuration(float duration, float y);

    /**
     * @brief Starts the action with the specified target node.
     *
     * Records the initial Y position of the target node.
     *
     * @param target Node to apply the action to.
     */
    virtual void startWithTarget(cocos2d::CCNode* target) override;

    /**
     * @brief Updates the action state.
     *
     * @param dt Delta time since the last update.
     */
    virtual void update(float dt) override;

private:
    float m_startY;
    float m_endY;
    float m_deltaY;
};

}
