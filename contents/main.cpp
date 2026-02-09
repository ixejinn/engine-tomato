#include "tomato/tomato.h"
#include "tomato/net/rollback/RollbackSlice.h"

#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/Movement.h"

#include "TestState.h"

int main() {
    tmt::WindowService window(1600, 900, "TOMATO");
  
    tmt::Engine engine(window);
    engine.SetRollbackManager<tmt::RollbackSlice<tmt::PositionComponent, tmt::JumpComponent>>();    // State 내부에서 하도록 옮기는게 좋을지도?
    engine.SetNextState(std::make_unique<TestState>());
    engine.Run();
}