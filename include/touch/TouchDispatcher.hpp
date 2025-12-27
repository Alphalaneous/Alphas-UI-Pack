#pragma once

#include <Geode/cocos/cocoa/CCObject.h>

#include "../export.hpp"

namespace alpha::dispatcher {

class TouchEvent;

/**
 * @brief Types of touch events.
 */
enum class TouchType {
    /// Touch or click began.
    CLICK_BEGAN,

    /// Touch or click moved.
    CLICK_MOVED,

    /// Touch or click ended.
    CLICK_ENDED
};

/**
 * @brief Mouse button identifiers.
 */
enum class MouseButton : unsigned int {
    /// Primary mouse button.
    LEFT = 0,

    /// Touch input (alias of LEFT).
    TOUCH = 0,

    /// Middle mouse button.
    MIDDLE,

    /// Secondary mouse button.
    RIGHT,

    /// Additional mouse button 3.
    BUTTON3,

    /// Additional mouse button 4.
    BUTTON4,

    /// Hover state (no button pressed).
    HOVER
};

/**
 * @brief Dispatches touch and mouse events to registered handlers.
 *
 * Manages clicks, hovers, and input polling for the application.
 */
class ALPHAS_UI_PACK_DLL TouchDispatcher : public cocos2d::CCObject {
public:
    /**
     * @brief Constructs a new TouchDispatcher.
     */
    TouchDispatcher();

    /**
     * @brief Destroys the TouchDispatcher.
     */
    ~TouchDispatcher();

    /**
     * @brief Returns the global TouchDispatcher instance.
     *
     * @return Pointer to the global TouchDispatcher.
     */
    static TouchDispatcher* get();

protected:
    /// Initializes the dispatcher.
    void init();

    /// Processes hover events for a given touch.
    void hovers(TouchEvent* touch);

    /**
     * @brief Processes click events for a given touch.
     *
     * @param touch Touch event to handle.
     * @param type Type of click event (began, moved, ended).
     */
    void clicks(TouchEvent* touch, TouchType type);

    /// Polls input at the specified position.
    void pollInput(const cocos2d::CCPoint& pos);

    /// Handles hover logic at the specified position.
    void handleHover(const cocos2d::CCPoint& pos);

    /// Rearranges input handlers (internal use).
    void rearrangeHandlers();

    /**
     * @brief Updates the dispatcher state each frame.
     *
     * @param dt Delta time since the last update.
     */
    virtual void update(float dt) override;
    
    /// Internal implementation details.
    struct Impl;
    std::unique_ptr<Impl> m_impl; 

    friend class MyCCTouchDispatcher;
};

}
