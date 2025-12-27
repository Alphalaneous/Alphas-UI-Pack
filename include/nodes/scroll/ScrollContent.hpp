#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>

#include "../../export.hpp"

namespace alpha::ui {

class AdvancedScrollLayer;

/**
 * @brief Node representing the scrollable content inside an AdvancedScrollLayer.
 *
 * ScrollContent is managed by AdvancedScrollLayer and should not have its
 * position or anchor modified directly. It acts as a container for scrollable
 * child nodes.
 */
class ALPHAS_UI_PACK_DLL ScrollContent : public cocos2d::CCNode {
public:
    /**
     * @brief Constructs a new ScrollContent node.
     */
    ScrollContent();

    /**
     * @brief Destroys the ScrollContent node.
     */
    ~ScrollContent();

    /**
     * @brief Creates a ScrollContent instance associated with the given scroll layer.
     *
     * @param scrollLayer AdvancedScrollLayer that will manage this content.
     * @return Newly created ScrollContent instance.
     */
    static ScrollContent* create(AdvancedScrollLayer* scrollLayer);

private:
    /**
     * @brief Initializes the ScrollContent with a scroll layer.
     *
     * @param scrollLayer AdvancedScrollLayer that will manage this content.
     * @return True if initialization succeeds; false otherwise.
     */
    bool init(AdvancedScrollLayer* scrollLayer);

    /**
     * @brief Returns the scroll layer managing this content.
     *
     * @return Pointer to the associated AdvancedScrollLayer.
     */
    AdvancedScrollLayer* getScrollLayer();

    // Prevent these methods from being used directly:
    virtual void setAnchorPoint(const cocos2d::CCPoint& anchor) override;
    virtual void ignoreAnchorPointForPosition(bool ignore) override;
    virtual void setPosition(const cocos2d::CCPoint& position) override;
    virtual void setPositionX(float x) override;
    virtual void setPositionY(float y) override;

protected:
    /// Internal implementation details.
    struct Impl;
    std::unique_ptr<Impl> m_impl; 

    /// AdvancedScrollLayer can access internal members.
    friend class AdvancedScrollLayer;
};

}
