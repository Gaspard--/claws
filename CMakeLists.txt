if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. Please create a build directory outside of the source code and call cmake from there")
endif ()

cmake_minimum_required(VERSION 3.9)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

##! Project
project(claws)

##! Prerequisites CTEST
enable_testing()

##! Project options
option(CLAWS_BUILD_TESTS "Build claws tests" ON)
option(CLAWS_BUILD_EXAMPLES "Build claws examples" OFF)
option(USE_PROJECT_IN_AN_IDE "Workaround for header-only libraries, put it to ON if you use CLION" ON)

##! CMake Path
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(library)

##! Project modules
add_subdirectory(modules)

##! Project tests
if (CLAWS_BUILD_TESTS)
    add_subdirectory(tests)
endif ()

##! Project examples
if (CLAWS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

##! Main library
add_library(claws INTERFACE)

target_link_libraries(claws INTERFACE
        claws::utils
        )
