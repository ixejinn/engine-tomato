#include "tomato/input/InputRecorder.h"
#include "tomato/services/InputService.h" // KeyEvent
#include "tomato/tomato_math.h"

namespace tomato
{
    InputRecorder::InputRecorder()
    {
        InitKeyActionMap();
    }

    void InputRecorder::UpdateInputAxis(std::vector<KeyEvent>& events, uint32_t tick)
    {
        prev_ = curr_;
        curr_.tick = tick;

        for (const auto& event : events)
        {
            if (event.consumed)
                continue;

            keyStates_[event.key].value = event.value;

            if (event.action == KeyAction::RELEASE)
                curr_.held &= ~keyIntents_[event.key];
            else             // KeyAction::PRESS
                curr_.held |= keyIntents_[event.key];
        }
        curr_.down = ~prev_.held & curr_.held;
    }

    void InputRecorder::InitKeyActionMap()
    {
        keyIntents_[Key::W] = InputIntent::UP;
        keyIntents_[Key::S] = InputIntent::DOWN;
        keyIntents_[Key::A] = InputIntent::LEFT;
        keyIntents_[Key::D] = InputIntent::RIGHT;
        keyIntents_[Key::SpaceBar] = InputIntent::JUMP;
    }
}