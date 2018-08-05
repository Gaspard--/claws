set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

##! Global Configuration
set(COMMON_CXX_FLAGS "-Wall -Wextra -Wfatal-errors -Wno-unknown-pragmas")

##! Configuration for "Debug" builds
macro(__internal_debug_cxx_flags)
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_CXX_FLAGS} -O0 -g3")
endmacro()

##! Configuration for "Release" builds
macro(__internal_release_cxx_flags)
    add_definitions(-DRELEASE)
    add_definitions(-DNDEBUG)
    set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O3 -march=native")
endmacro()

##! Configuration for "MinSizeRel" builds
macro(__internal_minsizerel_cxx_flags)
    __internal_release_cxx_flags()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")
endmacro()

##! Configuration for "RelWithDebInfo" builds
macro(__internal_relwithdebinfo_cxx_flags)
    __internal_release_cxx_flags()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g3")
endmacro()


##! Compiler Configuration
if (CMAKE_BUILD_TYPE MATCHES Debug)
    __internal_debug_cxx_flags()
    MSG_YELLOW_BOLD(STATUS "CMAKE_CXX_FLAGS_DEBUG:" "${CMAKE_CXX_FLAGS_DEBUG}" "")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    __internal_release_cxx_flags()
    MSG_YELLOW_BOLD(STATUS "CMAKE_CXX_FLAGS_RELEASE:" "${CMAKE_CXX_FLAGS_RELEASE}" "")
elseif (CMAKE_BUILD_TYPE MATCHES MinSizeRel)
    __internal_minsizerel_cxx_flags()
    MSG_YELLOW_BOLD(STATUS "CMAKE_CXX_FLAGS_RELEASE:" "${CMAKE_CXX_FLAGS_RELEASE}" "")
elseif (CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    __internal_relwithdebinfo_cxx_flags()
    MSG_YELLOW_BOLD(STATUS "CMAKE_CXX_FLAGS_RELEASE:" "${CMAKE_CXX_FLAGS_RELEASE}" "")
endif ()
