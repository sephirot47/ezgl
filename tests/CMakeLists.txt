cmake_minimum_required(VERSION 3.7)

project(ezgl-tests)

add_executable(TestGeneric.exe "${CMAKE_CURRENT_SOURCE_DIR}/src/TestGeneric.cpp")
target_link_libraries(TestGeneric.exe ezgl)

add_executable(TestIntersections.exe "${CMAKE_CURRENT_SOURCE_DIR}/src/TestIntersections.cpp")
target_link_libraries(TestIntersections.exe ezgl)

add_executable(TestOctree.exe "${CMAKE_CURRENT_SOURCE_DIR}/src/TestOctree.cpp")
target_link_libraries(TestOctree.exe ezgl)

add_executable(TestRayIntersection.exe "${CMAKE_CURRENT_SOURCE_DIR}/src/TestRayIntersection.cpp")
target_link_libraries(TestRayIntersection.exe ezgl)

add_executable(TestText.exe "${CMAKE_CURRENT_SOURCE_DIR}/src/TestText.cpp")
target_link_libraries(TestText.exe ezgl)

add_executable(TestRayTracingCPU.exe "${CMAKE_CURRENT_SOURCE_DIR}/src/TestRayTracingCPU.cpp")
target_link_libraries(TestRayTracingCPU.exe ezgl)