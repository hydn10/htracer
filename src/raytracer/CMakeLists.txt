set(target raytracerlib)

add_library(${target} ${CMAKE_CURRENT_SOURCE_DIR}/src/null.cpp)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)