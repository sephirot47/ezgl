cmake_minimum_required(VERSION 3.7)

project(ezgl-tests)

add_executable(test.exe "${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp")
target_link_libraries(test.exe ezgl)

