#include "tomato/services/InputService.h"
#include <GLFW/glfw3.h>

namespace tomato
{
    void InputService::UpdateRecord(GLFWwindow* window, uint32_t tick)
    {
        curr_ = {tick, InputAction::NONE, InputAction::NONE};

        if (glfwGetKey(window, MOVE_KEYS[UP]) == GLFW_PRESS)
            curr_.keypress |= InputAction::UP;

        if (glfwGetKey(window, MOVE_KEYS[DOWN]) == GLFW_PRESS)
            curr_.keypress |= InputAction::DOWN;

        if (glfwGetKey(window, MOVE_KEYS[LEFT]) == GLFW_PRESS)
            curr_.keypress |= InputAction::LEFT;

        if (glfwGetKey(window, MOVE_KEYS[RIGHT]) == GLFW_PRESS)
            curr_.keypress |= InputAction::RIGHT;

        if (glfwGetKey(window, MOVE_KEYS[JUMP]) == GLFW_PRESS)
            curr_.keypress |= InputAction::JUMP;

        //curr_.keydown = static_cast<InputAction>((~prev_.keypress) & curr_.keypress);
        curr_.keydown = ~prev_.keypress & curr_.keypress;
    }
}