cmake_minimum_required(VERSION 3.24)
project(luka_engine)

set(CORE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/core)
set(RESOURCE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/resource)
set(FUNCTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/function)
set(EDITOR_FILE ${CMAKE_CURRENT_SOURCE_DIR}/editor)
set(ENGINE_FILE ${CMAKE_CURRENT_SOURCE_DIR})

configure_file(
        "${RESOURCE_FILE}/config/source_path.h.in"
        "${RESOURCE_FILE}/config/generated/source_path.h"
)

set(
        HEADER_FILE
        # core
        ${CORE_FILE}/meta/reflection/reflection.h
        ${CORE_FILE}/meta/reflection/reflection_register.h
        ${CORE_FILE}/meta/serializer/serializer.h

        ${CORE_FILE}/math/math.h
        ${CORE_FILE}/math/math_headers.h
        ${CORE_FILE}/math/math_marcos.h
        ${CORE_FILE}/math/matrix3.h
        ${CORE_FILE}/math/matrix4.h
        ${CORE_FILE}/math/quaternion.h
        ${CORE_FILE}/math/random.h
        ${CORE_FILE}/math/transform.h
        ${CORE_FILE}/math/vector2.h
        ${CORE_FILE}/math/vector3.h
        ${CORE_FILE}/math/vector4.h
        ${CORE_FILE}/math/axis_aligned.h

        ${CORE_FILE}/context.h
        ${CORE_FILE}/log.h
        ${CORE_FILE}/math.h
        ${CORE_FILE}/time.h
        # resource
        ${RESOURCE_FILE}/asset/asset.h
        ${RESOURCE_FILE}/config/generated/source_path.h
        ${RESOURCE_FILE}/config/config.h
        ${RESOURCE_FILE}/res_type/common/world.h
        ${RESOURCE_FILE}/res_type/common/level.h
        # function
        ${FUNCTION_FILE}/input/input.h
        ${FUNCTION_FILE}/rendering/rendering.h
        ${FUNCTION_FILE}/rendering/vulkan_rhi.h
        ${FUNCTION_FILE}/rendering/vulkan_util.h
        ${FUNCTION_FILE}/window/window.h
        ${FUNCTION_FILE}/framework/common/world.h
        ${FUNCTION_FILE}/framework/common/object.h
        ${FUNCTION_FILE}/framework/common/object_id_allocator.h
        ${FUNCTION_FILE}/framework/component/component.h
        # editor
        ${EDITOR_FILE}/editor.h
        # engine
        ${ENGINE_FILE}/engine.h
)

set(
        SOURCE_FILE
        # core
        ${CORE_FILE}/math/axis_aligned.cc
        ${CORE_FILE}/math/math.cc
        ${CORE_FILE}/math/matrix3.cc
        ${CORE_FILE}/math/matrix4.cc
        ${CORE_FILE}/math/quaternion.cc
        ${CORE_FILE}/math/vector2.cc
        ${CORE_FILE}/math/vector3.cc
        ${CORE_FILE}/math/vector4.cc
        ${CORE_FILE}/meta/reflection/reflection.cc
        ${CORE_FILE}/meta/reflection/reflection_register.cc
        ${CORE_FILE}/meta/serializer/serializer.cc
        ${CORE_FILE}/context.cc
        ${CORE_FILE}/math.cc
        ${CORE_FILE}/time.cc
        # resource
        ${RESOURCE_FILE}/asset/asset.cc
        ${RESOURCE_FILE}/config/config.cc
        ${RESOURCE_FILE}/res_type/common/world.cc
        ${RESOURCE_FILE}/res_type/common/level.cc
        # function
        ${FUNCTION_FILE}/input/input.cc
        ${FUNCTION_FILE}/rendering/rendering.cc
        ${FUNCTION_FILE}/rendering/vulkan_rhi.cc
        ${FUNCTION_FILE}/rendering/vulkan_util.cc
        ${FUNCTION_FILE}/window/window.cc
        ${FUNCTION_FILE}/framework/common/world.cc
        ${FUNCTION_FILE}/framework/common/object.cc
        ${FUNCTION_FILE}/framework/common/object_id_allocator.cc
        ${FUNCTION_FILE}/framework/component/component.cc
        # editor
        ${EDITOR_FILE}/editor.cc
        # engine
        ${ENGINE_FILE}/engine.cc
        ${ENGINE_FILE}/main.cc
)

add_subdirectory(${CORE_FILE}/meta/parser)
set(HEADER_FILE_JSON "${CORE_FILE}/meta/generated/header_file.json")
configure_file(
        "${CORE_FILE}/meta/header_file.json.in"
        ${HEADER_FILE_JSON}
)
add_custom_target(
        run_meta_parser ALL
        COMMAND meta_parser ${HEADER_FILE_JSON} "${CORE_FILE}/meta/generated/parser_header.h" "${CORE_FILE}/meta/generated" "*" "luka" 0
)
add_dependencies(run_meta_parser meta_parser)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILE})
target_include_directories(${PROJECT_NAME} PRIVATE ${ENGINE_FILE})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME} PRIVATE luka_third_party)
add_dependencies(${PROJECT_NAME} run_meta_parser)