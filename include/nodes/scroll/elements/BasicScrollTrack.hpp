#pragma once

#include <Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h>
#include "../ScrollBarElement.hpp"
#include "../../../export.hpp"
#include "ccTypes.h"

namespace alpha::ui {

/**
 * @brief Track element for a basic-style scroll bar.
 *
 * Represents the scroll track along which the handle moves.
 */
class ALPHAS_UI_PACK_DLL BasicScrollTrack : public ScrollBarElement {
public:
    /**
     * @brief Constructs a new BasicScrollTrack.
     */
    BasicScrollTrack();

    /**
     * @brief Destroys the BasicScrollTrack.
     */
    ~BasicScrollTrack();

    /**
     * @brief Creates a new BasicScrollTrack instance.
     * @return Pointer to the created BasicScrollTrack.
     */
    static BasicScrollTrack* create();

    /**
     * @brief Initializes the track element.
     * @return True if initialization succeeded.
     */
    bool init() override;

    /**
     * @brief Sets the background color of the track.
     * @param color Color to apply to the background.
     */
    void setBackgroundColor(const cocos2d::ccColor4B& color);

    /**
     * @brief Sets the color of the track when clicked.
     * @param color Clicked color.
     */
    void setClickColor(const cocos2d::ccColor4B& color);

    /**
     * @brief Returns the background color of the track.
     * @return Background color.
     */
    cocos2d::ccColor4B getBackgroundColor();

    /**
     * @brief Returns the click color of the track.
     * @return Click color.
     */
    cocos2d::ccColor4B getClickColor();

    /**
     * @brief Sets the content size of the track element.
     * @param contentSize Desired size.
     */
    virtual void setContentSize(const cocos2d::CCSize& contentSize) override;

    /**
     * @brief Called when the track is clicked.
     * @param pos Click position relative to the track.
     */
    virtual void onClick(const cocos2d::CCPoint& pos) override;

    /**
     * @brief Called when the click is released.
     * @param pos Release position relative to the track.
     */
    virtual void onRelease(const cocos2d::CCPoint& pos) override;

protected:
    /// Internal implementation details.
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}
