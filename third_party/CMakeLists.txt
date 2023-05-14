cmake_minimum_required(VERSION 3.24)
project(luka_third_party)

# **********************************************************
# vulkan sdk
# **********************************************************
find_package(Vulkan REQUIRED GLOBAL)

# **********************************************************
# glfw
# **********************************************************
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)
add_library(glfw::glfw ALIAS glfw)

# **********************************************************
# glm
# **********************************************************
add_subdirectory(glm)

# **********************************************************
# stb image
# **********************************************************
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE stb_image)
add_library(stb_image::stb_image ALIAS stb_image)

# **********************************************************
# tiny obj loader
# **********************************************************
add_library(tiny_obj_loader INTERFACE)
target_include_directories(tiny_obj_loader INTERFACE tiny_obj_loader)
add_library(tiny_obj_loader::tiny_obj_loader ALIAS tiny_obj_loader)