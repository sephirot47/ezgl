cmake_minimum_required(VERSION 3.7)

project(ezgl)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp")

# TODO: change this only for ezgl
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")

add_compile_options("-Werror")

add_library(ezgl SHARED ${SRC_FILES})

target_include_directories(ezgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/Camera")
target_include_directories(ezgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/Generic")
target_include_directories(ezgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/GL")
target_include_directories(ezgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/Math")
target_include_directories(ezgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/Mesh")
target_include_directories(ezgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/Renderer")
target_include_directories(ezgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/ThirdParty")
target_include_directories(ezgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/UI")

find_package(assimp REQUIRED)
target_include_directories(ezgl PUBLIC ${assimp_INCLUDE_DIRS})
target_link_libraries(ezgl ${assimp_LIBRARIES})

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
target_include_directories(ezgl PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(ezgl ${OPENGL_LIBRARIES})

find_package(GLEW REQUIRED)
target_include_directories(ezgl PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(ezgl ${GLEW_LIBRARIES})

find_package(glfw3 REQUIRED)
target_link_libraries(ezgl glfw)

