add_subdirectory(htracer)

if(HTRACER_BUILD_RAY)
    add_subdirectory(apps)
endif()

if(HTRACER_BUILD_TESTS)
    add_subdirectory(tests)
endif()
