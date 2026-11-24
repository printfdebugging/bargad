#include "engine/window.h"
#include "engine/logger.h"

#ifdef _WIN32
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <dwmapi.h>
#	include "GLFW/glfw3native.h"
#endif

#ifdef _WIN32
static bool windows_get_dark_mode();
#endif

static void window_setup_callbacks(GLFWwindow *window, struct window_options options);
static void window_set_hints(struct window_options options);

#if defined(DEBUG)
static void opengl_debug_callback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char *message, const void *user_param);
static void window_setup_debug_callback();
#endif

GLFWwindow *
window_create(struct window_options opts)
{
#if defined(__APPLE__)
	LOG_ERROR("We do not support APPLE as APPLE doesn't support OpenGL 4.6\n");
	return NULL;
#else
	if(!glfwInit())
	{
		LOG_ERROR("Failed to initialize glfw\n");
		return NULL;
	}

	window_set_hints(opts);

	GLFWwindow *window = glfwCreateWindow(opts.properties.width, opts.properties.height, opts.properties.title, NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSwapInterval(1);

#	if defined(_WIN32)
	HWND hwnd = glfwGetWin32Window(window);
	DWORD value = windows_get_dark_mode();
	DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));
#	endif

	window_setup_callbacks(window, opts);
	glfwSetWindowUserPointer(window, opts.userdata);

	return window;
#endif
}

void
window_get_bounds(GLFWwindow *window, int *width, int *height)
{
	if(!window)
	{
		LOG_ERROR("Received a NULL window argument\n");
		return;
	}
	glfwGetWindowSize(window, width, height);
}

void
window_destroy(GLFWwindow *window)
{
	glfwDestroyWindow(window);
}

void
window_poll_events()
{
	glfwPollEvents();
}

void
window_swap_buffers(GLFWwindow *window)
{
	glfwSwapBuffers(window);
}

static void
window_setup_callbacks(GLFWwindow *window, struct window_options options)
{
	glfwSetCursorPosCallback(window, options.callbacks.cursor_position);
	glfwSetScrollCallback(window, options.callbacks.scroll);
	glfwSetFramebufferSizeCallback(window, options.callbacks.framebuffer_resize);
	glfwSetKeyCallback(window, options.callbacks.key);
	glfwSetErrorCallback(options.callbacks.error);

#if defined(DEBUG)
	window_setup_debug_callback();
#endif
}

static void
window_set_hints(struct window_options options)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, options.attributes.transparent);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, options.attributes.scalable);
	glfwWindowHint(GLFW_RESIZABLE, options.attributes.resizable);
	glfwWindowHint(GLFW_DECORATED, options.attributes.decorated);
	glfwWindowHint(GLFW_SAMPLES, options.attributes.samples);
#if defined(__APPLE__)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
#if defined(DEBUG)
	glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
#endif
}

#if defined(DEBUG)
static void
window_setup_debug_callback()
{
	int32_t context_flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
	if(context_flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(opengl_debug_callback, NULL);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
}

static void
opengl_debug_callback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char *message, const void *user_param)
{
	const char *debug_source;
	switch(source)
	{
		case GL_DEBUG_SOURCE_API:             debug_source = "source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   debug_source = "source: WINDOW_SYSTEM"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: debug_source = "source: SHADER_COMPILER"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     debug_source = "source: THIRD_PARTY"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     debug_source = "source: APPLICATION"; break;
		case GL_DEBUG_SOURCE_OTHER:           debug_source = "source: OTHER"; break;
	}

	const char *debug_type;
	switch(type)
	{
		case GL_DEBUG_TYPE_ERROR:               debug_type = "type: ERROR"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: debug_type = "type: DEPRECATED_BEHAVIOR"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  debug_type = "type: UNDEFINED_BEHAVIOR"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         debug_type = "type: PORTABILITY"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         debug_type = "type: PERFORMANCE"; break;
		case GL_DEBUG_TYPE_MARKER:              debug_type = "type: MARKER"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          debug_type = "type: PUSH_GROUP"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           debug_type = "type: POP_GROUP"; break;
		case GL_DEBUG_TYPE_OTHER:               debug_type = "type: OTHER"; break;
	}

	switch(severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         LOG_ERROR("[%s] [%s] message: %s\n", debug_source, debug_type, message); break;
		case GL_DEBUG_SEVERITY_MEDIUM:       LOG_ERROR("[%s] [%s] message: %s\n", debug_source, debug_type, message); break;
		case GL_DEBUG_SEVERITY_LOW:          LOG_WARNING("[%s] [%s] message: %s\n", debug_source, debug_type, message); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_INFO("[%s] [%s] message: %s\n", debug_source, debug_type, message); break;
	}
}
#endif

#ifdef _WIN32
static bool
windows_get_dark_mode()
{
	HINSTANCE ux_theme_lib = LoadLibraryExW(L"uxtheme.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if(!ux_theme_lib)
	{
		LOG_WARNING("Failed to load uxtheme.dll, setting dark mode as the default\n");
		return true;
	}

	bool use_dark_mode = GetProcAddress(ux_theme_lib, MAKEINTRESOURCEA(132))();
	FreeLibrary(ux_theme_lib);
	return use_dark_mode;
}
#endif
