set(target_name htracer.apps.benchmarks)

add_executable(${target_name})

target_sources(
    ${target_name}
    PRIVATE
        src/main.cpp
        src/model.cpp
        src/cli.cpp
        src/render_benchmark.cpp
        src/report.cpp
        src/suite.cpp
    PRIVATE
        FILE_SET HEADERS
        BASE_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/model.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/render_benchmark.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/report.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/scenes.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/suite.hpp"
)

target_compile_features(${target_name} PRIVATE cxx_std_23)

target_compile_definitions(
    ${target_name}
    PRIVATE
        HTRACER_BENCHMARK_BUILD_TYPE="$<CONFIG>"
)

target_link_libraries(${target_name} PRIVATE htracer::htracer)

set_target_properties(
    ${target_name}
    PROPERTIES
        OUTPUT_NAME htracer-benchmarks
        EXPORT_NAME benchmarks-app
)

install(
    TARGETS ${target_name}
    EXPORT htracer-targets
)

add_test(
    NAME benchmarks.list
    COMMAND ${target_name} --list
)

message(STATUS "Added app: ${target_name}")
