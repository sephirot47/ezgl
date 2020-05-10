cmake_minimum_required(VERSION 3.7)

project(ezgl-examples)

add_executable(HelloWorld.exe "${CMAKE_CURRENT_SOURCE_DIR}/HelloWorld.cpp")
target_link_libraries(HelloWorld.exe ezgl)