#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>

#include "../../export.hpp"

namespace alpha::ui {

class AdvancedScrollLayer;

class ALPHAS_UI_PACK_DLL ScrollContent : public cocos2d::CCNode {
public:
    ScrollContent();
    ~ScrollContent();

    static ScrollContent* create(AdvancedScrollLayer* scrollLayer);
private:
    bool init(AdvancedScrollLayer* scrollLayer);
    AdvancedScrollLayer* getScrollLayer();

    // prevent these from being used directly
    virtual void setAnchorPoint(const cocos2d::CCPoint& anchor) override;
    virtual void ignoreAnchorPointForPosition(bool ignore) override;
    virtual void setPosition(const cocos2d::CCPoint& position) override;
    virtual void setPositionX(float x) override;
    virtual void setPositionY(float y) override;

protected:
    struct Impl;
    std::unique_ptr<Impl> m_impl; 

    friend class AdvancedScrollLayer;
};

}