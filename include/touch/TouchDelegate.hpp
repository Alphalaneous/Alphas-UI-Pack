#pragma once

#include <Geode/cocos/touch_dispatcher/CCTouchDelegateProtocol.h>
#include "TouchEvent.hpp"

#include "../export.hpp"

namespace alpha::dispatcher {

/**
 * @brief Base class for receiving touch and mouse events.
 *
 * Extend this class to handle click and mouse interactions
 * in your application.
 */
class ALPHAS_UI_PACK_DLL TouchDelegate : public cocos2d::CCTouchDelegate {
public:
    /**
     * @brief Called when a click begins.
     *
     * @param touch Touch event associated with the click.
     * @return True to claim the event, false otherwise.
     */
	virtual bool clickBegan(TouchEvent* touch) { return false; }

    /**
     * @brief Called when a click moves.
     *
     * @param touch Touch event associated with the movement.
     */
	virtual void clickMoved(TouchEvent* touch) {}

    /**
     * @brief Called when a click ends.
     *
     * @param touch Touch event associated with the release.
     */
	virtual void clickEnded(TouchEvent* touch) {}

    /**
     * @brief Called when the mouse enters a region.
     *
     * @param touch Touch event representing the entry.
     * @return True to claim the event, false otherwise.
     */
	virtual bool mouseEntered(TouchEvent* touch) { return false; }

    /**
     * @brief Called when the mouse exits a region.
     *
     * @param touch Touch event representing the exit.
     */
	virtual void mouseExited(TouchEvent* touch) {}

    /**
     * @brief Called when the mouse moves inside a region.
     *
     * @param touch Touch event representing the movement.
     */
	virtual void mouseMoved(TouchEvent* touch) {}

    /**
     * @brief Called by cocos2d when a touch begins.
     *
     * Converts the CCTouch to a TouchEvent and dispatches it.
     *
     * @param touch Original CCTouch instance.
     * @param event Original CCEvent instance.
     * @return True if the touch was handled.
     */
    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

    /**
     * @brief Called by cocos2d when a touch moves.
     *
     * Converts the CCTouch to a TouchEvent and dispatches it.
     *
     * @param touch Original CCTouch instance.
     * @param event Original CCEvent instance.
     */
    void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

    /**
     * @brief Called by cocos2d when a touch ends.
     *
     * Converts the CCTouch to a TouchEvent and dispatches it.
     *
     * @param touch Original CCTouch instance.
     * @param event Original CCEvent instance.
     */
    void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

    /**
     * @brief Called by cocos2d when a touch is cancelled.
     *
     * Converts the CCTouch to a TouchEvent and dispatches it.
     *
     * @param touch Original CCTouch instance.
     * @param event Original CCEvent instance.
     */
    void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
};

}
