set(target_name htracer.apps.benchmarks)

add_executable(${target_name})

target_sources(
    ${target_name}
    PRIVATE
        src/app.cpp
        src/exit.cpp
        src/main.cpp
        src/benchmark_case.cpp
        src/benchmark_definition.cpp
        src/benchmark_result.cpp
        src/cli.cpp
        
        src/image_extent.cpp
        src/measurement_plan.cpp
        src/quick_suite.cpp
        src/render_benchmark.cpp
        src/render_mode.cpp
        src/scene_spec.cpp
        src/cli/custom_definition.cpp
        src/cli/raw_options.cpp
        src/reporting/benchmark_name.cpp
        src/reporting/console.cpp
        src/reporting/environment.cpp
        src/reporting/json.cpp
        src/reporting/json_writer.cpp
        src/reporting/result_values.cpp
    PRIVATE
        FILE_SET HEADERS
        BASE_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/benchmark_case.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/benchmark_definition.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/benchmark_result.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/app.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli/custom_definition.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli/raw_options.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/exit.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/execution/image_validation.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/execution/measurement.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/image_extent.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/invocation.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/measurement_plan.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/quick_suite.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/render_benchmark.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/render_mode.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/reporting/benchmark_name.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/reporting/console.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/reporting/environment.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/reporting/json.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/reporting/json_writer.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/reporting/result_values.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/run_report.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/scene_spec.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/scenes/make_scene.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/scenes/mixed.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/scenes/rng_probe.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/scenes/scene_setup.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/scenes/traversal.hpp"
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
