cmake_minimum_required(VERSION 3.24)
project(luka_engine)

set(LUKA_HEADER_FILE "${CMAKE_BINARY_DIR}/header_file.json")

configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/source_path.h.in"
        "${CMAKE_BINARY_DIR}/generated/luka/source_path.h"
)
file(GLOB_RECURSE HEADER_FILE "*.h")
file(GLOB_RECURSE SOURCE_FILE "*.cc")
configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/header_file.json.in"
        ${LUKA_HEADER_FILE}
)
add_custom_target(
        run_meta_parser
        COMMAND meta_parser ${LUKA_HEADER_FILE} "${CMAKE_BINARY_DIR}/parser_header.h" "${CMAKE_BINARY_DIR}/generated/luka" "*" "luka" 0
)
add_dependencies(run_meta_parser meta_parser)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILE})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR}/generated)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME} PRIVATE third_party)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
add_dependencies(${PROJECT_NAME} run_meta_parser)