#ifndef ENGINE_SHADER_H
#define ENGINE_SHADER_H

#include <stdint.h>

#include "glad/glad.h"

/*!
 * @brief Shader Helpers
 * 	These are a bunch of commonly used shader helpers. They
 * can be used by the client program directly to create some
 * shader or indrectly through other subsystems (most often).
 *
 * 	Usually the model rendering shader has it's own shader
 * uniforms and vertex attributes, and the font rendering code
 * has it's own shader.. So ideally these helpers would be used
 * by another helper function which would create the shader
 * for a specific subsystem and setup everything..
 *
 * 	The client code would mostly call `shader_program_use`,
 * or `shader_program_destroy` to use/destroy the shader program
 * respectively.
 */

/*!
 * @brief `shader_program_create`
 * - Creates a shader program from vertex and fragment sources
 * - Calls `glBindAttribLocation` on `attributes` before linking
 */
uint32_t shader_program_create(
	const char **vertex,         /*! @param vertex    	       - an array of strings comprising the vertex shader source */
	int32_t      vertex_size,    /*! @param vertex_size       - number of strings in vertex_sources array */
	const char **fragment,       /*! @param fragment          - an array of strings comprising the fragment shader source */
	int32_t      fragment_size,  /*! @param fragment_size     - number of strings in fragment_sources array */
	const char **attributes,     /*! @param attributes        - an array of strings mapping the position to the attribute location in a vertex shader */
	uint32_t     attributes_size /*! @param attributes_size   - number of strings in attributes array */
);

/*!
 * @brief `shader_program_destroy`
 * - Calls glDeleteProgram(program)
 */
void shader_program_destroy(uint32_t program);

/*!
 * @brief `shader_program_use`
 * - A helper which remembers the last shader program
 * - Avoids unnecessary shader program switches.
 */
static inline void
shader_program_use(uint32_t program)
{
	extern uint32_t shader_program_current;

	if (program != shader_program_current) {
		glUseProgram(program);
		shader_program_current = program;
	}
}

#endif
