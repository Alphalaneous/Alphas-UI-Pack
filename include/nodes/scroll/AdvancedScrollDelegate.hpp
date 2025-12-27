#pragma once

#include "../../export.hpp"

namespace alpha::dispatcher {

/**
 * @brief Interface for nodes that respond to keyboard and mouse events related to scroll when focused.
 *
 * AdvancedScrollDelegate defines callbacks for scrolling, zooming, and keyboard input.
 * Classes implementing this interface can receive scroll/zoom events from the mouse and keyboard
 */
class ALPHAS_UI_PACK_DLL AdvancedScrollDelegate {
public:
    /**
     * @brief Called when the mouse wheel is scrolled.
     *
     * @param x Horizontal scroll offset delta.
     * @param y Vertical scroll offset delta.
     */
    virtual void scroll(float x, float y) {};

    /**
     * @brief Called when the zoom keys are used (Ctrl +/-)
     *
     * @param zoom Current zoom level.
     */
    virtual void zoom(float zoom) {};

    /**
     * @brief Called when a key event occurs while the node is focused.
     *
     * @param key The key code pressed.
     * @param isKeyDown True if the key is pressed; false if released.
     * @param isKeyRepeat True if this is a repeated key event.
     */
    virtual void keyPress(cocos2d::enumKeyCodes key, bool isKeyDown, bool isKeyRepeat) {};
};

}
