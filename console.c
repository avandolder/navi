#include <stdint.h>
#include <stdarg.h>

#include "console.h"
#include "ioport.h"

uint8_t* const CON_MEM = (uint8_t*) 0xB8000;
int con_x, con_y, con_width, con_height, con_color, cursor_x, cursor_y;

// console_init initializes the console driver with the width, height, and
// color the console should be.
void
console_init ( int width, int height, int color )
{
	con_width = width;
	con_height = height;
	con_color = color;
	con_x = 0;
	con_y = 0;
	console_cursor(0, 0);
	console_clear();
}

// console_cursor moves the hardware cursor.
void
console_cursor(int x, int y)
{
	cursor_x = x;
	cursor_y = y;

	int loc = (cursor_y * con_width) + cursor_x;
	// Tell the VGA board we are setting the high cursor byte.
	outportb(0x3D4, 14);
	// Send the high cursor byte.
	outportb(0x3D5, loc >> 8);
	// Tell the VGA board we are setting the low cursor byte.
	outportb(0x3D4, 15);
	// Send the low cursor byte.
	outportb(0x3D5, loc);
}

// console_clear clears the console.
void
console_clear ()
{
	for (int i = 0; i < (con_width * con_height); i++)
	{
		CON_MEM[i * 2] = ' ';
		CON_MEM[i * 2 + 1] = con_color;
	}
}

// console_scroll scrolls the console up one row.
void
console_scroll ()
{
	// Loop over every row except the first
	for (int y = 1; y < con_height; y++)
		// Loop over every column
		for (int x = 0; x < con_width; x++)
		{
			int src = ((y * con_width) + x) * 2;
			int dst = src - con_width;
			
			CON_MEM[dst] = CON_MEM[src];
			CON_MEM[dst + 1] = CON_MEM[src + 1];
		}
	
	// Clear out the new bottom row
	for (int x = 0; x < con_width; x++)
	{
		int dst = (((con_height - 1) * con_width) + x) * 2;
		CON_MEM[dst] = ' ';
		CON_MEM[dst + 1] = con_color;
	}
}

void
console_putnl ()
{
	con_x = 0;
	if (++con_y == con_height)
	{
		con_y = con_height - 1;
		console_scroll();
	}
}

void
console_putchar ( uint8_t character )
{
	// Go to the next line if character is a new line.
	if (character == '\n')
		console_putnl();
	// Skip ahead to the next number divisible by 8 for a tab character.
	else if (character == '\t')
		con_x = con_x + (-((con_x % 8 ?: 8) - 8) ?: 8);
	// Print out any other character to the screen.
	else
	{
		uint32_t dst = ((con_y * con_width) + con_x) * 2;	
		CON_MEM[dst] = character;
		CON_MEM[dst + 1] = con_color;
		con_x++;
	}
	
	if (con_x == con_width)
		console_putnl();

	console_cursor(con_x, con_y);
}

void
console_putuint ( uint32_t number )
{
	if (number > 9)
		console_putuint((uint32_t) number / 10);

	console_putchar('0' + (number % 10));
}

void
console_putint ( int32_t number )
{
	if (number < 0)
	{
		console_putchar('-');
		console_putuint(-number);
	}
	else
		console_putuint(number);
}

void
console_puthex ( uint32_t number )
{
	if (number > 0xF)
		console_puthex(number >> 4);

	if ((number & 0xF) >= 0xA)
		console_putchar('A' + ((number & 0xF) - 0xA));
	else
		console_putchar('0' + (number & 0xF));
}

void
console_putstr ( const char* string )
{
	for (; *string != 0; string++)
		console_putchar(*string);
}

void
console_printf ( const char* format, ... )
{
	va_list args;
	va_start(args, format);

	for (; *format != 0; format++)
	{
		/* If the character is the start of a format specifier,
		 * figure out what format/data is required. */
		if (*format == '%')
		{
			format++;
			switch (*format)
			{
			case 'c':
				console_putchar(*format);
				break;

			case 'i':
				console_putint(va_arg(args, int));
				break;

			case 's':
				console_putstr(va_arg(args, char*));
				break;

			// x - Print out a hexidecimal number with '0x' prefix.
			case 'x':
				console_putchar('0');
				console_putchar('x');
				// Falls through into 'h'...
			// h - Print out hexidecimal uint
			case 'h':
				console_puthex(va_arg(args, uint32_t));
				break;

			case '%':
				console_putchar('%');
				break;

			default:
				console_printf("Invalid console_printf format specifier %c.\n",
							  *format);
				break;
			}
		}
		else	// If not a format specifier, just print the character.
			console_putchar(*format);
	}

	va_end(args);
}
