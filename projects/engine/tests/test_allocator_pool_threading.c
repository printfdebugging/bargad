#include "engine/allocators.h"
#include "engine/constants.h"
#include "engine/logger.h"
#include "engine/macros.h"
#include "engine/os.h"

#include "uv.h"

#include <inttypes.h>
#include <stdlib.h>

struct thread_context {
	uint64_t number;
	struct pool pool;
};

void
thread_writer(void *arg)
{
	struct thread_context *ctx = arg;
	for(uint64_t idx = 0; idx < 16'000; ++idx) {
		uint64_t *integer = pool_allocate(&ctx->pool, sizeof(uint64_t));
		*integer = idx;
	}
}

void
thread_reader(void *arg)
{
	struct thread_context *ctx = arg;
	for(uint64_t idx = 0; idx < 16'000; ++idx) {
		uint64_t integer = valueof(uint64_t, ctx->pool.beg, idx);
		if(integer != idx) {
			LOG_ERROR("integer %" PRIu64 " is not the same as idx %" PRIu64 " in thread %" PRIu64 "\n", integer, idx, ctx->number);
			exit(1);
		}
	}
}

int
test_allocator_pool_threading(int argc, char *argv[])
{
	uint64_t procs = os_sysinfo().procs;
	uv_thread_t threads[procs];
	struct thread_context contexts[procs];

	struct pool_options options = {
		.alignment = sizeof(uint64_t),
		.chunksize = 64u * KB,
		.reserve = 1u * MB,
	};

	for(uint64_t proc = 0; proc < procs; ++proc) {
		contexts[proc].number = proc;
		if(!pool_init(&contexts[proc].pool, options))
			return EXIT_FAILURE;
	}

	for(uint64_t proc = 0; proc < procs; ++proc)
		uv_thread_create(&threads[proc], thread_writer, &contexts[proc]);
	for(uint64_t proc = 0; proc < procs; ++proc)
		uv_thread_join(&threads[proc]);
	for(uint64_t proc = 0; proc < procs; ++proc)
		uv_thread_create(&threads[proc], thread_reader, &contexts[proc]);
	for(uint64_t proc = 0; proc < procs; ++proc)
		uv_thread_join(&threads[proc]);

	for(uint64_t proc = 0; proc < procs; ++proc)
		pool_deinit(&contexts[proc].pool);

	return EXIT_SUCCESS;
}
