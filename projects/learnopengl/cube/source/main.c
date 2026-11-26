#include "engine/logger.h"
#include "engine/macros.h"
#include "engine/shader.h"
#include "engine/window.h"

#include <stdlib.h>

static const char vshader_source[] = {
#embed VERTEX_SHADER
	, '\0'
};

static const char fshader_source[] = {
#embed FRAGMENT_SHADER
	, '\0'
};

static const char *attributes[] = {
	[0] = "in_vertex_position",
};

#define version "#version 460 core"
static const char *vshader[] = { version, vshader_source };
static const char *fshader[] = { version, fshader_source };

void window_scroll_callback(GLFWwindow *window, double x, double y);
void window_framebuffer_resize_callback(GLFWwindow *window, int32_t width, int32_t height);
void window_cursor_position_callback(GLFWwindow *window, double x, double y);
void window_key_callback(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
void window_error_callback(int code, const char *description);

static struct window_options window_options = {
	.userdata = NULL,
	.properties = (struct window_properties) {
		.width = 1200,
		.height = 600,
		.title = "GLFWwindow",
	},
	.callbacks = (struct window_callbacks) {
		.cursor_position = window_cursor_position_callback,
		.scroll = window_scroll_callback,
		.framebuffer_resize = window_framebuffer_resize_callback,
		.key = window_key_callback,
		.error = window_error_callback,
	},
	.attributes = (struct window_attributes) {
		.resizable = false,
		.scalable = true,
		.decorated = true,
		.transparent = false,
		.samples = 4,
	},
};

/* clang-format off */
static float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f,
};
/* clang-format on */

int
main(int argc, char *argv[])
{
	GLFWwindow *window = window_create(window_options);

	uint32_t varray;
	glGenVertexArrays(1, &varray);
	glBindVertexArray(varray);

	uint32_t vbuffer;
	glGenBuffers(1, &vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	uint32_t program = shader_program_create(vshader, countof(vshader), fshader, countof(fshader), attributes, countof(attributes));
	glUseProgram(program);

	glClearColor(HexRGBA(0X00212bFF));

	while(!glfwWindowShouldClose(window))
	{
		window_poll_events();
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		window_swap_buffers(window);
	}

	window_destroy(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}

void
window_scroll_callback(GLFWwindow *window, double x, double y)
{
}

void
window_framebuffer_resize_callback(GLFWwindow *window, int32_t width, int32_t height)
{
	glViewport(0, 0, width, height);
}

void
window_cursor_position_callback(GLFWwindow *window, double x, double y)
{
}

void
window_key_callback(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
	if((mods & GLFW_MOD_SHIFT) && (key == GLFW_KEY_Q) && (action == GLFW_PRESS))
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void
window_error_callback(int code, const char *description)
{
	LOG_ERROR("[code: %i], description: %s\n", code, description);
}
