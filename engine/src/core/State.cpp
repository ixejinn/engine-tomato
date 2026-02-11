#include "tomato/State.h"
#include "tomato/resource/AssetRegistry.h"

namespace tomato
{
    State::State()
    {
        //TODO: AssetRegistry<T>.Load() 기본 리소스 로드
        // 또는 각 리소스 타입 클래스에서 static 기본 리소스 로드 함수를 만들고, 그 함수를 각 타입의 AssetRegistry 생성자에서 호출
        // 이 경우 AssetRegistry<T>.Clear()에서는 기본 리소스를 제외한 모두를 해제하도록 해야 함
    }

    State::~State()
    {
        //TODO: AssetRegistry<T>.Clear() 모든 리소스 타입에 대해 순회
    }
}