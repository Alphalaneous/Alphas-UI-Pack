#pragma once

#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include <Geode/cocos/sprite_nodes/CCSprite.h>
#include "../ScrollArrowElement.hpp"
#include "../../../export.hpp"
#include "ccTypes.h"

namespace alpha::ui {

/**
 * @brief Arrow button element for Windows Classic style scroll bars.
 *
 * Represents the up/down arrow buttons that can incrementally scroll the content.
 */
class ALPHAS_UI_PACK_DLL WindowsClassicArrow : public ScrollArrowElement {
public:
    /**
     * @brief Constructs a new WindowsClassicArrow.
     */
    WindowsClassicArrow();

    /**
     * @brief Destroys the WindowsClassicArrow.
     */
    ~WindowsClassicArrow();

    /**
     * @brief Creates a new WindowsClassicArrow instance.
     * @return Pointer to the created WindowsClassicArrow.
     */
    static WindowsClassicArrow* create();

    /**
     * @brief Initializes the arrow element.
     * @return True if initialization succeeded.
     */
    bool init() override;

    /**
     * @brief Sets the background color of the arrow button.
     * @param color Color to apply to the background.
     */
    void setBackgroundColor(const cocos2d::ccColor4B& color);

    /**
     * @brief Sets the color of the arrow itself.
     * @param color Color to apply to the arrow.
     */
    void setArrowColor(const cocos2d::ccColor4B& color);

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
     * @brief Sets the direction of the arrow.
     * @param direction UP or DOWN.
     */
    virtual void setDirection(ArrowDirection direction) override;

    /**
     * @brief Sets the scroll bar this arrow belongs to.
     * @param scrollBar Target AdvancedScrollBar.
     */
    virtual void setScrollBar(AdvancedScrollBar* scrollBar) override;

protected:
    /// Internal implementation details.
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}
