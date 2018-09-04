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
option(IDE_BUILD "Workaround for header-only libraries, put it to ON if you use CLION" OFF)

##! CMake Path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
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
  claws::container
  claws::iterator
  claws::algorithm
  )

if (NOT IDE_BUILD)
  include(GNUInstallDirs)
  include(CMakePackageConfigHelpers)
  install(TARGETS
    claws
    EXPORT claws-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

  install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/modules
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}/claws
    FILES_MATCHING PATTERN "*.h*"
    )

  install(EXPORT claws-targets
    FILE claws-targets.cmake
    NAMESPACE claws::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/claws
    )

  configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/claws-config.cmake.in"
    "${PROJECT_BINARY_DIR}/claws-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/claws
    )

  install(FILES
    "${PROJECT_BINARY_DIR}/claws-config.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/claws)
endif()
