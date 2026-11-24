#ifndef ENGINE_OS_H
#define ENGINE_OS_H

#include "engine/features.h"

#include <stdint.h>

struct os_sysinfo
{
	uint32_t procs;
	struct
	{
		uint64_t total;
		uint64_t free;
		uint64_t page_size;
	} memory;
};

/*!
 * @brief `os_sysinfo`
 * - Returns the system info using various platform specific helper/library functions
 * - On Unix, `sysconf` and `sysinfo`
 * - On Win32, `GetSystemInfo` and `GlobalMemoryStatusEx`
 */
struct os_sysinfo os_sysinfo(void);

/*!
 * @brief `os_malloc`
 * - Calls the platform malloc
 */
attribute_malloc
attribute_alloc_size(1)
void *os_malloc(uint64_t size);

/*!
 * @brief `os_free`
 * - Calls the platform free
 */
void os_free(void *chunk);

/*!
 * @brief `os_mmap`
 * - On Unix, calls `mmap` with `PROT_NONE` and `MAP_PRIVATE | MAP_ANONYMOUS`
 * - On Win32, calls `VirtualAlloc` with `MEM_RESERVE` and `PAGE_READWRITE`
 */
void *os_mmap(uint64_t size);

/*!
 * @brief `os_mprotect`
 * - On Unix, calls `mprotect`
 * - On Win32, calls `VirtualAlloc` with `MEM_COMMIT` and `PAGE_READWRITE`
 */
bool os_mprotect(void *chunk, uint64_t size);

/*!
 * @brief `os_mprotect`
 * - On Unix, calls `munmap`
 * - On Win32, calls `VirtualFree` with `MEM_RELEASE`
 */
void os_munmap(void *chunk, uint64_t size);

#endif
