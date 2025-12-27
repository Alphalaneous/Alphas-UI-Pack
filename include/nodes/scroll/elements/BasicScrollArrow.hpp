#pragma once

#include <Geode/cocos/sprite_nodes/CCSprite.h>
#include "../ScrollArrowElement.hpp"
#include "../../../API.hpp"
#include "ccTypes.h"

namespace alpha::ui {

/**
 * @brief Arrow element for a basic-style scroll bar.
 *
 * Represents the clickable arrow at either end of the scroll bar.
 */
class ALPHAS_UI_PACK_DLL BasicScrollArrow : public ScrollArrowElement {
public:
    /**
     * @brief Constructs a new BasicScrollArrow.
     */
    BasicScrollArrow();

    /**
     * @brief Destroys the BasicScrollArrow.
     */
    ~BasicScrollArrow();

    /**
     * @brief Creates a new BasicScrollArrow instance.
     * @return Pointer to the created BasicScrollArrow.
     */
    static BasicScrollArrow* create();

    /**
     * @brief Initializes the arrow element.
     * @return True if initialization succeeded.
     */
    bool init() override;

    /**
     * @brief Sets the color of the arrow.
     * @param color Color to apply.
     */
    void setArrowColor(const cocos2d::ccColor4B& color);

    /**
     * @brief Sets the color when the arrow is clicked.
     * @param color Click color.
     */
    void setClickColor(const cocos2d::ccColor4B& color);

    /**
     * @brief Returns the current arrow color.
     * @return Arrow color.
     */
    cocos2d::ccColor4B getArrowColor();

    /**
     * @brief Returns the current click color.
     * @return Click color.
     */
    cocos2d::ccColor4B getClickColor();

    /**
     * @brief Sets the content size of the arrow element.
     * @param contentSize Desired size.
     */
    virtual void setContentSize(const cocos2d::CCSize& contentSize) override;

    /**
     * @brief Called when the arrow is clicked.
     * @param pos Click position relative to the arrow.
     */
    virtual void onClick(const cocos2d::CCPoint& pos) override;

    /**
     * @brief Called when the click is released.
     * @param pos Release position relative to the arrow.
     */
    virtual void onRelease(const cocos2d::CCPoint& pos) override;

    /**
     * @brief Sets the arrow direction (UP or DOWN).
     * @param direction Arrow direction.
     */
    virtual void setDirection(ArrowDirection direction) override;

protected:
    /// Internal implementation details.
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}
