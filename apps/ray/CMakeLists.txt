set(target ray)

add_executable(
    ${target}
    main.cpp
)

target_compile_features(${target} PRIVATE cxx_std_17)

target_link_libraries(
    ${target}
    PRIVATE
    htracer
)

install(TARGETS ${target})

message(STATUS "Added app: ${target}")