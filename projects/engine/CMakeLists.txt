set(DWMAPI "$<$<BOOL:${WIN32}>:dwmapi>")

library(engine
	LANGUAGES C
	PUBLIC_LIBRARIES glad glfw stb
	PRIVATE_LIBRARIES "${DWMAPI}"
	ENABLE_TESTING
	TESTING_LIBRARIES libuv::libuv
)

