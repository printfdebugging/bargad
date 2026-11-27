get_filename_component(name "${CMAKE_CURRENT_LIST_DIR}" NAME)
executable("${name}"
	LANGUAGES C
	LIBRARIES engine)
