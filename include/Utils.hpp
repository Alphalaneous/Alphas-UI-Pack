#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>
#include "nodes/scroll/ScrollBarStyle.hpp"

#include "export.hpp"

namespace alpha::utils {


/**
 * @brief Checks if a point is inside a node.
 *
 * @param node The node in any coordinate space.
 * @param point The coordinate in world space.
 * @return True if the point is inside the node; false otherwise.
 */
ALPHAS_UI_PACK_DLL bool isPointInsideNode(cocos2d::CCNode* node, const cocos2d::CCPoint& point);

/**
 * @brief Returns the current time in seconds.
 * 
 * @return The current time in seconds as a double.
 */
ALPHAS_UI_PACK_DLL double nowSeconds();

/**
 * @brief Get a node's rect in world space, useful for culling methods.
 * 
 * @param node The node in any coordinate space.
 * @param multiplier A multiplier for the rect.
 * @return Returns a node's rect in world space.
 */
ALPHAS_UI_PACK_DLL cocos2d::CCRect rectToWorld(cocos2d::CCNode* node, float multiplier = 1.f);

/**
 * @brief Rotates Insets clockwise.
 * 
 * @param insets The non-rotated Insets.
 * @return The rotated Insets.
 */
ALPHAS_UI_PACK_DLL alpha::ui::Insets rotateInsetsCW(const alpha::ui::Insets& insets);

/**
 * @brief Rotates Insets counter-clockwise.
 * 
 * @param insets The non-rotated Insets.
 * @return The rotated Insets.
 */
ALPHAS_UI_PACK_DLL alpha::ui::Insets rotateInsetsCCW(const alpha::ui::Insets& insets);

#ifdef GEODE_IS_WINDOWS
/**
 * @brief Easily acquire the HWND of the Geometry Dash window.
 * 
 * @return The HWND of the Geometry Dash window.
 */
ALPHAS_UI_PACK_DLL HWND getHWND();
#endif

}