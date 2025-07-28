#pragma once
#include <functional>
#include <list>
#include <utility>


namespace gl3::engine::events {
    /// Represents an event that listeners can subscribe to.
    /// Events have an owner and an arbitrary number of arguments that are passes to the listeners when invoked.
    /// Only the owner is allowed to invoke the event.
    template<typename Owner, typename ...Args>
    class Event {
        friend Owner;

    public:
        using listener_t = typename std::function<void(Args...)>;
        using container_t = typename std::list<listener_t>;
        using handle_t = typename container_t::iterator;

        /// Adds an event listener. All registered event listeners are called when the event is invoked.
        /// @param listener An event listener. Its function signature is defined by the @a listener_t.
        /// @return Returns a handle you can pass to the @a removeListener function use to remove the listener.
        handle_t addListener(listener_t listener) {
            listeners.push_back(listener);
            return --listeners.end();
        }

        /// Removes an event listener. Removed event listeners are no longer called when the event is invoked.
        /// @param handle The handle returned by the @a addListener function.
        void removeListener(handle_t handle) {
            listeners.erase(handle);
        }

    private:
        void invoke(Args ...args) {
            container_t unmodifiedCallbacks(listeners);
            for(auto &callback: unmodifiedCallbacks) {
                callback(std::forward<Args>(args) ...);
            }
        }

        container_t listeners;
    };
}