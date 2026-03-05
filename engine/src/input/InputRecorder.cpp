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
        if (curr_.tick != tick)
        {
            prev_ = curr_;
            curr_.tick = tick;
            curr_.down = InputIntent::NONE;
        }

        for (const auto& event : events)
        {
            if (event.consumed)
                continue;

            keyStates_[event.key].value = event.value;

            if (event.action == KeyAction::RELEASE)
                curr_.held &= ~keyIntents_[event.key];
            else             // KeyAction::PRESS
            {
                if (!HasIntent(prev_.held, keyIntents_[event.key]))
                    curr_.down |= keyIntents_[event.key];
                curr_.held |= keyIntents_[event.key];
            }
        }
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