set(target_name "htracer.apps.ray")

add_executable(${target_name})

target_sources(${target_name} PRIVATE main.cpp)

target_compile_features(${target_name} PRIVATE cxx_std_17)

target_link_libraries(
    ${target_name}
    PRIVATE
    htracer::htracer
)

set_target_properties(
    ${target_name}
    PROPERTIES
        EXPORT_NAME ray-app
)

install(
    TARGETS ${target_name}
    EXPORT htracer-targets
)

message(STATUS "Added app: ${target_name}")