#ifndef ENGINE_IMAGE_H
#define ENGINE_IMAGE_H

#include "engine/logger.h"

#include "glad/glad.h"

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
void image_init_from_file(struct image *image, const char *filepath);

/*!
 * @brief
 * - Only free's the image data, `image->data`
 * - Does not free the `struct image` itself
 */
void image_deinit(struct image *image);

#endif
