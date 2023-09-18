cmake_minimum_required(VERSION 3.6)

add_library(json11 json11.cpp)
target_include_directories(json11 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
