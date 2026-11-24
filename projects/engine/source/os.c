#include "engine/os.h"
#include "engine/constants.h"
#include "engine/logger.h"

#include <inttypes.h>
#include <stdlib.h>

#if defined(__USE_POSIX)
#	include <unistd.h>
#	include <sys/mman.h>
#	include <sys/sysinfo.h>
#elif defined(_WIN32)
#	include <memoryapi.h>
#	include <sysinfoapi.h>
#endif

inline static void
os_info_log()
{
	const char *fmt =
		"\n"
		"struct system_info {\n"
		"▏	uint32_t procs =  %" PRIu32 "\n"
		"▏	struct {\n"
		"▏	▏	uint64_t total;     = %" PRIu64 " GB\n"
		"▏	▏	uint64_t free;      = %" PRIu64 " GB\n"
		"▏	▏	uint64_t page_size; = %" PRIu64 " B\n"
		"▏	};\n"
		"};\n";

	const struct os_sysinfo info = os_sysinfo();
	LOG_WARNING(fmt, info.procs, info.memory.total / GB, info.memory.free / GB, info.memory.page_size);
}

void *
os_malloc(uint64_t size)
{
	return malloc(size);
}

void
os_free(void *chunk)
{
	free(chunk);
}

#if defined(__USE_POSIX)

struct os_sysinfo
os_sysinfo(void)
{
	uint64_t pages = (uint64_t)sysconf(_SC_PHYS_PAGES);
	uint64_t page_size = (uint64_t)sysconf(_SC_PAGESIZE);

	struct sysinfo usysinfo = {};
	sysinfo(&usysinfo);

	struct os_sysinfo info = {
		.procs = (uint32_t)get_nprocs(),
		.memory = {
			.total = pages * page_size,
			.free = usysinfo.freeram,
			.page_size = (uint64_t)getpagesize(),
		},
	};

	return info;
}

void *
os_mmap(uint64_t size)
{
	void *chunk = mmap(0, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if(chunk == MAP_FAILED)
	{
		LOG_ERROR("mmap returned MAP_FAILED for size: %" PRIu64 "\n", size);
		os_info_log();
		return NULL;
	}
	return chunk;
}

bool
os_mprotect(void *chunk, uint64_t size)
{
	return mprotect(chunk, size, PROT_READ | PROT_WRITE) == 0;
}

void
os_munmap(void *chunk, uint64_t size)
{
	munmap(chunk, size);
}

#elif defined(_WIN32)

struct os_sysinfo
os_sysinfo(void)
{
	SYSTEM_INFO system = {};
	MEMORYSTATUSEX memory = { .dwLength = sizeof(MEMORYSTATUSEX) };
	GetSystemInfo(&system);
	GlobalMemoryStatusEx(&memory);

	struct os_sysinfo info = {
		.procs = system.dwNumberOfProcessors,
		.memory = {
			.total = memory.ullTotalPhys,
			.free = memory.ullAvailPhys,
			.page_size = system.dwPageSize,
		},
	};

	return info;
}

void *
os_mmap(uint64_t size)
{
	return VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);
}

bool
os_mprotect(void *chunk, uint64_t size)
{
	return VirtualAlloc(chunk, size, MEM_COMMIT, PAGE_READWRITE);
}

void
os_munmap(void *chunk, uint64_t size)
{
	(void)size;
	VirtualFree(chunk, 0, MEM_RELEASE);
}

#endif
