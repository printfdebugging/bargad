#include "engine/allocators.h"
#include "engine/logger.h"
#include "engine/macros.h"
#include "engine/os.h"

#include <inttypes.h>

bool
pool_init(struct pool *pool, struct pool_options options)
{
#if defined(DEBUG)
	bool invalid_options =
		!options.alignment ||
		!options.chunksize ||
		!options.reserve;

	bool invalid_alignment =
		!is_power_of_two(options.alignment) ||
		!is_power_of_two(options.chunksize);

	if(invalid_options) {
		LOG_ERROR("invalid pool_options\n");
		return false;
	}

	if(invalid_alignment) {
		LOG_ERROR("options.alignment %" PRIu64 ", or options.chunksize %" PRIu64 " is not a power of 2\n", options.alignment, options.chunksize);
		return false;
	}
#endif

	void *chunk = os_mmap(options.reserve);

	bool invalid_allocation =
		!chunk ||
		!os_mprotect(chunk, options.chunksize);

	if(invalid_allocation) {
		LOG_ERROR("failed to allocate, reserve, or commit memory\n");
		return false;
	}

	pool->beg = chunk;
	pool->cur = chunk;
	pool->end = pool->cur + options.chunksize;
	pool->options = options;
	return true;
}

void *
pool_allocate(struct pool *pool, uint64_t size)
{
	uint64_t committed = (uint64_t)(pool->end - pool->beg);
	uint64_t used = (uint64_t)(pool->cur - pool->beg);
	uint64_t available = committed - used;

	if(size < available) {
		void *chunk = pool->cur;
		pool->cur += size;
		return chunk;
	}

	uint64_t needed = size - available + pool->options.chunksize;
	uint64_t remaining = pool->options.reserve - committed;

	if(remaining < needed)
		LOG_FATAL("the reserves can't fulfill this request\n");

	if(!os_mprotect(pool->end, needed))
		LOG_FATAL("failed to commit memory\n");

	pool->end += needed;
	return pool_allocate(pool, size);
}

void
pool_reset(struct pool *pool)
{
	pool->cur = pool->beg;
}

void
pool_deinit(struct pool *pool)
{
	os_munmap(pool->beg, pool->options.reserve);
}
