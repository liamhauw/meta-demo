cmake_minimum_required(VERSION 3.24)
project(meta_parser)

#set(
#        PARSER_HEADER_FILE
#
#)

file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/LLVM/include ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/mustache ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/parser)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if (CMAKE_HOST_WIN32)
    set(LLVM_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/LLVM/lib/x64)
    set(LLVM_SHARED_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/LLVM/bin/x64)
    target_link_libraries(${PROJECT_NAME} ${LLVM_LIBRARY_DIR}/libclang.lib)
else ()
    set(LLVM_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/LLVM/lib/macOS)
    set(LLVM_SHARED_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/LLVM/bin/macOS)
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O3")
    target_link_libraries(${PROJECT_NAME} ${LLVM_SHARED_LIBRARY_DIR}/libclang.dylib)
endif ()

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${LLVM_SHARED_LIBRARY_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

