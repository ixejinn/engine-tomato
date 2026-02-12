#ifndef TOMATO_INPUTRECORDER_H
#define TOMATO_INPUTRECORDER_H

#include <array>
#include <vector>
#include <map>
#include "tomato/containers/EnumArray.h"
#include "tomato/input/InputTypes.h"
#include "tomato/input/InputRecord.h"

namespace tomato
{
    struct KeyEvent;

    /**
     * @brief Builds per-tick input records from raw platform events and current input states.
     *
     * 플랫폼 계층에서 수집된 입력 이벤트를 소비하여,
     * 현재 입력 상태(버튼/축 값)를 갱신하고, InputRecord를 생성한다.
     */
    class InputRecorder
    {
    public:
        InputRecorder();

        void UpdateInputAxis(std::vector<KeyEvent>& events, uint32_t tick);
        char TMP_UpdateInputAxis(std::vector<KeyEvent>& events, uint32_t tick);

        void BindInputIntent(Key key, InputIntent intent) { keyIntents_[key] = intent; }
        InputIntent GetBoundInputIntent(Key key) { return keyIntents_[key]; }

        const InputRecord& GetCurrInputRecord() const { return curr_; }

        float GetKeyStates(Key key) const { return keyStates_[key].value; }

    private:
        void InitKeyActionMap();

        EnumArray<Key, InputIntent> keyIntents_;
        EnumArray<Key, InputAxis> keyStates_;

        InputRecord prev_;
        InputRecord curr_;
    };
}

#endif //TOMATO_INPUTRECORDER_H
