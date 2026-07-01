add_executable(htracer.tests.randomized_renderer)

target_sources(
    htracer.tests.randomized_renderer
    PRIVATE main.cpp
)

target_link_libraries(
    htracer.tests.randomized_renderer
    PRIVATE
    htracer::htracer
)

add_test(
    NAME randomized_renderer
    COMMAND htracer.tests.randomized_renderer
)

message(STATUS "Added test: htracer.tests.randomized_renderer")
