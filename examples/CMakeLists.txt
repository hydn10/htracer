function(add_example example_name)
    set(target_name "htracer.examples.${example_name}")

    add_executable(${target_name})

    target_sources(${target_name} PRIVATE ${ARGN})
    
    target_compile_features(${target_name} PRIVATE cxx_std_17)

    target_link_libraries(
        ${target_name}
        PRIVATE
        htracer::htracer
    )

    set_target_properties(
        ${target_name}
        PROPERTIES
            OUTPUT_NAME "htracer-${example_name}-example"
            EXPORT_NAME "${example_name}-example"
    )
    
    install(
        TARGETS ${target_name}
        EXPORT htracer-targets
    )

    message(STATUS "Added example: ${target_name}")
endfunction()

add_subdirectory(basic)
add_subdirectory(disco_ring)
