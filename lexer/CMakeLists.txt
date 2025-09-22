include(${PROJECT_SOURCE_DIR}/cmake/export.cmake)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# library part of the lexer
add_library(lexer SHARED ${SOURCE_FILES})
add_library(volt::lexer ALIAS lexer)
target_compile_features(lexer
	PUBLIC cxx_std_23
	PRIVATE cxx_std_26
)
target_include_directories(lexer
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated/include>
)
target_link_libraries(lexer PUBLIC cpp-unicodelib::cpp-unicodelib volt::core)
generate_export_header(lexer
	PREFIX VOLT_LX
	HEADER_PATH ${CMAKE_CURRENT_BINARY_DIR}/generated/include/volt/lx/export.hpp
)
target_compile_options(lexer PRIVATE -Wall -Wextra -Wpedantic)


# executable part of the lexer
add_executable(lexer-exe ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
add_executable(volt::lexer-exe ALIAS lexer-exe)
target_link_libraries(lexer-exe PRIVATE volt::lexer)
target_compile_options(lexer-exe PRIVATE -Wall -Wextra -Wpedantic)
