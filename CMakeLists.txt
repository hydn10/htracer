cmake_minimum_required(VERSION 3.23)

# Trigger configuration on file change.
configure_file(htracer.json htracer.json)

file(READ ${CMAKE_CURRENT_BINARY_DIR}/htracer.json HTRACER_JSON_STRING)
string(JSON VERSION_FROM_JSON GET ${HTRACER_JSON_STRING} version)

project(HTRACER LANGUAGES CXX VERSION ${VERSION_FROM_JSON})

option(HTRACER_BUILD_RAY "Build the ray app" OFF)
option(HTRACER_BUILD_TESTS "Build the tests and enable testing" OFF)

if(HTRACER_BUILD_TESTS)
	enable_testing()
endif()

add_subdirectory(source)