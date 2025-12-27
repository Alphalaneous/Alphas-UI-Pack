#include <Geode/Geode.hpp>
#include "Utils.hpp"

bool alpha::utils::isPointInsideNode(cocos2d::CCNode* node, const cocos2d::CCPoint& touch) {
    if (!node) return false;

    cocos2d::CCPoint local = node->convertToNodeSpace(touch);
    auto rect = cocos2d::CCRect(0, 0, node->getContentWidth(), node->getContentHeight());

    return rect.containsPoint(local);
}

double alpha::utils::nowSeconds() {
    using clock = std::chrono::steady_clock;
    return std::chrono::duration<double>(clock::now().time_since_epoch()).count();
}

cocos2d::CCRect alpha::utils::rectToWorld(cocos2d::CCNode* node, float multiplier) {
    const auto baseSize = node->getContentSize();
    const auto anchor = node->getAnchorPoint();

    auto anchorRelative = cocos2d::CCPoint{baseSize.width * anchor.x, baseSize.height * anchor.y};

    auto scaled = cocos2d::CCSize{baseSize.width * multiplier,baseSize.height * multiplier};

    auto bl = anchorRelative + cocos2d::CCPoint(-scaled.width * anchor.x, -scaled.height * anchor.y);
    auto br = anchorRelative + cocos2d::CCPoint(scaled.width * (1 - anchor.x), -scaled.height * anchor.y);
    auto tl = anchorRelative + cocos2d::CCPoint(-scaled.width * anchor.x, scaled.height * (1 - anchor.y));
    auto tr = anchorRelative + cocos2d::CCPoint(scaled.width * (1 - anchor.x), scaled.height * (1 - anchor.y));

    const auto t = node->nodeToWorldTransform();

    bl = CCPointApplyAffineTransform(bl, t);
    br = CCPointApplyAffineTransform(br, t);
    tl = CCPointApplyAffineTransform(tl, t);
    tr = CCPointApplyAffineTransform(tr, t);

    float minX = std::min(std::min(bl.x, br.x), std::min(tl.x, tr.x));
    float maxX = std::max(std::max(bl.x, br.x), std::max(tl.x, tr.x));
    float minY = std::min(std::min(bl.y, br.y), std::min(tl.y, tr.y));
    float maxY = std::max(std::max(bl.y, br.y), std::max(tl.y, tr.y));

    return cocos2d::CCRect(minX, minY, maxX - minX, maxY - minY);
}

alpha::ui::Insets alpha::utils::rotateInsetsCW(const alpha::ui::Insets& insets) {
    return {
        insets.bottom,
        insets.top,
        insets.left,
        insets.right
    };
}

alpha::ui::Insets alpha::utils::rotateInsetsCCW(const alpha::ui::Insets& insets) {
    return {
        insets.top,
        insets.bottom,
        insets.right,
        insets.left
    };
}

#ifdef GEODE_IS_WINDOWS
HWND alpha::utils::getHWND() {
    return WindowFromDC(wglGetCurrentDC());
}
#endif