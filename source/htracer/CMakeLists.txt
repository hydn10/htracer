set(
    HTRACER_LIBRARY_SOURCE_FILES
    include/htracer/colors/colors.hpp
    include/htracer/colors/hsl.hxx
    include/htracer/colors/hsl_impl.hxx
    include/htracer/colors/srgb.hxx
    include/htracer/colors/srgb_impl.hxx
    include/htracer/colors/srgb_linear.hxx
    include/htracer/colors/srgb_linear_impl.hxx
    include/htracer/geometries/geometry.hpp
    include/htracer/geometries/plane.hpp
    include/htracer/geometries/ray.hpp
    include/htracer/geometries/sphere.hpp
    include/htracer/numerics.hpp
    include/htracer/outputs/ppm.hpp
    include/htracer/raytracing/camera.hpp
    include/htracer/raytracing/image.hpp
    include/htracer/raytracing/intersector.hpp
    include/htracer/raytracing/sampler.hpp
    include/htracer/scene/light.hpp
    include/htracer/scene/material.hpp
    include/htracer/scene/object_base.hpp
    include/htracer/scene/object.hpp
    include/htracer/scene/scene.hpp
    include/htracer/scene/scene_view.hpp
    include/htracer/utils/heterogeneous_visitable.hpp
    include/htracer/utils/random.hpp
    include/htracer/utils/vector_crtp.hpp
    include/htracer/utils/vector_transform.hpp
    include/htracer/vector.hpp
)


add_library(htracer INTERFACE)
add_library(HTracer::HTracer ALIAS htracer)

target_sources(
    htracer INTERFACE
    FILE_SET HEADERS
    BASE_DIRS include
    FILES ${HTRACER_LIBRARY_SOURCE_FILES}
)

target_compile_features(htracer INTERFACE cxx_std_20)


include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set_target_properties(htracer PROPERTIES EXPORT_NAME HTracer)

write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/HTracerConfigVersion.cmake
    VERSION ${CMAKE_PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    HTracerConfig.cmake.in
    ${PROJECT_BINARY_DIR}/HTracerConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/htracer
    NO_SET_AND_CHECK_MACRO
)

install(
    TARGETS htracer
    EXPORT HTracerExport
    FILE_SET
        HEADERS
        DESTINATION include
)
install(
    EXPORT HTracerExport
    NAMESPACE HTracer::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/htracer"
)

install(
    FILES
        "${PROJECT_BINARY_DIR}/HTracerConfigVersion.cmake"
        "${PROJECT_BINARY_DIR}/HTracerConfig.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/htracer"
)