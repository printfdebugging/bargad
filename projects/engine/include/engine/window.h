#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include "glad/glad.h"

#include "GLFW/glfw3.h"

struct window_userdata;

struct window_options {
	struct window_properties {
		int32_t width;
		int32_t height;
		const char *title;
	} properties;

	struct window_callbacks {
		GLFWcursorposfun cursor_position;
		GLFWscrollfun scroll;
		GLFWframebuffersizefun framebuffer_resize;
		GLFWkeyfun key;
		GLFWerrorfun error;
	} callbacks;

	struct window_attributes {
		bool resizable;
		bool scalable;
		bool decorated;
		bool transparent;
		int32_t samples;
	} attributes;

	struct window_userdata *userdata;
};

GLFWwindow *window_create(struct window_options opts);

void window_destroy(GLFWwindow *window);
void window_get_bounds(GLFWwindow *window, int *width, int *height);
void window_swap_buffers(GLFWwindow *window);
void window_poll_events();

#endif
