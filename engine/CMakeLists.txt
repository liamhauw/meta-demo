cmake_minimum_required(VERSION 3.24)
project(luka_engine)

# **********************************************************
#                         platform
# **********************************************************
add_library(platform STATIC)
target_compile_features(platform PUBLIC cxx_std_20)
target_include_directories(platform PUBLIC include)
target_sources(platform
        PRIVATE
        src/platform/platform.cc)

target_link_libraries(platform PUBLIC
        Vulkan::Vulkan
        glfw::glfw
        glm::glm
        stb_image::stb_image
        tiny_obj_loader::tiny_obj_loader
        )
add_dependencies(platform glsl2spirv)

add_library(luka_engine::platform ALIAS platform)

# **********************************************************
#                          core
# **********************************************************
add_library(core STATIC)
target_sources(core
        PRIVATE
        src/core/core.cc
        src/core/file.cc)
target_link_libraries(core
        PUBLIC
        luka_engine::platform)
add_library(luka_engine::core ALIAS core)

# **********************************************************
#                         resource
# **********************************************************
add_library(resource STATIC)
target_sources(resource
        PRIVATE
        src/resource/resource.cc)
target_link_libraries(resource
        PUBLIC
        luka_engine::core)
add_library(luka_engine::resource ALIAS resource)

# **********************************************************
#                         function
# **********************************************************
add_library(function STATIC)
target_sources(function
        PRIVATE
        src/function/rendering.cc
        src/function/function.cc)
target_link_libraries(function
        PUBLIC
        luka_engine::resource
        )
add_library(luka_engine::function ALIAS function)

# **********************************************************
#                          editor
# **********************************************************
add_library(editor STATIC)
target_sources(editor
        PRIVATE
        src/editor/editor.cc)
target_link_libraries(editor
        PUBLIC
        luka_engine::function)
add_library(luka_engine::editor ALIAS editor)

# **********************************************************
#                       luka engine
# **********************************************************
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
        PRIVATE
        src/engine.cc
        src/main.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE luka_engine::editor)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")