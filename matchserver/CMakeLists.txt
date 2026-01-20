add_executable(matchserver
        src/main.cpp
        src/MatchServer.cpp
        src/Match.cpp
        src/MatchManager.cpp)

target_link_libraries(matchserver
        PRIVATE
        engine
)
target_include_directories(matchserver
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
