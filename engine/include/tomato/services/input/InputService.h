#ifndef TOMATO_INPUTSERVICE_H
#define TOMATO_INPUTSERVICE_H

#include <queue>
#include <vector>
#include "tomato/input/InputTypes.h"    // KeyEvent

namespace tomato
{
    class WindowService;

    /**
     * @brief Translates raw device input into engine-level input actions.
     *
     * 하드웨어로부터 받은 원시 입력 값을 엔진에서 사용할 수 있도록 의미 있는 입력 값으로 변환한다.
     */
    class InputService
    {
    public:
        explicit InputService(WindowService& window);

        static Key ConvertGLFWtoTomato(int glfwKey);

        void DrainKeyEvents(std::vector<KeyEvent>& out);

    private:
        void SetCallback(WindowService& window);

        std::queue<KeyEvent> keyEvents_;
    };
}


#endif //TOMATO_INPUTSERVICE_H
