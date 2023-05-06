cmake_minimum_required(VERSION 3.20)
project(luka_third_party)

# **********************************************************
#                        vulkan sdk
# **********************************************************
find_package(Vulkan)
if (NOT Vulkan_FOUND) 
  message(FATAL_ERROR "fail to find vulkan sdk")
endif()
set(VULKAN_INCLUDE_DIRS ${Vulkan_INCLUDE_DIRS} CACHE STRING "")
set(VULKAN_LIBRARIES ${Vulkan_LIBRARIES} CACHE STRING "")

# **********************************************************
#                          glfw
# **********************************************************
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)

