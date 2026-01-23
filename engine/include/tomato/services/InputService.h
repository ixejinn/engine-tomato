#ifndef TOMATO_INPUTSERVICE_H
#define TOMATO_INPUTSERVICE_H

#include <cstdint>
#include <array>
#include "tomato/services/InputTypes.h"    // InputRecord prev_, curr_

namespace tomato
{
    class WindowService;

    /// Logical input binding slots used as indices into the input binding table.
    enum class KeyboardBinding : uint16_t
    {
        UP,
        LEFT,
        DOWN,
        RIGHT,
        JUMP,
        LAST
    };

    /// Struct for binding physical key to input action.
    struct Binding
    {
        int key;
        InputAction ia;
    };

    /**
     * @brief Translates raw device input into engine-level input actions.
     *
     * 하드웨어로부터 받은 원시 입력 값을 엔진에서 사용할 수 있도록 의미 있는 입력 값으로 변환한다.
     */
    class InputService
    {
    public:
        static void BindAction(KeyboardBinding bindAction, int physicalKey);

        /// Initializes previous input record to current per frame to detect key down edge.
        void BeginFrame() { prev_ = curr_; }

        /// Updates the current input record from the latest device state.
        void UpdateRecord(WindowService& window, uint32_t tick);

        const InputRecord& GetCurrInputRecord() const { return curr_; }

    private:
        static bool IsPressed(WindowService& window, int key) ;

        InputRecord prev_;
        InputRecord curr_;
    };
}


#endif //TOMATO_INPUTSERVICE_H
