cmake_minimum_required(VERSION 3.7)

project(ezgl)



# ======================================================================
# Options ==============================================================
# ======================================================================

# If enabled, it will support mesh IO but will need
option(EZGL_COMPILE_MESH_IO "Whether to compile Mesh IO or not. If enabled, assimp library will be required. ON by default. " ON)

# ======================================================================
# ======================================================================
# ======================================================================

# ======================================================================
# CMake config =========================================================
# ======================================================================

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ======================================================================
# ======================================================================
# ======================================================================

# ======================================================================
# ezgl library =========================================================
# ======================================================================

file(GLOB EZGL_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp")
add_library(ezgl SHARED ${EZGL_SRC_FILES})

target_compile_options(ezgl PRIVATE "-Wall")
target_compile_options(ezgl PRIVATE "-Wextra")
target_compile_options(ezgl PRIVATE "-Werror")
target_compile_features(ezgl PUBLIC "cxx_std_20")
target_compile_features(ezgl PUBLIC "cxx_aggregate_default_initializers")
target_compile_features(ezgl PUBLIC "cxx_attributes")
target_compile_features(ezgl PUBLIC "cxx_auto_type")
target_compile_features(ezgl PUBLIC "cxx_constexpr")
target_compile_features(ezgl PUBLIC "cxx_decltype")
target_compile_features(ezgl PUBLIC "cxx_decltype_auto")
target_compile_features(ezgl PUBLIC "cxx_explicit_conversions")
target_compile_features(ezgl PUBLIC "cxx_final")
target_compile_features(ezgl PUBLIC "cxx_lambdas")
target_compile_features(ezgl PUBLIC "cxx_noexcept")
target_compile_features(ezgl PUBLIC "cxx_nullptr")
target_compile_features(ezgl PUBLIC "cxx_override")
target_compile_features(ezgl PUBLIC "cxx_range_for")
target_compile_features(ezgl PUBLIC "cxx_rvalue_references")
target_compile_features(ezgl PUBLIC "cxx_variable_templates")
target_compile_features(ezgl PUBLIC "cxx_variadic_macros")
target_compile_features(ezgl PUBLIC "cxx_variadic_templates")
target_compile_features(ezgl PUBLIC "cxx_template_template_parameters")

target_include_directories(ezgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/Camera")
target_include_directories(ezgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/Generic")
target_include_directories(ezgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/GL")
target_include_directories(ezgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/Mesh")
target_include_directories(ezgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/Renderer")
target_include_directories(ezgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/ThirdParty")
target_include_directories(ezgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/UI")

# ======================================================================
# ======================================================================
# ======================================================================

# ======================================================================
# Defines ==============================================================
# ======================================================================

if (EZGL_COMPILE_MESH_IO)
target_compile_definitions(ezgl PRIVATE "MESH_IO")
endif()

# ======================================================================
# ======================================================================
# ======================================================================

# ======================================================================
# Dependencies =========================================================
# ======================================================================

# assimp
if (EZGL_COMPILE_MESH_IO)
  find_package(assimp REQUIRED)
  target_include_directories(ezgl PUBLIC ${assimp_INCLUDE_DIRS})
  target_link_libraries(ezgl ${assimp_LIBRARIES})
endif()

# OpenGL
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
target_include_directories(ezgl PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(ezgl ${OPENGL_LIBRARIES})

# GLEW
find_package(GLEW REQUIRED)
target_include_directories(ezgl PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(ezgl ${GLEW_LIBRARIES})

# GLFW
find_package(glfw3 REQUIRED)
target_link_libraries(ezgl glfw)

# ezcommon
if (NOT TARGET ezcommon)
  add_subdirectory(deps/ezcommon)
  target_link_libraries(ezgl ezcommon)
endif()

# ezmath
add_subdirectory(deps/ezmath)
target_link_libraries(ezgl ezmath)

# ======================================================================
# ======================================================================
# ======================================================================

# ======================================================================
# Tests ================================================================
# ======================================================================

add_subdirectory(tests)

# ======================================================================
# ======================================================================
# ======================================================================

# ======================================================================
# Examples =============================================================
# ======================================================================

add_subdirectory(examples)

# ======================================================================
# ======================================================================
# ======================================================================
