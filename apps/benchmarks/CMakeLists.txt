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
        src/cli/parsers.cpp
        src/cli_structure/detail/argument_view.cpp
        src/cli_structure/detail/help_writer.cpp
        src/cli_structure/detail/option_parser.cpp
        src/cli_structure/detail/parsed_arguments.cpp
        src/cli_structure/foundations/errors.cpp
        src/cli_structure/parsing/parsers.cpp
        src/quick_suite.cpp
        src/render_benchmark.cpp
        src/reporting/benchmark_name.cpp
        src/reporting/console.cpp
        src/reporting/environment.cpp
        src/reporting/json.cpp
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
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli/commands.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli/invocation.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli/parsers.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli/render_configuration.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli/schema.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli/schema/command_tree.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli/schema/definition.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli/schema/inputs.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/commands/application.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/commands/command.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/commands/leaf_node.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/commands/subcommands.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/composition/composed_input.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/composition/inputs.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/composition/object_input.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/detail/argument_view.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/detail/brace_constructible.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/detail/help_writer.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/detail/name_validation.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/detail/option_descriptor.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/detail/option_parser.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/detail/parsed_arguments.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/detail/type_list.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/foundations/errors.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/foundations/help_page.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/foundations/parse_result.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/options/defaulted_value.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/options/flag_option.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/options/option_info.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/options/optional_value.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/options/value_option.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/cli_structure/parsing/parsers.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/exit.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/execution/checksum_accumulator.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/execution/image_validation.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/execution/measurement.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/execution/render_pipeline.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/image_extent.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/measurement_plan.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/quick_suite.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/render_benchmark.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/render_mode.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/reporting/benchmark_name.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/reporting/console.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/reporting/environment.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/reporting/json.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/reporting/result_values.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/run_report.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/htracer_benchmarks/schema_version.hpp"
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

message(STATUS "Added app: ${target_name}")
