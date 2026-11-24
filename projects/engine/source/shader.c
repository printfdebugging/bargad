#include "engine/shader.h"
#include "engine/logger.h"

uint32_t shader_program_current = 0;

bool
shader_get_compile_status(uint32_t object)
{
	int32_t status;
	glGetShaderiv(object, GL_COMPILE_STATUS, &status);
	if(status)
		return true;

	int32_t length;
	glGetShaderiv(object, GL_INFO_LOG_LENGTH, &length);

	char log[length];
	glGetShaderInfoLog(object, length, NULL, log);
	LOG_ERROR("failed to compile shader, error message: %s\n", log);
	return false;
}

bool
shader_get_link_status(uint32_t program)
{
	int32_t status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status)
		return true;

	int32_t length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

	char log[length];
	glGetProgramInfoLog(program, length, NULL, log);
	LOG_ERROR("failed to link shader program: %s\n", log);
	return false;
}

uint32_t
shader_compile(uint32_t type, const char **sources, int32_t size)
{
	uint32_t shader = glCreateShader(type);
	glShaderSource(shader, size, sources, NULL);
	glCompileShader(shader);

	if(!shader_get_compile_status(shader))
	{
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

uint32_t
shader_program_create(const char **vertex, int32_t vertex_size, const char **fragment, int32_t fragment_size, const char **attributes, uint32_t attributes_size)
{
	uint32_t vertex_shader = shader_compile(GL_VERTEX_SHADER, vertex, vertex_size);
	if(!vertex_shader)
		return 0;

	uint32_t fragment_shader = shader_compile(GL_FRAGMENT_SHADER, fragment, fragment_size);
	if(!fragment_shader)
	{
		glDeleteShader(vertex_shader);
		return 0;
	}

	uint32_t program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	if(attributes)
	{
		for(uint32_t i = 0; i < attributes_size; ++i)
			glBindAttribLocation(program, i, attributes[i]);
	}

	glLinkProgram(program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	if(!shader_get_link_status(program))
	{
		glDeleteProgram(program);
		return 0;
	}

	return program;
}

void
shader_program_destroy(uint32_t program)
{
	glDeleteProgram(program);
}
