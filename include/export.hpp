#pragma once

#ifdef GEODE_IS_WINDOWS
    #ifdef ALPHALANEOUS_UI_PACK_EXPORTING
        #define ALPHAS_UI_PACK_DLL __declspec(dllexport)
    #else
        #define ALPHAS_UI_PACK_DLL __declspec(dllimport)
    #endif
    #else
        #define ALPHAS_UI_PACK_DLL __attribute__((visibility("default")))
#endif