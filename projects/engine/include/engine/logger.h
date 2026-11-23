#ifndef ENGINE_LOGGER_H
#define ENGINE_LOGGER_H

#if defined(DEBUG)
#	include <stdio.h>

#	define LOG(level, color, ...) \
		do { \
			fprintf(stderr, "\033[0;" color "m" level ": %s: ", __func__); \
			fprintf(stderr, __VA_ARGS__); \
			fprintf(stderr, "\033[0m"); \
		} while(0)

#	define LOG_INFO(...)    LOG("[INFO]   ", "34", __VA_ARGS__)
#	define LOG_WARNING(...) LOG("[WARNING]", "33", __VA_ARGS__)
#	define LOG_ERROR(...)   LOG("[ERROR]  ", "31", __VA_ARGS__)

#else
#	define LOG_INFO(...)
#	define LOG_WARNING(...)
#	define LOG_ERROR(...)
#endif

#include <stdlib.h>

#define LOG_FATAL(...) \
	{ \
		LOG("[FATAL]  ", "33", __VA_ARGS__); \
		abort(); \
	}

#endif
