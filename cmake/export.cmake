include(GenerateExportHeader)
include(CMakeParseArguments)

function(generate_export_header target)
	cmake_parse_arguments(ARG
		""
		"PREFIX;HEADER_PATH"
		""
		${ARGN}
	)

	if (NOT ARG_PREFIX)
		message(FATAL_ERROR "The PREFIX argument must be set when calling generate_export_header")
	endif()
	if (NOT ARG_HEADER_PATH)
		message(FATAL_ERROR "The HEADER_PATH argument must be set when calling generate_export_header")
	endif()

	set(GENERATE_EXPORT_HEADERS_UNDEF_DEPRECATION "
#undef ${ARG_PREFIX}_DEPRECATED
#undef ${ARG_PREFIX}_DEPRECATED_EXPORT
#undef ${ARG_PREFIX}_DEPRECATED_NO_EXPORT"
	)
	_generate_export_header(${target}
	BASE_NAME ${ARG_PREFIX}
		EXPORT_FILE_NAME ${ARG_HEADER_PATH}
		EXPORT_MACRO_NAME ${ARG_PREFIX}_EXPORT
		NO_EXPORT_MACRO_NAME ${ARG_PREFIX}_NO_EXPORT
		STATIC_DEFINE ${ARG_PREFIX}_STATIC_DEFINE
		CUSTOM_CONTENT_FROM_VARIABLE GENERATE_EXPORT_HEADERS_UNDEF_DEPRECATION
	)
endfunction()
