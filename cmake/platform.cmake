
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LINUX TRUE)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(OSX TRUE)
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CLANG TRUE)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(GCC TRUE)
endif()
