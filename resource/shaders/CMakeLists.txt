cmake_minimum_required(VERSION 3.24)
project(luka_shader)

file(
        GLOB_RECURSE
        GLSL_SHADERS
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl/*.vert"
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl/*.comp"
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl/*.geom"
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl/*.tesc"
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl/*.tese"
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl/*.frag"
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl/*.comp"
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl/*.mesh"
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl/*.task"
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl/*.rgen"
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl/*.rint"
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl/*.rahit"
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl/*.rchit"
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl/*.rmiss"
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl/*.rcall"
)

set(GLSL_SHADER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(GENERATED_DIR "generated")
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${GENERATED_DIR})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${GENERATED_DIR})
endif ()

set(GENERATED_SPIRVS "")

foreach (GLSL_SHADER ${GLSL_SHADERS})
    get_filename_component(GLSL_SHADER_NAME ${GLSL_SHADER} NAME)
    set(SPRIV_SHADER "${CMAKE_CURRENT_SOURCE_DIR}/${GENERATED_DIR}/${GLSL_SHADER_NAME}.spirv")
    add_custom_command(
            OUTPUT ${SPRIV_SHADER}
            COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -I${GLSL_SHADER_INCLUDE_DIR} -V100 -o ${SPRIV_SHADER} ${GLSL_SHADER}
            DEPENDS ${GLSL_SHADER}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    list(APPEND GENERATED_SPIRVS ${SPRIV_SHADER})
endforeach ()

add_custom_target(glsl2spirv DEPENDS ${GENERATED_SPIRVS} SOURCES ${GLSL_SHADERS})
