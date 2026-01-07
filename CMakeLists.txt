cmake_minimum_required(VERSION 3.26)
project(tomato
        VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 20)
# requires expression (NetBitReader::ReadInt)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(/utf-8)

include(FetchContent)

# GLFW
FetchContent_Declare(glfw
        GIT_REPOSITORY  https://github.com/glfw/glfw.git
        GIT_TAG         3.4
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS   OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLM
FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        1.0.2
)
FetchContent_MakeAvailable(glm)

# EnTT
FetchContent_Declare(entt
        GIT_REPOSITORY https://github.com/skypjack/entt
        GIT_TAG        v3.16.0
)
FetchContent_MakeAvailable(entt)

add_subdirectory(engine)
add_subdirectory(contents)