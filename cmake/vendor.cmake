file(GLOB vendor_build_files
	CONFIGURE_DEPENDS
	LIST_DIRECTORIES false "${VENDOR_PATH}/*/CMakeLists.txt")

foreach(vendor_build_file ${vendor_build_files})
	get_filename_component(vendor_directory "${vendor_build_file}" DIRECTORY)
	add_subdirectory("${vendor_directory}")
endforeach()
