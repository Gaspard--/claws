set(SOURCES box-test.cpp lambda_utils-test.cpp)
CREATE_UNIT_TEST(utils-test claws: "${SOURCES}")
target_link_libraries(utils-test claws::utils)
