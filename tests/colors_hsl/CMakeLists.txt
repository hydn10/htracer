add_executable(htracer.tests.colors_hsl)

target_sources(htracer.tests.colors_hsl PRIVATE main.cpp)
target_link_libraries(htracer.tests.colors_hsl PRIVATE htracer::htracer)

add_test(NAME colors_hsl COMMAND htracer.tests.colors_hsl)

message(STATUS "Added test: htracer.tests.colors_hsl")
