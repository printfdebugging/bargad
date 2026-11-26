#include "engine/image.h"

#include "stb_image.h"

bool
image_init_from_file(struct image *image, const char *filepath)
{
	stbi_set_flip_vertically_on_load(true);
	if(!(image->data = stbi_load(filepath, &image->width, &image->height, &image->channels, 0)))
	{
		LOG_ERROR("Failed to load image %s\n", filepath);
		return false;
	}

	return true;
}

void
image_deinit(struct image *image)
{
	stbi_image_free(image->data);
}
