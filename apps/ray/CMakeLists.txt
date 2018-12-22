set(target ray)

add_executable(
    ${target}
    main.cpp
)

target_link_libraries(
    ${target}
    raytracerlib
)