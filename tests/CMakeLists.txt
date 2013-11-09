add_executable(
  test-darts
  test-darts.cc
)

target_link_libraries(
	test-darts
	libdarts
)

add_test(
  test-darts-test
  ${PROJECT_BINARY_DIR}/tests/test-darts
)
