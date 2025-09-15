cmake_minimum_required(VERSION 3.23)

# Trigger configuration on file change.
configure_file(htracer.json htracer.json)

file(READ ${CMAKE_CURRENT_BINARY_DIR}/htracer.json HTRACER_JSON_STRING)
string(JSON VERSION_FROM_JSON GET ${HTRACER_JSON_STRING} version)

project(HTRACER LANGUAGES CXX VERSION ${VERSION_FROM_JSON})

option(HTRACER_BUILD_TESTS "Build the tests and enable testing" OFF)
option(HTRACER_BUILD_RAY "Build the ray app" OFF)
option(HTRACER_BUILD_EXAMPLES "Build the examples" OFF)
option(HTRACER_ENABLE_CLANG_TIDY "Enable clang-tidy analysis" OFF)

if(HTRACER_ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES clang-tidy clang-tidy.exe)

  if(CLANG_TIDY_EXE)
    message(STATUS "clang-tidy enabled: ${CLANG_TIDY_EXE}")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};--warnings-as-errors=*")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  else()
    message(FATAL_ERROR "HTRACER_ENABLE_CLANG_TIDY=ON but clang-tidy not found.")
  endif()
endif()

if(HTRACER_BUILD_TESTS)
    enable_testing()
endif()

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
