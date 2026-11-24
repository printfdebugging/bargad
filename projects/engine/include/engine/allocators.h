#ifndef ENGINE_ALLOCATORS_H
#define ENGINE_ALLOCATORS_H

#include "engine/features.h"

#include <stddef.h>
#include <stdint.h>

struct pool
{
	uint8_t *beg;
	uint8_t *cur;
	uint8_t *end;
	struct pool_options
	{
		uint64_t alignment;
		uint64_t chunksize;
		uint64_t reserve;
	} options;
};

/*!
 * @brief `pool_init`
 * - Pool should be allocated/managed by the caller
 * - Reserves `options.reserve` size virtual address space
 * - Commits `options.chunksize` for the initial capacity
 */
bool pool_init(struct pool *pool, struct pool_options options);

/*!
 * @brief `pool_allocate`
 * - Returns a pointer to a chunk of size bytes
 * - todo: Aligns the pointer to `options.alignment` boundary
 * - Commits an extra chunk when it expands to have some spare capacity
 */
attribute_malloc
attribute_alloc_size(2)
void *pool_allocate(struct pool *pool, uint64_t size);

/*!
 * @brief `pool_reset`
 * - Resets the current pointer to the beginning.
 * - Does not free the memory, for that use pool_deinit(p);
 * - Used with small, temporary pools.
 */
void pool_reset(struct pool *pool);

/*!
 * @brief `pool_deinit`
 * - Unmaps the address space page mappings
 * - Does not free the pool itself
 */
void pool_deinit(struct pool *pool);

#endif
