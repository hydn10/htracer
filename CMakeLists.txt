cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(cmake/Custom.cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(RAYTRACER C CXX)

add_subdirectory(src)
add_subdirectory(apps)