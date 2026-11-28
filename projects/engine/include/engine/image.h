#ifndef ENGINE_IMAGE_H
#define ENGINE_IMAGE_H

#include "engine/logger.h"

#include "glad/glad.h"
#include "stb_image.h"

#include <inttypes.h>
#include <stdint.h>

struct image
{
	int width;
	int height;
	int channels;
	int size;
	void *data;
};

static inline uint64_t
image_format_from_channels(int channels)
{
	switch(channels)
	{
		case 3:  return GL_RGB;
		case 4:  return GL_RGBA;
		default: LOG_FATAL("Unknown channel count %i\n", channels);
	}
}

/*!
 * @brief
 * - Loads an image, flipped vertically
 */
static inline void
image_init_from_file(struct image *image, const char *filepath)
{
	stbi_set_flip_vertically_on_load(true);
	image->data = stbi_load(filepath, &image->width, &image->height, &image->channels, 0);

	if(!image->data)
		LOG_FATAL("Failed to load image %s\n", filepath);
}

/*!
 * @brief
 * - Only free's the image data, `image->data`
 * - Does not free the `struct image` itself
 */
static inline void
image_deinit(struct image *image)
{
	stbi_image_free(image->data);
}

#endif
