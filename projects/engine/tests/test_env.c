#include "engine/constants.h"
#include "engine/env.h"

#include <stdlib.h>

static const char *dummy_text =
	"Lorem Ipsum is simply dummy text of the printing and typesetting"
	"industry. Lorem Ipsum has been the industry's standard dummy text ever since 1966,"
	"when designers at Letraset and James Mosley, the librarian at St Bride Printing"
	"Library in London, took a 1914 Cicero translation and scrambled it to make dummy"
	"text for Letraset's Body Type sheets. It has survived not only many decades, but"
	"also the leap into electronic typesetting, remaining essentially unchanged. It was"
	"popularised thanks to these sheets and more recently with desktop publishing "
	"software like Aldus PageMaker and Microsoft Word including versions of Lorem Ipsum.";

static bool
dummy_text_env_strdup(__env)
{
	const char *dummy_text_duplicate = env_strdup(dummy_text);
	if (!dummy_text_duplicate) {
		LOG_ERROR("Got NULL from env_strdup\n");
		return false;
	}

	if (strcmp(dummy_text, dummy_text_duplicate) != 0) {
		LOG_ERROR("Got non-zero strcmp for dummy_text and env_strdup(env, dummy_text)\n");
		return false;
	}

	uint64_t length = strlen(dummy_text);
	if (length == 0 || dummy_text_duplicate[length] != '\0') {
		LOG_ERROR("Got a non-null terminated string from env_strdup\n");
		return false;
	}

	return true;
}

int
test_env(int argc, char *argv[])
{
	struct env_options options = {
		.pool_options = (struct pool_options) {
			.alignment = 8,
			.chunksize = 64 * KB,
			.reserve   = 04 * GB,
		},
	};

	struct env env;
	env_init(&env, options);

	bool failed = !dummy_text_env_strdup(&env);
	env_deinit(&env);

	if (failed) {
		LOG_ERROR("Failed\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
