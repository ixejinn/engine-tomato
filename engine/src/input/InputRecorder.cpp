#include "tomato/input/InputRecorder.h"
#include "tomato/services/InputService.h" // KeyEvent
#include "tomato/tomato_math.h"
#include <iostream>

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

            if (event.action == KeyAction::RELEASE)
            {
                keyStates_[event.key].value = 0;
                curr_.held &= ~keyIntents_[event.key];
            }

            else             // KeyAction::PRESS
            {
                keyStates_[event.key].value = 1;
                curr_.held |= keyIntents_[event.key];
            }
        }
        curr_.down = ~prev_.held & curr_.held;
    }

    char InputRecorder::TMP_UpdateInputAxis(std::vector<KeyEvent>& events, uint32_t tick)
    {
        prev_ = curr_;
        curr_.tick = tick;

        for (const auto& event : events)
        {
            if (event.consumed)
                continue;

            if (event.action == KeyAction::RELEASE)
            {
                keyStates_[event.key].value = 0;
                curr_.held &= ~keyIntents_[event.key];
            }

            else             // KeyAction::PRESS
            {
                keyStates_[event.key].value = 1;
                curr_.held |= keyIntents_[event.key];

                if (event.key == Key::Z)
                    return 1;

                if (event.key == Key::X)
                    return 2;

                if (event.key == Key::C)
                    return 3;

                if (event.key == Key::V)
                    return 4;
            }
        }
        curr_.down = ~prev_.held & curr_.held;
        return 0;
    }

    void InputRecorder::InitKeyActionMap()
    {
        keyIntents_[Key::W] = InputIntent::UP;
        keyIntents_[Key::S] = InputIntent::DOWN;
        keyIntents_[Key::A] = InputIntent::LEFT;
        keyIntents_[Key::D] = InputIntent::RIGHT;
    }
}