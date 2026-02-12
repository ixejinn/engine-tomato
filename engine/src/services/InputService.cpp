#include "tomato/services/InputService.h"
#include "tomato/services/WindowService.h"
#include "tomato/Logger.h"

#include <GLFW/glfw3.h>

namespace tomato
{
    Key InputService::ConvertKeyGLFW(int glfwKey)
    {
        switch (glfwKey)
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                return Key::LeftMouseButton;
            case GLFW_MOUSE_BUTTON_RIGHT:
                return Key::RightMouseButton;

            case GLFW_KEY_ESCAPE:
                return Key::Escape;
            case GLFW_KEY_SPACE:
                return Key::SpaceBar;

            case GLFW_KEY_LEFT:
                return Key::Left;
            case GLFW_KEY_UP:
                return Key::Up;
            case GLFW_KEY_RIGHT:
                return Key::Right;
            case GLFW_KEY_DOWN:
                return Key::Down;

            case GLFW_KEY_0:
                return Key::K_0;
            case GLFW_KEY_1:
                return Key::K_1;
            case GLFW_KEY_2:
                return Key::K_2;
            case GLFW_KEY_3:
                return Key::K_3;
            case GLFW_KEY_4:
                return Key::K_4;
            case GLFW_KEY_5:
                return Key::K_5;
            case GLFW_KEY_6:
                return Key::K_6;
            case GLFW_KEY_7:
                return Key::K_7;
            case GLFW_KEY_8:
                return Key::K_8;
            case GLFW_KEY_9:
                return Key::K_9;

            case GLFW_KEY_A:
                return Key::A;
            case GLFW_KEY_B:
                return Key::B;
            case GLFW_KEY_C:
                return Key::C;
            case GLFW_KEY_D:
                return Key::D;
            case GLFW_KEY_E:
                return Key::E;
            case GLFW_KEY_F:
                return Key::F;
            case GLFW_KEY_G:
                return Key::G;
            case GLFW_KEY_H:
                return Key::H;
            case GLFW_KEY_I:
                return Key::I;
            case GLFW_KEY_J:
                return Key::J;
            case GLFW_KEY_K:
                return Key::K;
            case GLFW_KEY_L:
                return Key::L;
            case GLFW_KEY_M:
                return Key::M;
            case GLFW_KEY_N:
                return Key::N;
            case GLFW_KEY_O:
                return Key::O;
            case GLFW_KEY_P:
                return Key::P;
            case GLFW_KEY_Q:
                return Key::Q;
            case GLFW_KEY_R:
                return Key::R;
            case GLFW_KEY_S:
                return Key::S;
            case GLFW_KEY_T:
                return Key::T;
            case GLFW_KEY_U:
                return Key::U;
            case GLFW_KEY_V:
                return Key::V;
            case GLFW_KEY_W:
                return Key::W;
            case GLFW_KEY_X:
                return Key::X;
            case GLFW_KEY_Y:
                return Key::Y;
            case GLFW_KEY_Z:
                return Key::Z;

            case GLFW_KEY_KP_0:
                return Key::KP_0;
            case GLFW_KEY_KP_1:
                return Key::KP_1;
            case GLFW_KEY_KP_2:
                return Key::KP_2;
            case GLFW_KEY_KP_3:
                return Key::KP_3;
            case GLFW_KEY_KP_4:
                return Key::KP_4;
            case GLFW_KEY_KP_5:
                return Key::KP_5;
            case GLFW_KEY_KP_6:
                return Key::KP_6;
            case GLFW_KEY_KP_7:
                return Key::KP_7;
            case GLFW_KEY_KP_8:
                return Key::KP_8;
            case GLFW_KEY_KP_9:
                return Key::KP_9;

            default:
                return Key::UNKNOWN;
        }
    }

    KeyAction InputService::ConvertActionGLFW(int glfwAction)
    {
        switch (glfwAction)
        {
            case GLFW_RELEASE:
                return KeyAction::RELEASE;
            case GLFW_PRESS:
            case GLFW_REPEAT:
                return KeyAction::PRESS;
            default:
                return KeyAction::COUNT;
        }
    }

    void InputService::EnqueueKeyEvent(GLFWwindow* w, int key, int scancode, int action, int mods)
    {
        Key k = ConvertKeyGLFW(key);
        KeyAction a = ConvertActionGLFW(action);

        if (a >= KeyAction::COUNT)
        {
            TMT_WARN << "Invalid KeyAction " << action;
            return;
        }

        auto* input = static_cast<WindowData*>(glfwGetWindowUserPointer(w))->input;
        input->keyEvents_.emplace(k, a);
    }

    InputService::InputService(WindowService& window)
    {
        glfwSetKeyCallback(window.GetHandle(), EnqueueKeyEvent);
    }

    void InputService::DrainKeyEvents(std::vector<KeyEvent>& out)
    {
        while (!keyEvents_.empty())
        {
            out.push_back(keyEvents_.front());
            keyEvents_.pop();
        }
    }
}