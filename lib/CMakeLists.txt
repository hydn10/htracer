# Automatically discover header files in the include directory
file(
    GLOB_RECURSE
    HTRACER_LIBRARY_SOURCE_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hxx"
)

add_library(htracer INTERFACE)
add_library(HTracer::HTracer ALIAS htracer)

target_sources(
    htracer INTERFACE
    FILE_SET HEADERS
    BASE_DIRS include
    FILES ${HTRACER_LIBRARY_SOURCE_FILES}
)

target_compile_features(htracer INTERFACE cxx_std_23)


include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set_target_properties(htracer PROPERTIES EXPORT_NAME HTracer)

write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/HTracerConfigVersion.cmake
    VERSION ${CMAKE_PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    HTracerConfig.cmake.in
    ${PROJECT_BINARY_DIR}/HTracerConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/htracer
    NO_SET_AND_CHECK_MACRO
)

install(
    TARGETS htracer
    EXPORT HTracerExport
    FILE_SET
        HEADERS
        DESTINATION include
)
install(
    EXPORT HTracerExport
    NAMESPACE HTracer::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/htracer"
)

install(
    FILES
        "${PROJECT_BINARY_DIR}/HTracerConfigVersion.cmake"
        "${PROJECT_BINARY_DIR}/HTracerConfig.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/htracer"
)

message(STATUS "Added library: htracer")