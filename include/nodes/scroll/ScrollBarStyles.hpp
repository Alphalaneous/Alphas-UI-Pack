#pragma once

#include "ScrollBarStyle.hpp"

#include "../../export.hpp"

namespace alpha::ui {

/**
 * @brief Scroll bar style resembling classic Windows scroll bars.
 *
 * Provides the appearance and behavior of traditional Windows scroll bars.
 */
struct ALPHAS_UI_PACK_DLL WindowsClassicScrollStyle : public ScrollBarStyle {
    /**
     * @brief Constructs a WindowsClassicScrollStyle.
     */
    WindowsClassicScrollStyle();
};

/**
 * @brief Scroll bar style with rounded edges.
 *
 * Provides a modern, rounded appearance for scroll bars.
 */
struct ALPHAS_UI_PACK_DLL RoundedScrollStyle : public ScrollBarStyle {
    /**
     * @brief Constructs a RoundedScrollStyle.
     */
    RoundedScrollStyle();
};

/**
 * @brief Basic scroll bar style with minimal visual features.
 *
 * Provides a simple, unstyled scroll bar suitable for lightweight UIs.
 */
struct ALPHAS_UI_PACK_DLL BasicScrollStyle : public ScrollBarStyle {
    /**
     * @brief Constructs a BasicScrollStyle.
     */
    BasicScrollStyle();
};

}
