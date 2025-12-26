#ifdef GEODE_IS_MACOS

#include <Geode/Geode.hpp>
#include "CursorManager.hpp"

using namespace geode::prelude;

void CursorManager::init() {
    m_cursors[Cursor::ARROW]   = [NSCursor arrowCursor];
    m_cursors[Cursor::TEXT]    = [NSCursor IBeamCursor];
    m_cursors[Cursor::CROSS]   = [NSCursor crosshairCursor];
    m_cursors[Cursor::HAND]    = [NSCursor pointingHandCursor];
    m_cursors[Cursor::NO]      = [NSCursor operationNotAllowedCursor];
    m_cursors[Cursor::SIZE_WE] = [NSCursor resizeLeftRightCursor];
    m_cursors[Cursor::SIZE_NS] = [NSCursor resizeUpDownCursor];
}

void CursorManager::setCursor(Cursor cursor) {
    m_currentCursor = cursor;
    if (auto it = m_cursors.find(cursor); it != m_cursors.end()) {
        [it->second set];
    }
}

void CursorManager::resetCursor() {
    if (m_currentCursor == Cursor::NONE) {
        if (!m_hidden) {
            [NSCursor hide];
            m_hidden = true;
        }
        return;
    }
    else {
        if (m_hidden) {
            [NSCursor unhide];
            m_hidden = false;
        }
    }
    if (m_currentCursor == Cursor::ARROW) return;

    if (auto it = m_cursors.find(m_currentCursor); it != m_cursors.end()) {
        [it->second set];
    }
}

bool CursorManager::isMouseInWindow() {
    NSWindow *window = [[CCEGLView sharedOpenGLView] window];
    if (![window isKeyWindow]) return false;

    NSPoint mouseLoc = [NSEvent mouseLocation];
    NSRect winFrame = [window frame];
    return NSPointInRect(mouseLoc, winFrame);
}

void CursorManager::update(float dt) {
    if (isMouseInWindow()) {
        resetCursor();
    }
    else {
        [[NSCursor arrowCursor] set];
        if (m_hidden) {
            m_hidden = false;
            [NSCursor unhide];
        }
    }
}

#endif