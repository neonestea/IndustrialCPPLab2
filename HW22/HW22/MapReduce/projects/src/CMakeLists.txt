set(target wordcount)

set(sources
    main.cpp
)


add_executable(${target}
    ${sources}
)

target_link_libraries(${target}
    PRIVATE
    mapreduce
    
)
