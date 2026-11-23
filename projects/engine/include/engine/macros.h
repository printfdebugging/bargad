#ifndef ENGINE_MACROS_H
#define ENGINE_MACROS_H

#define color_rgb_hex(color)  (((color >> 16) & 0xFF) / 255.0f), (((color >> 8) & 0xFF) / 255.0f), (((color) & 0xFF) / 255.0f)
#define color_rgba_hex(color) (((color >> 24) & 0xFF) / 255.0f), color_rgb_hex(color)

#define countof(a)         (sizeof(a) / sizeof(*(a)))
#define lengthof(s)        (countof(s) - 1)
#define is_power_of_two(x) (x && !(x & (x - 1)))

#define valueof(type, ptr, off) *(((type *)(void *)ptr) + off)

#endif
