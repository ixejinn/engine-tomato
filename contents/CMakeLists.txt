file (GLOB_RECURSE CONTENTS_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/*.h
)

add_executable(contents ${CONTENTS_SOURCES})

target_link_libraries(contents PRIVATE tomato)