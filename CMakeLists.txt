cmake_minimum_required(VERSION 3.25)

# Trigger configuration on file change.
configure_file(htracer.json htracer.json)

file(READ ${CMAKE_CURRENT_BINARY_DIR}/htracer.json HTRACER_JSON_STRING)
string(JSON VERSION_FROM_JSON GET ${HTRACER_JSON_STRING} version)

project(htracer LANGUAGES CXX VERSION ${VERSION_FROM_JSON})

option(
    HTRACER_BUILD_TESTS
    "Build the tests and enable testing. Default: ${PROJECT_IS_TOP_LEVEL}."
    ${PROJECT_IS_TOP_LEVEL})

option(
    HTRACER_BUILD_RAY
    "Build the ray app. Default: ${PROJECT_IS_TOP_LEVEL}."
    ${PROJECT_IS_TOP_LEVEL})

option(
    HTRACER_BUILD_EXAMPLES
    "Build the examples. Default: ${PROJECT_IS_TOP_LEVEL}."
    ${PROJECT_IS_TOP_LEVEL})

enable_testing()


add_subdirectory(lib)

if(HTRACER_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(HTRACER_BUILD_RAY)
    add_subdirectory(apps)
endif()

if(HTRACER_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(
    EXPORT htracer-targets
    NAMESPACE htracer::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/htracer"
    FILE htracer-targets.cmake
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/htracer-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/lib/htracer-config.cmake
    INSTALL_DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/htracer
    NO_SET_AND_CHECK_MACRO
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/lib/htracer-version.cmake
    COMPATIBILITY ExactVersion
) 

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/lib/htracer-version.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/lib/htracer-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/htracer"
)