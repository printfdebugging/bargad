#include "engine/image.h"

#include "stb_image.h"

void
image_init_from_file(struct image *image, const char *filepath)
{
	stbi_set_flip_vertically_on_load(true);
	image->data = stbi_load(filepath, &image->width, &image->height, &image->channels, 0);

	if(!image->data)
		LOG_FATAL("Failed to load image %s\n", filepath);
}

void
image_deinit(struct image *image)
{
	stbi_image_free(image->data);
}
