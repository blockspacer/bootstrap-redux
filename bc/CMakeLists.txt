cmake_minimum_required(VERSION 3.15)
project(bc)

message(STATUS "${PROJECT_NAME} build configuration:")

add_executable(
        ${PROJECT_NAME}

        main.cpp)

target_link_libraries(
        ${PROJECT_NAME}

        compiler
        boost-program_options
)