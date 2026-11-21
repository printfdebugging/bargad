#ifndef ENGINE_COMPILER_FEATURES_H
#define ENGINE_COMPILER_FEATURES_H

#if __has_attribute(malloc)
#	define attribute_malloc __attribute__((malloc))
#else
#	define attribute_malloc
#endif

#if __has_attribute(alloc_size)
#	define attribute_alloc_size(n) __attribute__((alloc_size(n)))
#else
#	define attribute_alloc_size(n)
#endif

#if __has_attribute(nonnull)
#	define attribute_nonnull(...) __attribute__((nonnull(__VA_ARGS__)))
#else
#	define attribute_nonnull(n)
#endif

#endif
