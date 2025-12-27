#pragma once

#include "../../export.hpp"

namespace alpha::dispatcher {

class AdvancedScrollDelegate;

/**
 * @brief Singleton class for dispatching scroll events.
 *
 * Manages registered AdvancedScrollDelegate instances and
 * forwards scroll input to them.
 */
class ALPHAS_UI_PACK_DLL ScrollDispatcher {
public:
    /**
     * @brief Returns the global ScrollDispatcher instance.
     *
     * @return Pointer to the singleton ScrollDispatcher.
     */
    static ScrollDispatcher* get();

    /**
     * @brief Constructs a new ScrollDispatcher.
     *
     * Typically called internally; use get() to access the singleton.
     */
    ScrollDispatcher();

    /**
     * @brief Destroys the ScrollDispatcher.
     */
    ~ScrollDispatcher();

    /**
     * @brief Registers a delegate to receive scroll events.
     *
     * @param delegate Delegate to register.
     */
    void registerScroll(AdvancedScrollDelegate* delegate);

    /**
     * @brief Unregisters a previously registered delegate.
     *
     * @param delegate Delegate to unregister.
     */
    void unregisterScroll(AdvancedScrollDelegate* delegate);

    /**
     * @brief Returns all currently registered delegates.
     *
     * @return Vector of pointers to registered delegates.
     */
    const std::vector<AdvancedScrollDelegate*>& getDelegates();

private:
    /// Singleton instance.
    static ScrollDispatcher* s_instance;

    /// Internal implementation details.
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}
