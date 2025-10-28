add_executable(contents
        main.cpp
        TestState.h
        TestState.cpp
)

target_link_libraries(contents PRIVATE engine)

# 셰이더 폴더
set(SHADER_DIR ${CMAKE_SOURCE_DIR}/assets)

# 실행 파일 옆 assets 폴더 경로
set(TARGET_ASSETS_DIR $<TARGET_FILE_DIR:contents>/assets)

# 빌드 후 assets 폴더 통째로 복사
add_custom_command(TARGET contents POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SHADER_DIR} ${TARGET_ASSETS_DIR}
)

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