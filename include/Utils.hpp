#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>
#include "export.hpp"

namespace alpha::utils {

/// Checks if a point is inside a node.
/// @param point The coordinate in world space.
ALPHAS_UI_PACK_DLL bool isPointInsideNode(cocos2d::CCNode* node, const cocos2d::CCPoint& point);

/// Returns the current time in seconds.
ALPHAS_UI_PACK_DLL double nowSeconds();

/// Get a node's rect in world space, useful for culling methods.
/// @param multiplier A multiplier for the rect.
ALPHAS_UI_PACK_DLL cocos2d::CCRect rectToWorld(cocos2d::CCNode* node, float multiplier = 1.f);

/// Rotates Padding clockwise.
ALPHAS_UI_PACK_DLL geode::Padding rotatePaddingCW(const geode::Padding& padding);

/// Rotates Insets counter-clockwise.
ALPHAS_UI_PACK_DLL geode::Padding rotatePaddingCCW(const geode::Padding& padding);

ALPHAS_UI_PACK_DLL bool canTouch(cocos2d::CCNode* node, const cocos2d::CCPoint& pos);

#ifdef GEODE_IS_WINDOWS
/// Easily acquire the HWND of the Geometry Dash window.
ALPHAS_UI_PACK_DLL HWND getHWND();
#endif

}