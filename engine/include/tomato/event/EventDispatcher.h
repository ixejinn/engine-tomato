#ifndef TOMATO_EVENTDISPATCHER_H
#define TOMATO_EVENTDISPATCHER_H

#include <entt/entt.hpp>
#include <memory>

namespace tomato
{
    class EventDispatcher
    {
    private:
        EventDispatcher()
        {
            dispatcher_ = std::make_unique<entt::dispatcher>();
        }

    public:
        static EventDispatcher& GetInstance()
        {
            static EventDispatcher instance;
            return instance;
        }

        ~EventDispatcher() = default;

        template<typename E, auto Candidate>
        void Connect()
        {
            dispatcher_->sink<E>().template connect<Candidate>();
        }

        template<typename E, auto Candidate, typename T>
        void Connect(T& instance)
        {
            dispatcher_->sink<E>().template connect<Candidate>(instance);
        }

        template<typename E>
        void Enqueue(const E& e)
        {
            dispatcher_->enqueue(e);
        }

        template<typename E>
        void Trigger(const E& e)
        {
            dispatcher_->trigger(e);
        }

        void Update()
        {
            dispatcher_->update();
        }

        template<typename E, auto Candidate>
        void Disconnect()
        {
            dispatcher_->sink<E>().template disconnect<Candidate>();
        }

        template<typename E, auto Candidate, typename T>
        void Disconnect(T& instance)
        {
            dispatcher_->sink<E>().template disconnect<Candidate>(instance);
        }

        void ClearEvents()
        {
            dispatcher_->clear();
        }

        void Clear()
        {
            dispatcher_ = std::make_unique<entt::dispatcher>();
        }

    private:
        std::unique_ptr<entt::dispatcher> dispatcher_;
    };
}

#endif //TOMATO_EVENTDISPATCHER_H
