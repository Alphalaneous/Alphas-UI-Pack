#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

CursorManager* CursorManager::get() {
    static CursorManager instance;
    static bool initialized = false;
    if (!initialized) {
        instance.init();
        initialized = true;
    }
    return &instance;
}

bool CursorManager::isMouseInWindow() {
    auto mousePos = getMousePos();
    if (auto scene = CCScene::get()) {
        auto sceneBox = scene->boundingBox();
        return sceneBox.containsPoint(mousePos);
    }

    return false;
}

#ifdef GEODE_IS_DESKTOP
$execute {
	CCScheduler::get()->scheduleUpdateForTarget(CursorManager::get(), INT_MIN, false);
}
#endif

#ifdef GEODE_IS_WINDOWS
#include <Geode/modify/CCEGLView.hpp>

class $modify(CCEGLView) {
    void setupWindow(cocos2d::CCRect rect) {
        CCEGLView::setupWindow(rect);
        CursorManager::setWndProc();
    }
};

LONG_PTR CursorManager::oWindowProc;

LRESULT CALLBACK nWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    auto ret = CallWindowProc((WNDPROC)CursorManager::oWindowProc, hwnd, msg, wparam, lparam);
	CursorManager::get()->resetCursor();
	return ret;
}

void CursorManager::setWndProc() {
    oWindowProc = SetWindowLongPtrA(alpha::utils::getHWND(), -4, (LONG_PTR)nWindowProc);
}

void CursorManager::init() {
    m_cursors[Cursor::ARROW]       = LoadCursor(NULL, IDC_ARROW);
    m_cursors[Cursor::TEXT]        = LoadCursor(NULL, IDC_IBEAM);
    m_cursors[Cursor::CROSS]       = LoadCursor(NULL, IDC_CROSS);
    m_cursors[Cursor::SIZE]        = LoadCursor(NULL, IDC_SIZEALL);
    m_cursors[Cursor::SIZE_NWSE]   = LoadCursor(NULL, IDC_SIZENWSE);
    m_cursors[Cursor::SIZE_NESW]   = LoadCursor(NULL, IDC_SIZENESW);
    m_cursors[Cursor::SIZE_WE]     = LoadCursor(NULL, IDC_SIZEWE);
    m_cursors[Cursor::SIZE_NS]     = LoadCursor(NULL, IDC_SIZENS);
    m_cursors[Cursor::NOT_ALLOWED] = LoadCursor(NULL, IDC_NO);
    m_cursors[Cursor::HAND]        = LoadCursor(NULL, IDC_HAND);
}

void CursorManager::setCursor(Cursor cursor) {
    m_currentCursor = cursor;
    SetCursor(m_cursors[m_currentCursor]);
}

void CursorManager::resetCursor() {
    if (m_currentCursor == Cursor::ARROW) return;
    if (m_currentCursor == Cursor::NONE) {
        ShowCursor(false);
        return;
    }
    ShowCursor(true);
    SetCursor(m_cursors[m_currentCursor]);
}

void CursorManager::update(float dt) {
    if (isMouseInWindow()) {
        resetCursor();
    }
    else {
        SetCursor(m_cursors[Cursor::ARROW]);
        ShowCursor(true);
    }
}

#endif

#ifndef GEODE_IS_DESKTOP
void CursorManager::init() {}

void CursorManager::setCursor(Cursor cursor) {
    log::debug("setting cursors is not supported on mobile");
}

void CursorManager::resetCursor() {}

void CursorManager::update(float dt) {

}

#endif