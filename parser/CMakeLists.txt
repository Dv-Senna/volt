include(${PROJECT_SOURCE_DIR}/cmake/export.cmake)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# library part of the parser
add_library(parser SHARED ${SOURCE_FILES})
add_library(volt::parser ALIAS parser)
target_compile_features(parser
	PUBLIC cxx_std_26
)
target_include_directories(parser
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated/include>
)
target_link_libraries(parser PUBLIC volt::core volt::lexer)
generate_export_header(parser
	PREFIX VOLT_PARSER
	HEADER_PATH ${CMAKE_CURRENT_BINARY_DIR}/generated/include/volt/parser/export.hpp
)
target_compile_options(parser PRIVATE -Wall -Wextra -Wpedantic)


# executable part of the parser
add_executable(parser-exe ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
add_executable(volt::parser-exe ALIAS parser-exe)
target_link_libraries(parser-exe PRIVATE volt::parser)
target_compile_options(parser-exe PRIVATE -Wall -Wextra -Wpedantic)
