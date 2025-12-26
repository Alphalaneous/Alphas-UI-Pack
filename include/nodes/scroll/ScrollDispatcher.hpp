#pragma once

#include "../../export.hpp"

namespace alpha::dispatcher {

class AdvancedScrollDelegate;

class ALPHAS_UI_PACK_DLL ScrollDispatcher {
public:
    static ScrollDispatcher* get();

    ScrollDispatcher();
    ~ScrollDispatcher();

    void registerScroll(AdvancedScrollDelegate* delegate);
    void unregisterScroll(AdvancedScrollDelegate* delegate);

    const std::vector<AdvancedScrollDelegate*>& getDelegates();
private:
    static ScrollDispatcher* s_instance;

    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}