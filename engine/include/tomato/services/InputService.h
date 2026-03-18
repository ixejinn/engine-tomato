#ifndef TOMATO_INPUTSERVICE_H
#define TOMATO_INPUTSERVICE_H

#include <queue>
#include <vector>
#include "tomato/input/InputTypes.h"    // KeyEvent

#include "tomato/event/EventSignal.h"

struct GLFWwindow;

namespace tomato
{
    class WindowService;
    class InputRecorder;

    /**
     * @brief Translates raw platform input into engine-level input actions.
     *
     * 플랫폼 레이어에서 전달된 원시 입력 값을 엔진이 사용하는 의미 있는 입력 값으로 변환한다.
     */
    class InputService
    {
    public:
        explicit InputService(WindowService& window, InputRecorder& recorder);

        static Key ConvertKeyGLFW(int glfwKey);
        static KeyAction ConvertActionGLFW(int glfwAction);

    private:
        static void OnKeyEvent(GLFWwindow* w, int key, int scancode, int action, int mods);
        static void OnMouseButtonEvent(GLFWwindow* w, int button, int action, int mods);

        EventSignal<KeyEvent> keySignal_;
        EventSignal<MouseEvent> mouseSignal_;
        struct InputEventCollector
        {
            bool operator()(int ret) { return !ret; }   // 리스너 함수에서 true가 반환되면 계속
        } collector;
    };
}


#endif //TOMATO_INPUTSERVICE_H
