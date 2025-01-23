function(add_example example_name)
    string(CONCAT target_name "${example_name}" "_example")

    add_executable(${target_name} ${ARGN})
    
    target_compile_features(${target_name} PRIVATE cxx_std_17)

    target_link_libraries(
        ${target_name}
        PRIVATE
        htracer
    )

    message(STATUS "Added example: ${target_name}")

    install(TARGETS ${target_name})
endfunction()

add_subdirectory(basic)
add_subdirectory(disco_ring)
