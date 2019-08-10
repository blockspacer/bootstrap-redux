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

# pstreams
include_directories(ext/pstreams-1.0.1)

# utf8proc
add_subdirectory(ext/utf8proc-2.4.0 EXCLUDE_FROM_ALL)
include_directories(ext/utf8proc-2.4.0)

# libfmt
add_subdirectory(ext/fmt-5.3.0 EXCLUDE_FROM_ALL)

# hat-trie
add_subdirectory(ext/hat-trie-0.6.0 EXCLUDE_FROM_ALL)
include_directories(ext/hat-trie-0.6.0/include)

# boost
include_directories(ext/boost-1.70.0)

# boost system library
add_library(boost-system ext/boost-1.70.0/libs/system/src/error_code.cpp)

# boost filesystem library
set(
        BOOST_FILESYSTEM_SOURCES
        ext/boost-1.70.0/libs/filesystem/src/codecvt_error_category.cpp
        ext/boost-1.70.0/libs/filesystem/src/operations.cpp
        ext/boost-1.70.0/libs/filesystem/src/path.cpp
        ext/boost-1.70.0/libs/filesystem/src/path_traits.cpp
        ext/boost-1.70.0/libs/filesystem/src/portability.cpp
        ext/boost-1.70.0/libs/filesystem/src/unique_path.cpp
        ext/boost-1.70.0/libs/filesystem/src/utf8_codecvt_facet.cpp)
if (WIN32)
    set(
            BOOST_FILESYSTEM_SOURCES
            ${BOOST_FILESYSTEM_SOURCES}
            ext/boost-1.70.0/libs/filesystem/src/windows_file_codecvt.cpp)
endif()
add_library(boost-filesystem ${BOOST_FILESYSTEM_SOURCES})

# boost program_options library
set(
        BOOST_PROGRAM_OPTIONS_SOURCES
        ext/boost-1.70.0/libs/program_options/src/cmdline.cpp
        ext/boost-1.70.0/libs/program_options/src/config_file.cpp
        ext/boost-1.70.0/libs/program_options/src/convert.cpp
        ext/boost-1.70.0/libs/program_options/src/options_description.cpp
        ext/boost-1.70.0/libs/program_options/src/parsers.cpp
        ext/boost-1.70.0/libs/program_options/src/positional_options.cpp
        ext/boost-1.70.0/libs/program_options/src/split.cpp
        ext/boost-1.70.0/libs/program_options/src/utf8_codecvt_facet.cpp
        ext/boost-1.70.0/libs/program_options/src/value_semantic.cpp
        ext/boost-1.70.0/libs/program_options/src/variables_map.cpp)
if (WIN32)
    set(
            BOOST_PROGRAM_OPTIONS_SOURCES
            ${BOOST_PROGRAM_OPTIONS_SOURCES}
            ext/boost-1.70.0/libs/program_options/src/winmain.cpp)
endif()
add_library(boost-program_options ${BOOST_PROGRAM_OPTIONS_SOURCES})

# catch2
include_directories(ext/catch2-2.9.1/single_include)

# spdlog
include_directories(ext/spdlog-1.3.1/include)

# tinycc
set(
        TINYCC_SOURCES
        ext/tinycc-mob/libtcc.c
        ext/tinycc-mob/tccpp.c
        ext/tinycc-mob/tccgen.c
        ext/tinycc-mob/tccelf.c
        ext/tinycc-mob/tccasm.c
        ext/tinycc-mob/tccrun.c
        ext/tinycc-mob/x86_64-gen.c
        ext/tinycc-mob/x86_64-link.c
        ext/tinycc-mob/i386-asm.c
)
add_library(tcc ${TINYCC_SOURCES})

# XXX: eventually add conditionals around all of these defines
target_compile_definitions(tcc PUBLIC TCC_TARGET_X86_64 ONE_SOURCE=0 _FORTIFY_SOURCE=0)

if (APPLE)
    target_compile_definitions(tcc PUBLIC TCC_TARGET_MACHO)
endif()

add_executable(tinycc ext/tinycc-mob/tcc.c)
target_link_libraries(tinycc tcc)

# entt
include_directories(ext/entt-master/single_include)

# mstch
add_subdirectory(ext/mstch-1.0.2)
include_directories(ext/mstch-1.0.2/include)

# our project-level includes
include_directories(${PROJECT_SOURCE_DIR})

# sub-projects
add_subdirectory(bc)
add_subdirectory(compiler)

# post build
add_custom_target(basecode-configured DEPENDS dummy-target compiler bc tinycc test-core-lexer test-core-parser)
add_custom_command(
        TARGET basecode-configured

        COMMAND ${CMAKE_COMMAND} -E echo  "copy_directory: ${PROJECT_SOURCE_DIR}/tests"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/tests ${TESTS_DIR})
