include(GoogleTest)

macro(__internal_specific_unit_test_platform_configuration EXECUTABLE_NAME)
    if (CLANG)
        set_target_properties(${EXECUTABLE_NAME} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors")
    endif ()
endmacro()

macro(__internal_specific_unit_test_properties EXECUTABLE_NAME)
    set_target_properties(${EXECUTABLE_NAME}
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
            LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/lib"
            LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin")
endmacro()

macro(CREATE_UNIT_TEST EXECUTABLE_NAME PREFIX_NAME SOURCES)
    add_executable(${EXECUTABLE_NAME} ${SOURCES})
    target_link_libraries(${EXECUTABLE_NAME} GTest::GTest GTest::Main)
    __internal_specific_unit_test_platform_configuration(${EXECUTABLE_NAME})
    __internal_specific_unit_test_properties(${EXECUTABLE_NAME})
    gtest_discover_tests(${EXECUTABLE_NAME} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin TEST_PREFIX ${PREFIX_NAME} TIMEOUT 5)
endmacro()
