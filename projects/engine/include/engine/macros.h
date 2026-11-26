#ifndef ENGINE_MACROS_H
#define ENGINE_MACROS_H

#define HexRGB(color)  (((color >> 16) & 0xFF) / 255.0f), (((color >> 8) & 0xFF) / 255.0f), (((color) & 0xFF) / 255.0f)
#define HexRGBA(color) (((color >> 24) & 0xFF) / 255.0f), HexRGB(color)

#define countof(a)         (sizeof(a) / sizeof(*(a)))
#define lengthof(s)        (countof(s) - 1)
#define is_power_of_two(x) (x && !(x & (x - 1)))

#define valueof(type, ptr, off) *(((type *)(void *)ptr) + off)

#endif
