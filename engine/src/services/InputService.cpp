#include "tomato/services/InputService.h"
#include "tomato/services/WindowService.h"
#include <GLFW/glfw3.h>

namespace tomato
{
    static std::array<Binding, static_cast<size_t>(KeyboardBinding::LAST)> KEYBOARD_BINDING_TABLE
    {{
         {GLFW_KEY_W, InputAction::UP},
         {GLFW_KEY_A, InputAction::LEFT},
         {GLFW_KEY_S, InputAction::DOWN},
         {GLFW_KEY_D, InputAction::RIGHT},
         {GLFW_KEY_SPACE, InputAction::JUMP}
    }};

    void InputService::BindAction(KeyboardBinding bindAction, int physicalKey)
    {
        KEYBOARD_BINDING_TABLE[static_cast<size_t>(bindAction)].key = physicalKey;
    }

    void InputService::UpdateRecord(WindowService& window, uint32_t tick)
    {
        curr_ = {tick, InputAction::NONE, InputAction::NONE};

        for (const auto& input : KEYBOARD_BINDING_TABLE) {
            if (IsPressed(window, input.key))
                curr_.key |= input.ia;
        }

        curr_.keydown = ~prev_.key & curr_.key;
    }

    bool InputService::IsPressed(WindowService& window, int key)
    {
        return glfwGetKey(window.GetHandle(), key) == GLFW_PRESS;
    }
}