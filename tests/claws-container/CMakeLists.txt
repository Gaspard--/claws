set(SOURCES vect-test.cpp)
CREATE_UNIT_TEST(container-test claws: "${SOURCES}")
target_link_libraries(container-test claws::container)
