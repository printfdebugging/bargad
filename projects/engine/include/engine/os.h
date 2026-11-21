#ifndef ENGINE_OS_H
#define ENGINE_OS_H

#include "engine/features.h"

#include <stdint.h>

struct os_sysinfo {
	uint32_t procs;
	struct {
		uint64_t total;
		uint64_t free;
		uint64_t page_size;
	} memory;
};

struct os_sysinfo os_sysinfo(void);

attribute_malloc
attribute_alloc_size(1)
void *os_malloc(uint64_t size);

void os_free(void *chunk);
void *os_mmap(uint64_t size);
bool os_mprotect(void *chunk, uint64_t size);
void os_munmap(void *chunk, uint64_t size);

#endif
