add_subdirectory(htracer)

if (HTRACER_BUILD_RAY)
    add_subdirectory(apps)
endif()
