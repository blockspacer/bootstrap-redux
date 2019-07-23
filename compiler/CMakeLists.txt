cmake_minimum_required(VERSION 3.15)
project(compiler)

message(STATUS "${PROJECT_NAME} build configuration:")

set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (PRODUCT_NAME  "Basecode Compiler Library")

configure_file (
        "${PROJECT_SOURCE_DIR}/configure.in.h"
        "${PROJECT_BINARY_DIR}/configure.h"
)

add_library(
        ${PROJECT_NAME}

        types.cpp types.h
)
target_link_libraries(
        ${PROJECT_NAME}
        utf8proc
        fmt-header-only
        boost-system
        boost-filesystem
        tcc
        mstch
)
