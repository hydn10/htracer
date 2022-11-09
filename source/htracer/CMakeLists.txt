# Trigger configuratoin on file change.
configure_file(include_files.txt include_files.txt)
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/include_files.txt HTRACER_LIBRARY_SOURCE_FILES)

add_library(htracer INTERFACE)
add_library(HTracer::HTracer ALIAS htracer)

target_sources(
    htracer INTERFACE
    FILE_SET HEADERS
    BASE_DIRS include
    FILES ${HTRACER_LIBRARY_SOURCE_FILES}
)

target_compile_features(htracer INTERFACE cxx_std_20)


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