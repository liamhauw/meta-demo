cmake_minimum_required(VERSION 3.24)
project(meta_parser)

set(
  HEADER_FILE
  parser/common/namespace.h
  parser/common/precompiled.h
  parser/common/schema_module.h
  parser/cursor/cursor.h
  parser/cursor/cursor_type.h
  parser/generator/generator.h
  parser/generator/reflection_generator.h
  parser/generator/serializer_generator.h
  parser/language_types/class.h
  parser/language_types/field.h
  parser/language_types/method.h
  parser/language_types/type_info.h
  parser/meta/meta_data_config.h
  parser/meta/meta_info.h
  parser/meta/meta_utils.h
  parser/parser/parser.h
  parser/template_manager/template_manager.h
)

set(
  SOURCE_FILE
  parser/cursor/cursor.cc
  parser/cursor/cursor_type.cc
  parser/generator/generator.cc
  parser/generator/reflection_generator.cc
  parser/generator/serializer_generator.cc
  parser/language_types/class.cc
  parser/language_types/field.cc
  parser/language_types/method.cc
  parser/language_types/type_info.cc
  parser/meta/meta_info.cc
  parser/meta/meta_utils.cc
  parser/parser/parser.cc
  parser/template_manager/template_manager.cc
  parser/main.cc
)

add_executable(${PROJECT_NAME} ${HEADER_FILE} ${SOURCE_FILE})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/third_party/llvm1/include ${CMAKE_SOURCE_DIR}/third_party/mustache ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/parser)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(CMAKE_HOST_WIN32)
  set(LLVM_SHARED_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/third_party/llvm1/bin/x64)
  target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/third_party/llvm1/lib/x64/libclang.lib)
elseif(CMAKE_HOST_APPLE)
  set(LLVM_SHARED_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/third_party/llvm1/bin/macOS)
  set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O3")
  target_link_libraries(${PROJECT_NAME} ${LLVM_SHARED_LIBRARY_DIR}/libclang.dylib)
endif()

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${LLVM_SHARED_LIBRARY_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
