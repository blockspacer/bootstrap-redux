cmake_minimum_required(VERSION 3.15)
project(basecode)

enable_language(C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# configure ccache if available
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    message(STATUS "${PROJECT_NAME} using ccache.")
endif(CCACHE_FOUND)

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -masm=intel -x assembler-with-cpp")

if (UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

set(DL_LIBRARY "")

if ("${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD")
    add_definitions(-D__USE_ISOC99)
elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    set(DL_LIBRARY "dl")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unused-private-field -Wno-unknown-pragmas -Wno-inconsistent-missing-override" CACHE STRING "compile flags" FORCE)
    message(STATUS "${PROJECT_NAME} using clang flags: ${CMAKE_CXX_FLAGS}")
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall -Wno-unknown-pragmas -Wno-parentheses -Wno-sequence-point" CACHE STRING "compile flags" FORCE)
    message(STATUS "${PROJECT_NAME} using gcc flags: ${CMAKE_CXX_FLAGS}")
else ()
    message(FATAL_ERROR "${PROJECT_NAME} unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

# dummy target used for file copies
add_custom_target(dummy-target ALL DEPENDS custom-output)
add_custom_command(OUTPUT custom-output COMMAND ${CMAKE_COMMAND} -E echo DEPENDS always-rebuild)
add_custom_command(OUTPUT always-rebuild COMMAND ${CMAKE_COMMAND} -E echo)

# settings
set (TESTS_DIR ${PROJECT_BINARY_DIR}/tests)
set (LOGS_DIR  ${PROJECT_BINARY_DIR}/logs)

# sub-projects
add_subdirectory(bc)
add_subdirectory(compiler)

# post build
add_custom_target(basecode-configured DEPENDS dummy-target compiler bc)
add_custom_command(
        TARGET basecode-configured

        COMMAND ${CMAKE_COMMAND} -E echo  "copy_directory: ${PROJECT_SOURCE_DIR}/tests"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/tests ${TESTS_DIR})
