#pragma once

#include "ScrollBarStyle.hpp"

#include "../../export.hpp"

namespace alpha::ui {

struct ALPHAS_UI_PACK_DLL WindowsClassicScrollStyle : public ScrollBarStyle {
    WindowsClassicScrollStyle();
};

struct ALPHAS_UI_PACK_DLL RoundedScrollStyle : public ScrollBarStyle {
    RoundedScrollStyle();
};

struct ALPHAS_UI_PACK_DLL BasicScrollStyle : public ScrollBarStyle {
    BasicScrollStyle();
};

}