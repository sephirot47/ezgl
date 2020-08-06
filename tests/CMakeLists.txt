cmake_minimum_required(VERSION 3.7)

project(ezgl-tests)

file(GLOB TEST_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

foreach(SRC_FILE IN LISTS TEST_SRC_FILES)

  get_filename_component(SRC_FILE_NAME ${SRC_FILE} NAME_WE)
  add_executable(${SRC_FILE_NAME}.exe "${SRC_FILE}")
  target_link_libraries(${SRC_FILE_NAME}.exe ezgl)

endforeach(SRC_FILE)