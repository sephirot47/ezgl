cmake_minimum_required(VERSION 3.7)

project(ezgl)

file(GLOB SRC_FILES "src/*.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")

add_executable(test.exe "${SRC_FILES}")

find_package(OpenGL REQUIRED)
target_include_directories(test.exe PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(test.exe ${OPENGL_LIBRARIES})

find_package(GLEW REQUIRED)
target_include_directories(test.exe PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(test.exe ${GLEW_LIBRARIES})

find_package(glfw3 REQUIRED)
target_link_libraries(test.exe glfw)

