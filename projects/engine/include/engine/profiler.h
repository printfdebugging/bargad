#ifndef ENGINE_PROFILER_H
#define ENGINE_PROFILER_H

#include <time.h>

#define __profiler_concat_tokens(a, b) a##b
#define __profiler_concat(a, b)        __profiler_concat_tokens(a, b)
#define __profiler_slot_name           __profiler_concat(profiler_slot_, __LINE__)

static inline double
profiler_now_ms(void)
{
	struct timespec now;
	timespec_get(&now, TIME_UTC);
	return ((double)now.tv_sec * 1e3) + ((double)now.tv_nsec / 1e6);
}

/*!
 * @brief `profiler_profile_max`
 * - Profiles a bunch of statements
 * - Prints warnings when the execution time exceeds `max_allowed_ms`.
 * - eg. profiler_profile_max("foo", 20) { ... }
 */
#define profiler_profile_max(block_name, max_allowed_ms) \
	static struct profiler_slot __profiler_slot_name = { \
		.name = (block_name), \
		.file = __FILE__, \
		.line = __LINE__, \
		.ms.limit = (max_allowed_ms) \
	}; \
	for(double start = profiler_now_ms(), elapsed = -1; elapsed < 0; elapsed = profiler_now_ms() - start, profiler_add(&__profiler_slot_name, elapsed))

/*!
 * @brief `profiler_profile`
 */
#define profiler_profile(block_name) \
	profiler_profile_max(block_name, 0.0)

struct profiler_slot
{
	const char *name;
	const char *file;
	const int line;

	struct
	{
		double limit;
		double total;
		double max;
	} ms;

	long call_count;
	struct profiler_slot *next;
};

void profiler_add(struct profiler_slot *slot, double elapsed_ms);
void profiler_frame(void);
void profiler_report(void);

#endif
