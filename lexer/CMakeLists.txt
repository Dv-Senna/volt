include(GenerateExportHeader)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# library part of the lexer
add_library(lexer SHARED ${SOURCE_FILES})
add_library(volt::lexer ALIAS lexer)
target_compile_features(lexer PUBLIC cxx_std_23)
target_include_directories(lexer
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated/include>
)
set(GENERATE_EXPORT_HEADERS_UNDEF_DEPRECATION "
#undef VOLT_LX_DEPRECATED
#undef VOLT_LX_DEPRECATED_EXPORT
#undef VOLT_LX_DEPRECATED_NO_EXPORT"
)
generate_export_header(lexer
	BASE_NAME VOLT_LX
	EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/generated/include/volt/lx/export.hpp
	EXPORT_MACRO_NAME VOLT_LX_EXPORT
	NO_EXPORT_MACRO_NAME VOLT_LX_NO_EXPORT
	STATIC_DEFINE VOLT_LX_STATIC_DEFINE
	CUSTOM_CONTENT_FROM_VARIABLE GENERATE_EXPORT_HEADERS_UNDEF_DEPRECATION
)


# executable part of the lexer
add_executable(lexer-exe ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
add_executable(volt::lexer-exe ALIAS lexer-exe)
target_link_libraries(lexer-exe PRIVATE volt::lexer)
