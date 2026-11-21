set(DWMAPI "$<$<BOOL:${WIN32}>:dwmapi>")

library(engine
	LANGUAGES C
	PUBLIC_LIBRARIES glad glfw
	PRIVATE_LIBRARIES "${DWMAPI}"
)

