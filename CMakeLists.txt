cmake_minimum_required(VERSION 4.0)
project(farmhouse C)

set(VENDOR_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vendor")
set(SCRIPTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(PROJECTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/projects")

list(APPEND CMAKE_MODULE_PATH "${SCRIPTS_PATH}")

include(GNUInstallDirs)
include(config)
include(packaging)
include(clangd)
include(debug-flags)
include(testing)
include(executable)
include(library)
include(vendor)


file(GLOB_RECURSE project_build_files
	CONFIGURE_DEPENDS
	LIST_DIRECTORIES false "${PROJECTS_PATH}/*/CMakeLists.txt")

foreach(project_build_file ${project_build_files})
	get_filename_component(project_directory "${project_build_file}" DIRECTORY)
	add_subdirectory("${project_directory}")
endforeach()

