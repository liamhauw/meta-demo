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
  # meta must be the first.
  # --------------------------------------------------
  core/meta/reflection/reflection.h
  core/meta/reflection/reflection_register.h
  core/meta/serializer/serializer.h
  # --------------------------------------------------
  core/context/context.h
  core/util/log.h
  core/util/time.h

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

  # engine
  engine.h
)

set(
  SOURCE_FILE

  # core
  # meta must be the first.
  # --------------------------------------------------
  core/meta/reflection/reflection.cc
  core/meta/reflection/reflection_register.cc
  core/meta/serializer/serializer.cc
  # --------------------------------------------------
  core/context/context.cc
  core/util/time.cc

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

if(CMAKE_HOST_WIN32)
  set(SYS_INCLUDE "n")
elseif(CMAKE_HOST_APPLE)
  find_program(XCRUN_EXECUTABLE xcrun)
  execute_process(
    COMMAND ${XCRUN_EXECUTABLE} --sdk macosx --show-sdk-platform-path
    OUTPUT_VARIABLE OSX_SDK_PLATFORM_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  set(SYS_INCLUDE "${OSX_SDK_PLATFORM_PATH}/../../Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1")
endif()

add_custom_target(
  run_meta_parser ALL
  COMMAND
  meta_parser
  "${CMAKE_CURRENT_SOURCE_DIR}/core/meta/generated/header_file.json"
  "${CMAKE_CURRENT_SOURCE_DIR}/core/meta/generated/parser_header.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/core/meta/generated"
  "${CMAKE_CURRENT_SOURCE_DIR}"
  ${SYS_INCLUDE}
)
add_dependencies(run_meta_parser meta_parser)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILE})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME} PRIVATE luka_third_party)
add_dependencies(${PROJECT_NAME} run_meta_parser)