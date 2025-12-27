#import <Cocoa/Cocoa.h>
#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

void CursorManager::init() {
    m_cursors[Cursor::ARROW]       = [NSCursor arrowCursor];
    m_cursors[Cursor::TEXT]        = [NSCursor IBeamCursor];
    m_cursors[Cursor::CROSS]       = [NSCursor crosshairCursor];
    m_cursors[Cursor::HAND]        = [NSCursor pointingHandCursor];
    m_cursors[Cursor::NOT_ALLOWED] = [NSCursor operationNotAllowedCursor];
    m_cursors[Cursor::SIZE_WE]     = [NSCursor resizeLeftRightCursor];
    m_cursors[Cursor::SIZE_NS]     = [NSCursor resizeUpDownCursor];
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