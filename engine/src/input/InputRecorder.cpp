#include "tomato/input/InputRecorder.h"
#include "tomato/services/InputService.h" // KeyEvent
#include "tomato/tomato_math.h"
#include "tomato/Logger.h"

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
                if (!HasIntent(prev_.held, keyIntents_[event.key]))
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
                if (!HasIntent(prev_.held, keyIntents_[event.key]))
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

    void InputRecorder::InitKeyActionMap()
    {
        keyIntents_[Key::W] = InputIntent::UP;
        keyIntents_[Key::S] = InputIntent::DOWN;
        keyIntents_[Key::A] = InputIntent::LEFT;
        keyIntents_[Key::D] = InputIntent::RIGHT;
        keyIntents_[Key::SpaceBar] = InputIntent::JUMP;
    }
}