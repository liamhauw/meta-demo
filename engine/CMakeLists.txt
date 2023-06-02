cmake_minimum_required(VERSION 3.24)
project(luka_engine)

# **********************************************************
# platform
# **********************************************************
add_library(platform INTERFACE)
target_compile_features(platform INTERFACE cxx_std_20)
target_include_directories(platform INTERFACE include)
target_link_libraries(
        platform INTERFACE
        Vulkan::Vulkan
        glfw
        glm
        spdlog_header_only
        stb_image
        tinygltf
        tinyobjloader
)
add_dependencies(platform glsl2spirv)
add_library(luka_engine::platform ALIAS platform)

# **********************************************************
# core
# **********************************************************
configure_file("${CMAKE_SOURCE_DIR}/cmake/config.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/include/luka/core/generated/config.h")

add_library(core STATIC)
target_sources(
        core
        PRIVATE
        src/core/context.cc
        src/core/file.cc
        src/core/math.cc
        src/core/time.cc
)
target_link_libraries(core PUBLIC luka_engine::platform)
add_library(luka_engine::core ALIAS core)

# **********************************************************
# resource
# **********************************************************
add_library(resource STATIC)
target_sources(
        resource
        PRIVATE
        src/resource/gltf_model.cc
        src/resource/image.cc
        src/resource/obj_model.cc
        src/resource/resource_path.cc
)
target_link_libraries(resource PUBLIC luka_engine::core)
add_library(luka_engine::resource ALIAS resource)

# **********************************************************
# function
# **********************************************************
add_library(function STATIC)
target_sources(
        function
        PRIVATE
        src/function/context/context.cc
        src/function/window/input.cc
        src/function/window/window.cc
#        src/function/rendering.cc
)
target_link_libraries(function PUBLIC luka_engine::resource)
add_library(luka_engine::function ALIAS function)

# **********************************************************
# editor
# **********************************************************
add_library(editor STATIC)
target_sources(
        editor
        PRIVATE
        src/editor/editor.cc
)
target_link_libraries(editor PUBLIC luka_engine::function)
add_library(luka_engine::editor ALIAS editor)

# **********************************************************
# luka engine
# **********************************************************
add_executable(${PROJECT_NAME})
target_sources(
        ${PROJECT_NAME}
        PRIVATE
        src/engine.cc
        src/main.cc
        )
target_link_libraries(${PROJECT_NAME} PRIVATE luka_engine::editor)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")