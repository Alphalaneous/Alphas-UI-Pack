#pragma once

#include <Geode/cocos/touch_dispatcher/CCTouch.h>
#include "TouchDispatcher.hpp"

#include "../export.hpp"

namespace alpha::dispatcher {

/**
 * @brief Touch event wrapper with mouse button information.
 *
 * Extends cocos2d::CCTouch by associating a specific mouse button
 * with the touch event.
 */
class ALPHAS_UI_PACK_DLL TouchEvent : public cocos2d::CCTouch {
public:
    /**
     * @brief Constructs an empty TouchEvent.
     */
    TouchEvent();

    /**
     * @brief Destroys the TouchEvent.
     */
    ~TouchEvent();

    /**
     * @brief Creates a TouchEvent for the specified mouse button.
     *
     * @param button Mouse button associated with the event.
     * @return Newly created TouchEvent instance.
     */
	static TouchEvent* create(MouseButton button);

    /**
     * @brief Initializes the TouchEvent with a mouse button.
     *
     * @param button Mouse button associated with the event.
     * @return True if initialization succeeds; false otherwise.
     */
	bool init(MouseButton button);

    /**
     * @brief Updates the touch position information.
     *
     * @param point Touch position in screen coordinates.
     */
	void setTouchInfo(const cocos2d::CCPoint& point);

    /**
     * @brief Returns the mouse button associated with this event.
     *
     * @return Mouse button value.
     */
    MouseButton getButton();

protected:
    /// Internal implementation details.
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}
