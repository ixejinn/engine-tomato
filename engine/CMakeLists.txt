set(TOMATO_SOURCES
        components/Transform.h
        components/Rigidbody.h
        components/PlayerController.h
        managers/GameManager.h
        managers/GameManager.cpp
        states/State.h
)

set(EXTERNAL_SOURCES
        external/glad/src/glad.c
)

add_library(tomato
        ${TOMATO_SOURCES}
        ${EXTERNAL_SOURCES}
)

target_include_directories(tomato PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/external/glad/include
        ${CMAKE_CURRENT_LIST_DIR}/external/entt/include
        ${CMAKE_CURRENT_LIST_DIR}/external/glm/include
)

if (NOT TARGET glfw)
    message(FATAL_ERROR "Target 'glfw' not found")
endif()
target_link_libraries(tomato PUBLIC glfw)

if (WIN32)
    target_link_libraries(tomato PUBLIC opengl32)
elseif(APPLE)
    target_link_libraries(tomato PUBLIC "-framework OpenGL")
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(tomato PUBLIC OpenGL::GL)
endif()
