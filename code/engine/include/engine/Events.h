#pragma once
#include <functional>
#include <list>
#include <utility>


namespace gl3::engine::events {
    template<typename Owner, typename ...Args>
    class Event {
        friend Owner;

    public:
        using listener_t = typename std::function<void(Args...)>;
        using container_t = typename std::list<listener_t>;
        using handle_t = typename container_t::iterator;

        handle_t addListener(listener_t listener) {
            listeners.push_back(listener);
            return --listeners.end();
        }

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