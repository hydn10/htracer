cmake_minimum_required(VERSION 3.21)

file (READ htracer.json HTRACER_JSON_STRING)
string (JSON VERSION_FROM_JSON GET ${HTRACER_JSON_STRING} version)

project(HTRACER LANGUAGES CXX VERSION ${VERSION_FROM_JSON})

option(HTRACER_BUILD_RAY "Build the ray app." OFF)

add_subdirectory(source)
