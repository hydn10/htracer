cmake_minimum_required(VERSION 3.25)

project(htracer_installed_consumer LANGUAGES CXX)

find_package(htracer CONFIG REQUIRED)

add_executable(htracer.installed_consumer main.cpp)

target_link_libraries(
    htracer.installed_consumer
    PRIVATE
        htracer::htracer
)

enable_testing()

add_test(
    NAME htracer.installed_consumer
    COMMAND htracer.installed_consumer
)

install(TARGETS htracer.installed_consumer)
