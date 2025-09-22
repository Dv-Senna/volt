include(${PROJECT_SOURCE_DIR}/cmake/export.cmake)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# core library of Volt compiler
add_library(core SHARED ${SOURCE_FILES})
add_library(volt::core ALIAS core)
target_compile_features(core
	PUBLIC cxx_std_23
	PRIVATE cxx_std_26
)
target_include_directories(core
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated/include>
)
generate_export_header(core
	PREFIX VOLT_CORE
	HEADER_PATH ${CMAKE_CURRENT_BINARY_DIR}/generated/include/volt/core/export.hpp
)
target_compile_options(core PRIVATE -Wall -Wextra -Wpedantic)
