cmake_minimum_required(VERSION 3.26)
project(tomato
        VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(glfw
        GIT_REPOSITORY  https://github.com/glfw/glfw.git
        GIT_TAG         3.4
)
set (GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set (BUILD_SHARED_LIBS   OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

add_subdirectory(engine)
add_subdirectory(contents)