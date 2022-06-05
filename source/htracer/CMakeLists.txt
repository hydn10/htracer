set(target htracer)

add_library(${target} INTERFACE)
target_include_directories(${target} INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features (${target} INTERFACE cxx_std_20)


include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${target}ConfigVersion.cmake"
    VERSION ${CMAKE_PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

configure_package_config_file(
    "${target}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${target}Config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${target}")

install(TARGETS ${target} EXPORT ${target}Targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(EXPORT ${target}Targets DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${target}")

install(FILES "${PROJECT_BINARY_DIR}/${target}ConfigVersion.cmake"
              "${PROJECT_BINARY_DIR}/${target}Config.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${target}")

install(DIRECTORY include/${target} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})