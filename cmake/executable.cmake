# executable(<name> LANGUAGES <language>... [VERSION v] [DESCRIPTION d] [URL u] [DEFINES ...] [LIBRARIES ...] [TESTING_LIBRARIES ...] [ENABLE_TESTING] [SKIP_INSTALL])
# source files come from source/, headers from include/, tests from tests/ and assets/ is installed alongside.
macro(executable name)
	cmake_parse_arguments(EXECUTABLE
		"ENABLE_TESTING;SKIP_INSTALL"
		"VERSION;DESCRIPTION;URL"
		"LANGUAGES;DEFINES;LIBRARIES;TESTING_LIBRARIES"
		${ARGN})

	if (NOT APPLE AND "OBJC" IN_LIST EXECUTABLE_LANGUAGES)
		return()
	endif()

	enable_language(${EXECUTABLE_LANGUAGES})
	executable_target(${ARGV})
endmacro()

function(executable_target name)
	cmake_parse_arguments(PARSE_ARGV 1 ARG
		"ENABLE_TESTING;SKIP_INSTALL"
		"VERSION;DESCRIPTION;URL"
		"LANGUAGES;DEFINES;LIBRARIES;TESTING_LIBRARIES")
	foreach(language ${ARG_LANGUAGES})
		list(TRANSFORM CMAKE_${language}_SOURCE_FILE_EXTENSIONS PREPEND source/*. OUTPUT_VARIABLE extensions)
		list(APPEND patterns ${extensions})
	endforeach()
	file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${patterns})

	add_executable(${name} ${sources})
	set_target_properties(${name} PROPERTIES
		C_STANDARD    23
		OBJC_STANDARD 23
		CXX_STANDARD  23)
	if(ARG_LANGUAGES STREQUAL ASM_NASM)
		set_target_properties(${name} PROPERTIES LINKER_LANGUAGE C)
	endif()

	target_compile_definitions(${name} PRIVATE ${ARG_DEFINES}
			PRIVATE_ASSETS="$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/assets/>$<INSTALL_INTERFACE:${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}/${name}/>"
			PROJECT_ASSETS="$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/assets/>$<INSTALL_INTERFACE:${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}/>")
	target_include_directories(${name} PRIVATE include)

	if (APPLE AND "OBJC" IN_LIST ARG_LANGUAGES)
		find_library(FOUNDATION_FRAMEWORK Foundation REQUIRED)
		find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
		find_library(CORE_FOUNDATION_LIBRARY CoreFoundation REQUIRED)
		find_library(QUARTZ_CORE_LIBRARY QuartzCore REQUIRED)
		list(APPEND ARG_LIBRARIES
			${FOUNDATION_FRAMEWORK}
			${COCOA_FRAMEWORK}
			${CORE_FOUNDATION_LIBRARY}
			${QUARTZ_CORE_LIBRARY})
	endif()

	list(APPEND ARG_LIBRARIES  $<$<CONFIG:Debug>:debug_flags;sanitizer>)
	target_link_libraries(${name} PRIVATE ${ARG_LIBRARIES})

	if (${ARG_ENABLE_TESTING})
		tests(${name}
			DEFINES ${ARG_DEFINES}
			INCLUDES include source
			LIBRARIES ${ARG_LIBRARIES} ${ARG_TESTING_LIBRARIES})
	endif()

	if (NOT ${ARG_SKIP_INSTALL})
		install(TARGETS ${name})
		install(
			DIRECTORY assets/
			DESTINATION ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}/${name} OPTIONAL)
	endif()
endfunction()
