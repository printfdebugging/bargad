#ifndef GL_ENV
#define GL_ENV

#include "engine/allocators.h"
#include "engine/features.h"
#include "engine/logger.h"

#include <string.h>

struct env_options
{
	struct pool_options pool_options;
};

/*!
 * @brief `env`
 * - Keeps the state associated with one entity/subsystem
 * - eg: allocators, libuv state/callbacks etc..
 */
struct env
{
	struct pool pool;
};

/*!
 * @brief `__env`
 * - Defines the first parameter of functions as env
 * - Keeps the code concise
 */
#define __env struct env *env

/*!
 * @brief `env_init`
 * - Initializes the env pool with `options.pool_options`
 */
static inline void
env_init(__env, struct env_options options)
{
	bool initialization_failed =
		!pool_init(&env->pool, options.pool_options);

	if(initialization_failed)
	{
		LOG_FATAL("Failed to initialize env\n");
	}
}

/*!
 * @brief `env_deinit`
 * - De-initializes the env
 * - Does *not* free the `env` itself
 */
static inline void
env_deinit(__env)
{
	pool_deinit(&env->pool);
}

#define env_strdup(string) env_strdup__impl(env, string)
#define env_allocate(size) env_allocate__impl(env, size);

/*!
 * @brief `env_strdup`
 * - Duplicates the string and returns a pointer to memory in `env.pool`
 * - A convenience helper
 */
attribute_nonnull(1, 2)
static inline char *
env_strdup__impl(__env, const char *string)
{
	uint64_t length = strlen(string);
	if(length == 0)
	{
		LOG_ERROR("Got 0 from strlen\n");
		return NULL;
	}

	char *copy = pool_allocate(&env->pool, length + 1);
	strcpy(copy, string);
	return copy;
}

/*!
 * @brief `env_allocate`
 * - Allocates using the pool allocator of the `env`
 * - Makes allocation more natural, acting as a kind-of syntactic sugar
 */
attribute_nonnull(1)
static inline void *
env_allocate__impl(__env, uint64_t size)
{
	return pool_allocate(&env->pool, size);
}

#endif
