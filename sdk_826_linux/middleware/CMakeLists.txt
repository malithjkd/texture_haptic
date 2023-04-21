cmake_minimum_required(VERSION 2.8.6)
project (s826)

add_compile_definitions(_WIN32_MINGW)
add_compile_definitions(S826_CC=)
add_compile_definitions(S826_EXPORTS)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/)
add_library(s826 MODULE SHARED 826lib.c 826drv.c s826.cpp)


target_link_libraries(s826 setupapi)
