cmake_minimum_required(VERSION 3.20)

project(volt VERSION 0.1.0 LANGUAGES CXX)

include(cmake/cpp-unicodelib.cmake)

add_subdirectory(core)
add_subdirectory(lexer)
add_subdirectory(parser)
