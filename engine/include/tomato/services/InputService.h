#ifndef TOMATO_INPUTSERVICE_H
#define TOMATO_INPUTSERVICE_H

#include <queue>
#include <vector>
#include "tomato/input/InputTypes.h"    // KeyEvent

struct GLFWwindow;

namespace tomato
{
    class WindowService;

    /**
     * @brief Translates raw platform input into engine-level input actions.
     *
     * 플랫폼 레이어에서 전달된 원시 입력 값을 엔진이 사용하는 의미 있는 입력 값으로 변환한다.
     */
    class InputService
    {
    public:
        explicit InputService(WindowService& window);

        static Key ConvertKeyGLFW(int glfwKey);
        static KeyAction ConvertActionGLFW(int glfwAction);

        void DrainKeyEvents(std::vector<KeyEvent>& out);

    private:
        static void EnqueueKeyEvent(GLFWwindow* w, int key, int scancode, int action, int mods);

        std::queue<KeyEvent> keyEvents_;
    };
}


#endif //TOMATO_INPUTSERVICE_H
