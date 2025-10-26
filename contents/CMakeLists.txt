add_executable(contents
        main.cpp
        TestState.h
        TestState.cpp
)

target_link_libraries(contents PRIVATE engine)

if (MSVC)
    target_link_options(contents PRIVATE
            LINKER:
            "/WHOLEARCHIVE:engine.lib"
    )
else()
    target_link_options(contents PRIVATE
            LINKER:
            "-Wl,--whole-archive"
            "$<TARGET_FILE:engine>"
            "-Wl,--no-whole-archive"
    )
endif()