#pragma once

#include <Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h>
#include "../ScrollBarElement.hpp"
#include "../../../export.hpp"

namespace alpha::ui {

/**
 * @brief Track element for a scroll bar using the Windows Classic style.
 *
 * Represents the scroll track that the handle moves along.
 */
class ALPHAS_UI_PACK_DLL WindowsClassicTrack : public ScrollBarElement {
public:
    /**
     * @brief Constructs a new WindowsClassicTrack.
     */
    WindowsClassicTrack();

    /**
     * @brief Destroys the WindowsClassicTrack.
     */
    ~WindowsClassicTrack();

    /**
     * @brief Creates a new WindowsClassicTrack instance.
     * @return Pointer to the created WindowsClassicTrack.
     */
    static WindowsClassicTrack* create();

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
     * @brief Sets the content size of the track element.
     *
     * Overrides CCNode behavior.
     * @param contentSize Desired size.
     */
    virtual void setContentSize(const cocos2d::CCSize& contentSize) override;

    /**
     * @brief Called when the track is clicked.
     *
     * Can be used to handle page-up/page-down scrolling.
     * @param pos Click position relative to the track.
     */
    virtual void onClick(const cocos2d::CCPoint& pos) override;

    /**
     * @brief Called when the click is released.
     * @param pos Release position.
     */
    virtual void onRelease(const cocos2d::CCPoint& pos) override;

protected:
    /// Internal implementation details.
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}
