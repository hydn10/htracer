add_executable(
    compare_output_test
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
	COMMAND compare_output_test compare_output.ppm
	COMMAND ${CMAKE_COMMAND} -E compare_files compare_output.ppm ${CMAKE_CURRENT_SOURCE_DIR}/golden.ppm
	VERBATIM
)

add_test(
	NAME compare_output
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target compare_output_test_wrapper
)