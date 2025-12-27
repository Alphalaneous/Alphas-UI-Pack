#import <Cocoa/Cocoa.h>
#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

void CursorManager::init() {
    m_cursors[Cursor::ARROW]       = (__bridge void*)[NSCursor arrowCursor];
    m_cursors[Cursor::TEXT]        = (__bridge void*)[NSCursor IBeamCursor];
    m_cursors[Cursor::CROSS]       = (__bridge void*)[NSCursor crosshairCursor];
    m_cursors[Cursor::HAND]        = (__bridge void*)[NSCursor pointingHandCursor];
    m_cursors[Cursor::NOT_ALLOWED] = (__bridge void*)[NSCursor operationNotAllowedCursor];
    m_cursors[Cursor::SIZE_WE]     = (__bridge void*)[NSCursor resizeLeftRightCursor];
    m_cursors[Cursor::SIZE_NS]     = (__bridge void*)[NSCursor resizeUpDownCursor];
}

void CursorManager::setCursor(Cursor cursor) {
    m_currentCursor = cursor;
    if (auto it = m_cursors.find(cursor); it != m_cursors.end()) {
        NSCursor* cursor = (__bridge NSCursor*)it->second;
        [cursor set];
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
        NSCursor* cursor = (__bridge NSCursor*)it->second;
        [cursor set];
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