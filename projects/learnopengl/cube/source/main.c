#include "engine/image.h"
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

enum vertex_attribute
{
	vertex_attribute_position,
	vertex_attribute_color,
	vertex_attribute_uv,
};

static const char *attributes[] = {
	[vertex_attribute_position] = "in_vertex_position",
	[vertex_attribute_color] = "in_vertex_color",
	[vertex_attribute_uv] = "in_vertex_uv",
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

static float colors[] = {
   1.0f, 0.0f, 0.0f, 
   0.0f, 1.0f, 0.0f,
   0.0f, 0.0f, 1.0f 
};

float uv[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.5f, 1.0f
};
/* clang-format on */

int
main(int argc, char *argv[])
{
	GLFWwindow *window = window_create(window_options);

	uint32_t varray;
	glGenVertexArrays(1, &varray);
	glBindVertexArray(varray);

	uint32_t buffer_vertex;
	glGenBuffers(1, &buffer_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(vertex_attribute_position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(vertex_attribute_position);

	uint32_t buffer_color;
	glGenBuffers(1, &buffer_color);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(vertex_attribute_color, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(vertex_attribute_color);

	uint32_t buffer_uv;
	glGenBuffers(1, &buffer_uv);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(vertex_attribute_uv, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(vertex_attribute_uv);

	uint32_t program = shader_program_create(vshader, countof(vshader), fshader, countof(fshader), attributes, countof(attributes));

	struct image image = { 0 };
	image_init_from_file(&image, PROJECT_ASSETS "bargad.png");
	uint64_t image_format = image_format_from_channels(image.channels);

	uint32_t texture_unit = 0;
	glActiveTexture(GL_TEXTURE0 + texture_unit);

	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, (int)image_format, image.width, image.height, 0, (uint32_t)image_format, GL_UNSIGNED_BYTE, image.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	{
		int location = glGetUniformLocation(program, "uniform_texture0");
		if(location == -1)
			LOG_FATAL("Invalid uniform location, got -1\n");
		glProgramUniform1i(program, location, (int)texture_unit);
	}

	glClearColor(HexRGBA(0X00212bFF));
	glUseProgram(program);

	while(!glfwWindowShouldClose(window))
	{
		window_poll_events();
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		window_swap_buffers(window);
	}

	image_deinit(&image);
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
