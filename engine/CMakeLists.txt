set(ENGINE_HEADERS
        include/tomato/DefaultState.h
        include/tomato/Engine.h
        include/tomato/Logger.h
        include/tomato/SimulationContext.h
        include/tomato/State.h
        include/tomato/tomato.h
        include/tomato/ecs/SystemManager.h
        include/tomato/ecs/SystemRegistry.h
        include/tomato/ecs/World.h
        include/tomato/ecs/components/Camera.h
        include/tomato/ecs/components/Movement.h
        include/tomato/ecs/components/Rigidbody.h
        include/tomato/ecs/components/Sprite.h
        include/tomato/ecs/components/Tags.h
        include/tomato/ecs/components/Transform.h
        include/tomato/ecs/systems/CameraSystem.h
        include/tomato/ecs/systems/KinematicMovementSystem.h
        include/tomato/ecs/systems/RenderSystem.h
        include/tomato/ecs/systems/System.h
        include/tomato/ecs/systems/TransformSystem.h
        include/tomato/render/Mesh.h
        include/tomato/render/Shader.h
        include/tomato/render/Texture.h
        include/tomato/services/CharacterInputHistory.h
        include/tomato/services/InputService.h
        include/tomato/services/InputTypes.h
        include/tomato/services/WindowService.h
        #include/tomato/services/network/Socket.h
        #include/tomato/services/network/SocketAddress.h
        include/tomato/services/network/NetworkService.h
        include/tomato/serialization/BitReader.h
        include/tomato/serialization/BitWriter.h
        include/tomato/services/network/NetBitReader.h
        include/tomato/services/network/CoreNetwork.h
        include/tomato/services/network/NetBitWriter.h
        include/tomato/containers/MemoryPool.h
)

set(ENGINE_SOURCES
        src/core/DefaultState.cpp
        src/core/Engine.cpp
        src/ecs/systems/CameraSystem.cpp
        src/ecs/systems/KinematicMovementSystem.cpp
        src/ecs/systems/RenderSystem.cpp  
        src/ecs/systems/TransformSystem.cpp
        src/ecs/SystemManager.cpp
        src/ecs/SystemRegistry.cpp
        src/render/Mesh.cpp
        src/render/Shader.cpp
        src/render/Texture.cpp
        src/services/InputService.cpp
        src/services/WindowService.cpp
        #src/services/network/Socket.cpp
        #src/services/network/SocketAddress.cpp
        src/serialization/BitReader.cpp
        src/serialization/BitWriter.cpp
        src/services/network/NetBitReader.cpp
)

set(EXTERNAL_SOURCES
        external/glad/src/glad.c
)

add_library(engine STATIC
        ${ENGINE_HEADERS}
        ${ENGINE_SOURCES}
        ${EXTERNAL_SOURCES}
)

target_include_directories(engine PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/external
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