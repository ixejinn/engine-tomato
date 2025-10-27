#ifndef TOMATO_INPUTSERVICE_H
#define TOMATO_INPUTSERVICE_H

#include <cstdint>
#include <array>
#include <GLFW/glfw3.h>
#include "tomato/services/InputTypes.h"    // InputRecord prev_, curr_

namespace tomato
{
    enum Move : uint8_t
    {
        UP = 0,
        DOWN,
        LEFT,
        RIGHT,
        JUMP
    };

    class InputService
    {
    public:
        const InputRecord& GetCurrInputRecord() const { return curr_; }

        // for edge key event pressed & released
        void BeginFrame() { prev_ = curr_; }

        void UpdateRecord(GLFWwindow* window, uint32_t tick);

    private:
        static constexpr std::array<int, 5> MOVE_KEYS{GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE};

        InputRecord prev_;
        InputRecord curr_;
    };
}

#endif //TOMATO_INPUTSERVICE_H
