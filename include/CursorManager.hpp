#pragma once

#include <Geode/cocos/cocoa/CCObject.h>

#include "export.hpp"

namespace alpha::ui {

/**
 * @brief Types of available cursors.
 * @note SIZE, SIZE_NWSE, and SIZE_NESW are ignored on macOS.
 */
enum class Cursor {
    /// No cursor is visible.
    NONE,

    /// Standard arrow cursor.
    ARROW,

    /// Text selection cursor.
    TEXT,

    /// Precision crosshair cursor.
    CROSS,

    /// Resize cursor in all directions (Windows only).
    SIZE,

    /// Resize cursor for northwest–southeast directions (Windows only).
    SIZE_NWSE,

    /// Resize cursor for northeast–southwest directions (Windows only).
    SIZE_NESW,

    /// Resize cursor for west–east directions.
    SIZE_WE,

    /// Resize cursor for north–south directions.
    SIZE_NS,

    /// Operation not allowed cursor.
    NOT_ALLOWED,

    /// Pointing hand cursor.
    HAND
};

/**
 * @brief Manages the application mouse cursor.
 */
class ALPHAS_UI_PACK_DLL CursorManager : public cocos2d::CCObject {
public:
    /**
    * @brief Returns the global CursorManager instance.
    * 
    * @return Pointer to the global CursorManager instance.
    */
    static CursorManager* get();

    /**
     * @brief Sets the mouse cursor icon.
     * 
     * @param cursor Cursor type to apply.
     * @note Unsupported cursor types are ignored on specific platforms.
     */
    void setCursor(Cursor cursor);

    /**
     * @brief Restores the cursor to the last requested state.
     * @note Intended for internal use.
     */
    void resetCursor();
protected:
    void init();
    void update(float dt);
    bool isMouseInWindow();
    
    Cursor m_currentCursor = Cursor::ARROW;

#if defined(GEODE_IS_WINDOWS)
public:
protected:
    std::unordered_map<Cursor, HCURSOR> m_cursors;
#elif defined(GEODE_IS_MACOS)
    bool m_hidden = false;
    std::unordered_map<Cursor, void*> m_cursors;
#endif
};

}