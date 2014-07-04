#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

uint8_t* const CON_MEM = (uint8_t*) 0xB8000;
int con_x, con_y, con_width, con_height, con_color;
int cursor_x, cursor_y;

// inportb provides a wrapper around the inb opcode.
inline uint8_t
inportb ( uint16_t port )
{
	uint8_t data;
	asm volatile ( "inb %1, %0" : "=a" (data) : "Nd" (port) );
	return data;
}

// outportb provides a wrapper around the outb opcode.
inline void
outportb ( uint16_t port, uint8_t data )
{
	asm volatile ( "outb %0, %1" : : "a" (data), "Nd" (port) );
}

// kernel_cursor moves the hardware cursor.
void
kernel_cursor(int x, int y)
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

// kernel_clear clears the console.
void
kernel_clear ()
{
	for (int i = 0; i < (con_width * con_height); i++)
	{
		CON_MEM[i * 2] = ' ';
		CON_MEM[i * 2 + 1] = con_color;
	}
}

void
kernel_initcon ( int w, int h, int c )
{
	con_width = w;
	con_height = h;
	con_color = c;
	con_x = 0;
	con_y = 0;
	kernel_cursor(0, 0);
	kernel_clear();
}

// kernel_scroll scrolls the console up one row.
void
kernel_scroll ()
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
kernel_putnl ()
{
	con_x = 0;
	if (++con_y == con_height)
	{
		con_y = con_height - 1;
		kernel_scroll();
	}
}

void
kernel_putchr ( uint8_t character )
{
	// Go to the next line if character is a new line.
	if (character == '\n')
		kernel_putnl();
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
		kernel_putnl();

	kernel_cursor(con_x, con_y);
}

void
kernel_putuint ( uint32_t number )
{
	if (number > 9)
		kernel_putuint((uint32_t) number / 10);

	kernel_putchr('0' + (number % 10));
}

void
kernel_putint ( int32_t number )
{
	if (number < 0)
	{
		kernel_putchr('-');
		kernel_putuint(-number);
	}
	else
		kernel_putuint(number);
}

void
kernel_puthex ( uint32_t number )
{
	if (number > 0xF)
		kernel_puthex(number >> 4);

	if ((number & 0xF) >= 0xA)
		kernel_putchr('A' + ((number & 0xF) - 0xA));
	else
		kernel_putchr('0' + (number & 0xF));
}

void
kernel_putstr ( const char* string )
{
	for (; *string != 0; string++)
		kernel_putchr(*string);
}

void
kernel_printf ( const char* format, ... )
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
				kernel_putchr(*format);
				break;

			case 'i':
				kernel_putint(va_arg(args, int));
				break;

			case 's':
				kernel_putstr(va_arg(args, char*));
				break;

			// x - Print out a hexidecimal number with '0x' prefix.
			case 'x':
				kernel_putchr('0');
				kernel_putchr('x');
				// fallthroughs into 'h'..

			// h - Print out hexidecimal uint
			case 'h':
				kernel_puthex(va_arg(args, uint32_t));
				break;

			case '%':
				kernel_putchr('%');
				break;

			default:
				kernel_printf("Invalid kernel_printf format specifier %c.\n",
							  *format);
				break;
			}
		}
		else	// If not a format specifier, just print the character.
			kernel_putchr(*format);
	}

	va_end(args);
}

void
kernel_main ()
{
	kernel_initcon(80, 25, 0x1F);

	kernel_printf("navi v0.1.1");

#ifdef BUILDNUM
	// BUILDNUM was defined on the commandline.
	kernel_printf(" Build #%i", BUILDNUM);
#endif

	kernel_printf("\tBooting...\n");

	kernel_printf("%i ", -1234567890);
	for(int i = 0; i < 5; i++)
		kernel_printf("%i", 1234567890);

	kernel_printf("\nMagic: %x%h", 0xDEADBEEF, 0xCAFE0101);
}
