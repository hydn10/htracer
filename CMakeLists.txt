cmake_minimum_required(VERSION 3.20)

project(HTRACER LANGUAGES CXX VERSION 0.3.0)

option(HTRACER_BUILD_RAY "Build the ray app." OFF)

add_subdirectory(source)
