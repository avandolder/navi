#include <stdint.h>

void console_init (int width, int height, int color);
void console_color (uint16_t color);
void console_cursor (int x, int y);
void console_move (int x, int y);
void console_clear (void);
void console_scroll (void);
void console_putnl (void);
void console_putchar (uint8_t character);
void console_putuint (uint32_t number);
void console_putint (int32_t number);
void console_puthex (uint32_t number);
void console_putstr (const char* string);
void console_printf (const char* format, ...);

enum vga4_color
{
	COLOR_BLACK,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_AQUA,
	COLOR_RED,
	COLOR_PURPLE,
	COLOR_BROWN,
	COLOR_LIGHT_GRAY,
	COLOR_BG_MAX = COLOR_LIGHT_GRAY,
	COLOR_GRAY,
	COLOR_LIGHT_BLUE,
	COLOR_LIGHT_GREEN,
	COLOR_CYAN,
	COLOR_LIGHT_RED,
	COLOR_MAGENTA,
	COLOR_YELLOW,
	COLOR_WHITE,
	COLOR_FG_MAX = COLOR_WHITE
};

// COLOR4 is a simple macro to allow for easier selecting of 4-bit color pairs.
#define COLOR4(bg, fg) ((bg << 4) | fg)

