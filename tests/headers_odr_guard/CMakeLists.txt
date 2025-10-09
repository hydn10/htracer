add_executable(htracer.tests.headers_odr_guard)

target_sources(
    htracer.tests.headers_odr_guard
    PRIVATE
        FILE_SET HEADERS
        FILES
            another.hpp
    PRIVATE
        main.cpp
        another.cpp
)

target_compile_features(htracer.tests.headers_odr_guard PRIVATE cxx_std_17)

target_link_libraries(
    htracer.tests.headers_odr_guard
    PRIVATE
    htracer::htracer
)

add_test(
    NAME headers_odr_guard
    COMMAND htracer.tests.headers_odr_guard
)

message(STATUS "Added test: htracer.tests.headers_odr_guard")
