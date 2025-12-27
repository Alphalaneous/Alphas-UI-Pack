#pragma once

#include <Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h>
#include "../ScrollBarElement.hpp"
#include "../../../export.hpp"

namespace alpha::ui {

/**
 * @brief Handle element for a basic-style scroll bar.
 *
 * Represents the draggable handle that moves along the scroll track.
 */
class ALPHAS_UI_PACK_DLL BasicScrollHandle : public ScrollBarElement {
public:
    /**
     * @brief Constructs a new BasicScrollHandle.
     */
    BasicScrollHandle();

    /**
     * @brief Destroys the BasicScrollHandle.
     */
    ~BasicScrollHandle();

    /**
     * @brief Creates a new BasicScrollHandle instance.
     * @return Pointer to the created BasicScrollHandle.
     */
    static BasicScrollHandle* create();

    /**
     * @brief Initializes the handle element.
     * @return True if initialization succeeded.
     */
    bool init() override;

    /**
     * @brief Sets the background color of the handle.
     * @param color Color to apply.
     */
    void setBackgroundColor(const cocos2d::ccColor4B& color);

    /**
     * @brief Sets the color when the handle is hovered.
     * @param color Hover color.
     */
    void setHoverColor(const cocos2d::ccColor4B& color);

    /**
     * @brief Sets the color when the handle is clicked.
     * @param color Click color.
     */
    void setClickColor(const cocos2d::ccColor4B& color);

    /**
     * @brief Returns the background color of the handle.
     * @return Background color.
     */
    cocos2d::ccColor4B getBackgroundColor();

    /**
     * @brief Returns the hover color of the handle.
     * @return Hover color.
     */
    cocos2d::ccColor4B getHoverColor();

    /**
     * @brief Returns the click color of the handle.
     * @return Click color.
     */
    cocos2d::ccColor4B getClickColor();

    /**
     * @brief Sets the content size of the handle.
     * @param contentSize Desired size.
     */
    virtual void setContentSize(const cocos2d::CCSize& contentSize) override;

    /**
     * @brief Called when the handle is clicked.
     * @param pos Click position relative to the handle.
     */
    virtual void onClick(const cocos2d::CCPoint& pos) override;

    /**
     * @brief Called when the click is released.
     * @param pos Release position relative to the handle.
     */
    virtual void onRelease(const cocos2d::CCPoint& pos) override;

    /**
     * @brief Called when the mouse enters the handle.
     * @param pos Position relative to the handle.
     */
    virtual void onMouseEnter(const cocos2d::CCPoint& pos) override;

    /**
     * @brief Called when the mouse exits the handle.
     * @param pos Position relative to the handle.
     */
    virtual void onMouseExit(const cocos2d::CCPoint& pos) override;

protected:
    /// Internal implementation details.
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}
