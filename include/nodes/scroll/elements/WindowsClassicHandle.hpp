#pragma once

#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include "../ScrollBarElement.hpp"
#include "../../../export.hpp"

namespace alpha::ui {

/**
 * @brief Handle element for a Windows Classic style scroll bar.
 *
 * Represents the draggable handle that moves along the scroll track.
 */
class ALPHAS_UI_PACK_DLL WindowsClassicHandle : public ScrollBarElement {
public:
    /**
     * @brief Constructs a new WindowsClassicHandle.
     */
    WindowsClassicHandle();

    /**
     * @brief Destroys the WindowsClassicHandle.
     */
    ~WindowsClassicHandle();

    /**
     * @brief Creates a new WindowsClassicHandle instance.
     * @return Pointer to the created WindowsClassicHandle.
     */
    static WindowsClassicHandle* create();

    /**
     * @brief Initializes the handle element.
     * @return True if initialization succeeded.
     */
    bool init() override;

    /**
     * @brief Sets the background color of the handle.
     * @param color Color to apply to the handle.
     */
    void setBackgroundColor(const cocos2d::ccColor4B& color);

    /**
     * @brief Sets the content size of the handle.
     *
     * Overrides CCNode behavior.
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
     * @param pos Release position.
     */
    virtual void onRelease(const cocos2d::CCPoint& pos) override;

    /**
     * @brief Sets the scroll bar this handle belongs to.
     * @param scrollBar Target AdvancedScrollBar.
     */
    virtual void setScrollBar(AdvancedScrollBar* scrollBar) override;

protected:
    /// Internal implementation details.
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}
