cmake_minimum_required(VERSION 3.24)
project(luka_third_party)

set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

# **********************************************************
# vulkan sdk
# **********************************************************
find_package(Vulkan REQUIRED GLOBAL)

# **********************************************************
# glfw
# **********************************************************
set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)

# **********************************************************
# glm
# **********************************************************
add_subdirectory(glm)

# **********************************************************
# json11
# **********************************************************
add_subdirectory(json11)

# **********************************************************
# spdlog
# **********************************************************
add_subdirectory(spdlog)

# **********************************************************
# stb image
# **********************************************************
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE stb_image)

# **********************************************************
# tiny gltf
# **********************************************************
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF)
set(TINYGLTF_BUILD_GL_EXAMPLES OFF)
set(TINYGLTF_BUILD_VALIDATOR_EXAMPLE OFF)
set(TINYGLTF_BUILD_BUILDER_EXAMPLE OFF)
set(TINYGLTF_HEADER_ONLY ON)
set(TINYGLTF_INSTALL OFF)
add_subdirectory(tiny_gltf)

# **********************************************************
# tiny obj loader
# **********************************************************
set(TINYOBJLOADER_USE_DOUBLE OFF)
set(TINYOBJLOADER_BUILD_TEST_LOADER OFF)
set(TINYOBJLOADER_BUILD_OBJ_STICHER OFF)
add_subdirectory(tiny_obj_loader)

# **********************************************************
# third_party
# **********************************************************
add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(
  ${PROJECT_NAME} INTERFACE
  Vulkan::Vulkan
  glfw
  glm
  json11
  spdlog_header_only
  stb_image
  tinygltf
  tinyobjloader
)
set(CMAKE_WARN_DEPRECATED TRUE CACHE BOOL "" FORCE)