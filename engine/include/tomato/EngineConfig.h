#ifndef TOMATO_ENGINECONFIG_H
#define TOMATO_ENGINECONFIG_H

namespace tomato
{
    // 나중에 현재 EngineConfig → EngineLimits로 바꾸고
    // EngineConfig나 GameConfig를 추가해서 엔진 객체 생성할 때 알맞게 엔진 변수 값 설정할 수 있도록 바꾸기
    struct EngineConfig
    {
        static constexpr int MAX_PLAYER_NUM{4};
    };
}

#endif //TOMATO_ENGINECONFIG_H
