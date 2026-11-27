get_filename_component(name "${CMAKE_CURRENT_LIST_DIR}" NAME)
executable("gl-${name}"
	LANGUAGES C
	LIBRARIES engine)
