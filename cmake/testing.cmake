enable_testing()

# tests(<name> [DEFINES ...] [INCLUDES ...] [LIBRARIES ...])
# builds <name>-tests from tests/*.c and registers each file as a test.
function(tests name)
	if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
		cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "DEFINES;INCLUDES;LIBRARIES")
		file(GLOB_RECURSE tests CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/tests tests/*.c)
		list(TRANSFORM tests PREPEND tests/ OUTPUT_VARIABLE test_sources)
		create_test_sourcelist(runner ${name}-tests.c ${tests})

		add_executable(${name}-tests ${name}-tests.c ${test_sources})
		set_target_properties(${name}-tests PROPERTIES C_STANDARD 23 CXX_STANDARD 23)
		target_compile_definitions(${name}-tests PRIVATE ${ARG_DEFINES})
		target_include_directories(${name}-tests PRIVATE ${ARG_INCLUDES})
		target_link_libraries(${name}-tests PRIVATE debug_flags ${ARG_LIBRARIES})

		foreach(test ${tests})
			cmake_path(REMOVE_EXTENSION test)
			add_test(
				NAME ${name}/${test}
				COMMAND ${name}-tests ${test}
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
		endforeach()
	endif()
endfunction()

