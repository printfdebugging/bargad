#include "engine/allocators.h"
#include "engine/constants.h"
#include "engine/logger.h"
#include "engine/macros.h"

#include <inttypes.h>
#include <stdlib.h>

int
test_allocator_pool(int argc, char *argv[])
{
	struct pool_options options = {
		.alignment = sizeof(uint64_t),
		.chunksize = 64u * KB,
		.reserve = 1u * GB
	};

	struct pool pool;
	if(!pool_init(&pool, options))
		return EXIT_FAILURE;

	for(uint64_t idx = 0; idx < 8'000'000; ++idx)
	{
		uint64_t *integer = pool_allocate(&pool, sizeof(uint64_t));
		*integer = idx;
	}

	for(uint64_t idx = 0; idx < 8'000'000; ++idx)
	{
		uint64_t integer = valueof(uint64_t, pool.beg, idx);
		if(integer != idx)
		{
			LOG_ERROR("integer at index %" PRIu64 " is not the same as the index, expected: %" PRIu64 ", got %" PRIu64 "\n", idx, integer, idx);
			pool_deinit(&pool);
			return EXIT_FAILURE;
		}
	}

	pool_deinit(&pool);
	return EXIT_SUCCESS;
}
