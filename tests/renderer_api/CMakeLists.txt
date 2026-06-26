add_executable(htracer.tests.renderer_api)

target_sources(
    htracer.tests.renderer_api
    PRIVATE main.cpp
)

target_compile_features(htracer.tests.renderer_api PRIVATE cxx_std_17)

target_link_libraries(
    htracer.tests.renderer_api
    PRIVATE
    htracer::htracer
)

add_test(
    NAME renderer_api
    COMMAND htracer.tests.renderer_api
)

message(STATUS "Added test: htracer.tests.renderer_api")
