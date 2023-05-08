cmake_minimum_required(VERSION 3.25)
project(luka_third_party)

# **********************************************************
# vulkan sdk
# **********************************************************
find_package(Vulkan 
  REQUIRED 
  COMPONENTS glslang shaderc_combined
  GLOBAL)

# **********************************************************
# glfw
# **********************************************************
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)

# **********************************************************
# glm
# **********************************************************
add_subdirectory(glm)