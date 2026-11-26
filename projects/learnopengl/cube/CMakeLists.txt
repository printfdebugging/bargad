set(ASSETS "${CMAKE_CURRENT_LIST_DIR}/assets")

executable(cube
	LANGUAGES C
	LIBRARIES engine
	DEFINES VERTEX_SHADER="${ASSETS}/cube.vert" FRAGMENT_SHADER="${ASSETS}/cube.frag"
)
