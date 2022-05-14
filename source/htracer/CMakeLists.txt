set(target htracerlib)

add_library(${target} INTERFACE)
target_include_directories(${target} INTERFACE ./include)
target_compile_features (${target} INTERFACE cxx_std_20)