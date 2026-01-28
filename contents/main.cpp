#include "tomato/tomato.h"

#include "TestState.h"

int main() {
    tmt::WindowService window(1600, 900, "TOMATO");
  
    tmt::Engine engine(window);
    engine.SetNextState(std::make_unique<TestState>());
    engine.Run();
}