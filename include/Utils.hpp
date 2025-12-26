#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>
#include "nodes/scroll/ScrollBarStyle.hpp"

#include "export.hpp"

namespace alpha::utils {

ALPHAS_UI_PACK_DLL bool isTouchInsideNode(cocos2d::CCNode* node, const cocos2d::CCPoint& touch);
ALPHAS_UI_PACK_DLL double nowSeconds();
ALPHAS_UI_PACK_DLL cocos2d::CCRect rectToWorld(cocos2d::CCNode* node, float multiplier = 1.f);
ALPHAS_UI_PACK_DLL alpha::ui::Insets rotateInsetsCW(const alpha::ui::Insets& insets);
ALPHAS_UI_PACK_DLL alpha::ui::Insets rotateInsetsCCW(const alpha::ui::Insets& insets);

#ifdef GEODE_IS_WINDOWS
ALPHAS_UI_PACK_DLL HWND getHWND();
#endif

}