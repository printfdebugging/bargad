add_library(debug_flags INTERFACE)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(DEBUG_FLAGS_NASM -g)
	set(DEBUG_FLAGS_SANITIZERS -fsanitize=undefined,address)
	set(DEBUG_FLAGS_C -Wstrict-prototypes -Wno-override-init)
	set(DEBUG_FLAGS_C_CXX
		-Wall -Wextra -Wpedantic
		-Wundef -Wshadow -Wpointer-arith -Wcast-align
		-Wwrite-strings -Wswitch-enum
		-Wconversion -Wunreachable-code
		-Wno-double-promotion -Wno-unused-parameter
		-Wno-empty-translation-unit
		-Werror=return-type
		-fno-omit-frame-pointer)

	if(NOT WIN32)
		list(APPEND DEBUG_FLAGS_C_CXX ${DEBUG_FLAGS_SANITIZERS})
		target_link_options(debug_flags INTERFACE ${DEBUG_FLAGS_SANITIZERS})
	endif()

	target_compile_definitions(debug_flags INTERFACE DEBUG)
	target_compile_options(debug_flags INTERFACE
		"$<$<COMPILE_LANGUAGE:C,CXX>:${DEBUG_FLAGS_C_CXX}>"
		"$<$<COMPILE_LANGUAGE:C>:${DEBUG_FLAGS_C}>"
		"$<$<COMPILE_LANGUAGE:ASM_NASM>:${DEBUG_FLAGS_NASM}>")
endif()
