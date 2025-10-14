add_executable(contents
        main.cpp
        TestState.h
        TestState.cpp
)

target_link_libraries(contents PRIVATE engine)