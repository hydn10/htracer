add_executable(
    compare_output_test
	EXCLUDE_FROM_ALL
    main.cpp
)
target_compile_features(compare_output_test PRIVATE cxx_std_17)
target_link_libraries(
    compare_output_test
    PRIVATE
    htracer
)

add_custom_target(
	compare_output_test_wrapper
	COMMAND compare_output_test
	COMMAND ${CMAKE_COMMAND} -E compare_files ray_compare_test.ppm ${CMAKE_CURRENT_SOURCE_DIR}/ray_compare_test-golden.ppm
	VERBATIM
)

add_test(
	NAME compare_output
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target compare_output_test_wrapper
)