cmake_minimum_required(VERSION 3.24)
project(luka_engine)

file(REMOVE_RECURSE "${CMAKE_CURRENT_SOURCE_DIR}/resource/config/generated")
configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/resource/config/source_path.h.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/resource/config/generated/source_path.h"
)

set(
        HEADER_FILE
        # core
        # meta must be the first, otherwise it won't reflect the previous header, i don't know.
        # --------------------------------------------------
        core/meta/reflection/reflection.h
        core/meta/reflection/reflection_register.h
        core/meta/serializer/serializer.h
        # --------------------------------------------------
        core/math/math.h
        core/math/math_headers.h
        core/math/math_marcos.h
        core/math/matrix3.h
        core/math/matrix4.h
        core/math/quaternion.h
        core/math/random.h
        core/math/transform.h
        core/math/vector2.h
        core/math/vector3.h
        core/math/vector4.h
        core/math/axis_aligned.h
        core/context.h
        core/log.h
        core/math.h
        core/time.h
        # resource
        resource/asset/asset.h
        resource/config/generated/source_path.h
        resource/config/config.h
        resource/res_type/common/world.h
        resource/res_type/common/level.h
        # function
        function/input/input.h
        function/rendering/rendering.h
        function/rendering/vulkan_rhi.h
        function/rendering/vulkan_util.h
        function/window/window.h
        function/framework/common/world.h
        function/framework/common/object.h
        function/framework/common/object_id_allocator.h
        function/framework/component/component.h
        # editor
        editor/editor.h
        # engine
        engine.h
)

set(
        SOURCE_FILE
        # core
        core/meta/reflection/reflection.cc
        core/meta/reflection/reflection_register.cc
        core/meta/serializer/serializer.cc
        core/math/axis_aligned.cc
        core/math/math.cc
        core/math/matrix3.cc
        core/math/matrix4.cc
        core/math/quaternion.cc
        core/math/vector2.cc
        core/math/vector3.cc
        core/math/vector4.cc
        core/context.cc
        core/math.cc
        core/time.cc
        # resource
        resource/asset/asset.cc
        resource/config/config.cc
        resource/res_type/common/world.cc
        resource/res_type/common/level.cc
        # function
        function/input/input.cc
        function/rendering/rendering.cc
        function/rendering/vulkan_rhi.cc
        function/rendering/vulkan_util.cc
        function/window/window.cc
        function/framework/common/world.cc
        function/framework/common/object.cc
        function/framework/common/object_id_allocator.cc
        function/framework/component/component.cc
        # editor
        editor/editor.cc
        # engine
        engine.cc
        main.cc
)

add_subdirectory(core/meta/parser)

file(REMOVE_RECURSE "${CMAKE_CURRENT_SOURCE_DIR}/core/meta/generated")
configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/core/meta/header_file.json.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/meta/generated/header_file.json"
)
if (CMAKE_HOST_WIN32)
  set(sys_include "*") 
elseif(CMAKE_HOST_APPLE)
  find_program(XCRUN_EXECUTABLE xcrun)
  if(NOT XCRUN_EXECUTABLE)
          message(FATAL_ERROR "xcrun not found!!!")
  endif()

  execute_process(
          COMMAND ${XCRUN_EXECUTABLE} --sdk macosx --show-sdk-platform-path
          OUTPUT_VARIABLE osx_sdk_platform_path_test
          OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  set(sys_include "${osx_sdk_platform_path_test}/../../Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1") 
  endif()

add_custom_target(
        run_meta_parser ALL
        COMMAND
        meta_parser
        "${CMAKE_CURRENT_SOURCE_DIR}/core/meta/generated/header_file.json"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/meta/generated/parser_header.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/meta/generated"
        ${sys_include}
        "${CMAKE_CURRENT_SOURCE_DIR}"
)
add_dependencies(run_meta_parser meta_parser)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILE})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME} PRIVATE luka_third_party)
add_dependencies(${PROJECT_NAME} run_meta_parser)