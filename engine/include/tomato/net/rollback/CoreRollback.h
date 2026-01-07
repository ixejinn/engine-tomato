#ifndef TOMATO_COREROLLBACK_H
#define TOMATO_COREROLLBACK_H

namespace tomato
{
    // !!! 네트워크 지연 커버리지 기준으로 수정 필요 !!!
    // 100틱 - 1.67초 (최소한의 롤백 보장)
    // 300틱 - 5초
    // 500틱 - 8.33초
    constexpr int ROLLBACK_WINDOW{128};
}

#endif //TOMATO_COREROLLBACK_H
