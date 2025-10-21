set(ENGINE_HEADERS
        include/tomato/services/WindowService.h
        include/tomato/ecs/World.h
        include/tomato/Engine.h
        include/tomato/State.h
        include/tomato/ecs/systems/System.h
        include/tomato/tomato.h
        include/tomato/ecs/components/Transform.h
        include/tomato/ecs/components/Rigidbody.h
        include/tomato/ecs/components/Movement.h
        include/tomato/ecs/systems/TransformSystem.h
        include/tomato/services/InputService.h
        include/tomato/ecs/systems/KinematicMovementSystem.h
        include/tomato/services/CharacterInputHistory.h
        include/tomato/services/InputTypes.h
        include/tomato/SimulationContext.h
        include/tomato/DefaultState.h
        include/tomato/ecs/SystemManager.h
        include/tomato/ecs/SystemRegistry.h
)

set(ENGINE_SOURCES
        src/services/WindowService.cpp
        src/core/Engine.cpp
        src/ecs/systems/TransformSystem.cpp
        src/services/InputService.cpp
        src/ecs/systems/KinematicMovementSystem.cpp
        src/core/DefaultState.cpp
        src/ecs/SystemManager.cpp
        src/ecs/SystemRegistry.cpp
)

set(EXTERNAL_SOURCES
        external/glad/src/glad.c
)

add_library(engine
        ${ENGINE_HEADERS}
        ${ENGINE_SOURCES}
        ${EXTERNAL_SOURCES}
)

target_include_directories(engine PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/external/glad/include
        ${CMAKE_CURRENT_LIST_DIR}/external/entt/include
        ${CMAKE_CURRENT_LIST_DIR}/external/glm/include
)

if (NOT TARGET glfw)
    message(FATAL_ERROR "Target 'glfw' not found")
endif()
target_link_libraries(engine PUBLIC glfw)

if (WIN32)
    target_link_libraries(engine PUBLIC opengl32)
elseif(APPLE)
    target_link_libraries(engine PUBLIC "-framework OpenGL")
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(engine PUBLIC OpenGL::GL)
endif()