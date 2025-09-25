add_executable(
    htracer.tests.compare_output
    main.cpp
)
target_compile_features(htracer.tests.compare_output PRIVATE cxx_std_17)
target_link_libraries(
    htracer.tests.compare_output
    PRIVATE
    htracer::htracer
)

add_custom_target(
	compare_output_test_wrapper
	COMMAND htracer.tests.compare_output compare_output.ppm
	COMMAND ${CMAKE_COMMAND} -E compare_files compare_output.ppm ${CMAKE_CURRENT_SOURCE_DIR}/golden.ppm
	DEPENDS htracer.tests.compare_output
	VERBATIM
)

add_test(
	NAME compare_output
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target compare_output_test_wrapper
)

message(STATUS "Added test: htracer.tests.compare_output")