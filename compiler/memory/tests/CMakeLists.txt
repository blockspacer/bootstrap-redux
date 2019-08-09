cmake_minimum_required(VERSION 3.15)
project(test-memory)

add_executable(${PROJECT_NAME} main.cpp memory_tests.cpp)
target_link_libraries(${PROJECT_NAME} compiler)

enable_testing()
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})