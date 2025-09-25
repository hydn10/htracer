# Automatically discover header files in the include directory
file(
    GLOB_RECURSE
    HTRACER_LIBRARY_SOURCE_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hxx"
)

add_library(htracer INTERFACE)
add_library(htracer::htracer ALIAS htracer)

target_sources(
    htracer
    INTERFACE
        FILE_SET HEADERS
        BASE_DIRS
            include
        FILES
            ${HTRACER_LIBRARY_SOURCE_FILES}
)

target_compile_features(htracer INTERFACE cxx_std_23)

set_target_properties(
    htracer
    PROPERTIES
        VERIFY_INTERFACE_HEADER_SETS ON
        EXPORT_NAME htracer
)

install(
    TARGETS htracer
    EXPORT htracer-targets
    FILE_SET HEADERS
)

message(STATUS "Added library: htracer")