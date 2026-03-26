#include "tomato/input/InputRecorder.h"
#include "tomato/services/InputService.h" // KeyEvent
#include "tomato/tomato_math.h"
#include "tomato/Logger.h"
#include "tomato/utils/BitmaskOperators.h"

#include <iostream>

namespace tomato
{
    InputRecorder::InputRecorder()
    {
        InitKeyActionMap();
    }

    bool InputRecorder::UpdateInputKey(const KeyEvent& event)
    {
        keyStates_[event.key].value = event.value;

        if (keyIntents_[event.key] != InputIntent::NONE)
        {
            if (event.action == KeyAction::RELEASE)
                curr_.held &= ~keyIntents_[event.key];
            else             // KeyAction::PRESS
            {
                if (!HasFlag(prev_.held, keyIntents_[event.key]))
                    curr_.down |= keyIntents_[event.key];
                curr_.held |= keyIntents_[event.key];
            }
        }

        return true;
    }

    bool InputRecorder::UpdateInputMouse(const MouseEvent& event)
    {
        keyStates_[event.key].value = event.value;
        keyStates_[Key::MouseX].value = event.xPos;
        keyStates_[Key::MouseY].value = event.yPos;

        if (keyIntents_[event.key] != InputIntent::NONE)
        {
            if (event.action == KeyAction::RELEASE)
                curr_.held &= ~keyIntents_[event.key];
            else             // KeyAction::PRESS
            {
                if (!HasFlag(prev_.held, keyIntents_[event.key]))
                    curr_.down |= keyIntents_[event.key];
                curr_.held |= keyIntents_[event.key];
            }
        }

        return true;
    }

    void InputRecorder::ResetInputRecords(uint32_t tick)
    {
        if (curr_.tick != tick)
        {
            prev_ = curr_;
            curr_.tick = tick;
            curr_.down = InputIntent::NONE;
        }
    }

    char InputRecorder::TMP_UpdateInputAxis(std::vector<KeyEvent>& events, uint32_t tick)
    {
        prev_ = curr_;
        curr_.tick = tick;

        for (const auto& event : events)
        {
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
        keyIntents_[Key::SpaceBar] = InputIntent::JUMP;
    }
}