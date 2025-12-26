#pragma once

#include <Geode/cocos/cocoa/CCObject.h>

#include "API.hpp"

enum class Cursor {
    NONE,
    ARROW,
    TEXT,
    CROSS,
    SIZE,
    SIZE_NWSE,
    SIZE_NESW,
    SIZE_WE,
    SIZE_NS,
    NO,
    HAND
};

class ALPHAS_UI_PACK_DLL CursorManager : public cocos2d::CCObject {
public:
    static CursorManager* get();

    void init();
    void setCursor(Cursor cursor);
    void resetCursor();
    bool isMouseInWindow();
    void update(float dt);
protected:
    Cursor m_currentCursor = Cursor::ARROW;

#if defined(GEODE_IS_WINDOWS)
public:
    static void setWndProc();
    static LONG_PTR oWindowProc;
protected:
    std::unordered_map<Cursor, HCURSOR> m_cursors;

#elif defined(GEODE_IS_MACOS)
#import <Cocoa/Cocoa.h>    
    bool m_hidden = false;
    std::unordered_map<Cursor, NSCursor*> m_cursors;
#endif
};