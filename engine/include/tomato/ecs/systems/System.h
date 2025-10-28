#ifndef TOMATO_SYSTEM_H
#define TOMATO_SYSTEM_H

namespace tomato
{
    class Engine;
    struct SimContext;

    class System
    {
    private:
        bool isActive{true};

    public:
        virtual ~System() = default;

        virtual void Update(Engine& engine, const SimContext& ctx) = 0;

        // !!! FOR RENDERING ONLY - FUNCTION OVERLOADING !!!
        virtual void Update(const Engine& engine, const SimContext& ctx) {}
    };
}

#endif //TOMATO_SYSTEM_H
