set(target test)
find_package (Threads REQUIRED)

set(sources
    wordcount/src/test.cpp ${CMAKE_SOURCE_DIR}/third-party/Catch2/extras/catch_amalgamated.cpp
)


add_executable(${target}
    ${sources}
)

target_link_libraries(${target}
    PRIVATE
    mapreduce
    ${CMAKE_THREAD_LIBS_INIT}
)
