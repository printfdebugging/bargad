#ifndef ENGINE_CONSTANTS_H
#define ENGINE_CONSTANTS_H

#define NUL             0x00
#define SPACE           0x20
#define NEWLINE         0x0a
#define HORIZONTAL_TAB  0x09
#define CARRIAGE_RETURN 0x0d

#define U64_MAX 18446744073709551615UL
#define U32_MAX 4294967295U
#define U16_MAX 65535U
#define U8_MAX  255U
#define U64_MIN 0UL
#define U32_MIN 0U
#define U16_MIN 0U
#define U8_MIN  0U

#define I8_MAX  127
#define I16_MAX 32767
#define I32_MAX 2147483647
#define I64_MAX 9223372036854775807L
#define I8_MIN  (-I8_MAX - 1)
#define I16_MIN (-I16_MAX - 1)
#define I32_MIN (-I32_MAX - 1)
#define I64_MIN (-I64_MAX - 1)

#define KB (1ull << 10)
#define MB (1ull << 20)
#define GB (1ull << 30)
#define TB (1ull << 40)

#define MM   (1.0f)
#define CM   (10.0f * MM)
#define INCH (25.4f * MM)

#define HB_GPU_ATLAS_TEXEL_SIZE 8
#define ATLAS_PAGE_SIZE         (HB_GPU_ATLAS_TEXEL_SIZE * MB)
#define MAX_TEXTURE_COUNT       16

#endif
