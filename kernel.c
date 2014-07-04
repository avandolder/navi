#include <stdbool.h>
#include <stdint.h>

#include "console.h"

void
kernel_main ()
{
	console_init(80, 25, COLOR4(COLOR_BLUE, COLOR_WHITE));

	console_printf("navi v0.1.1");

#ifdef BUILDNUM
	// BUILDNUM was defined on the commandline.
	console_printf(" Build #%i", BUILDNUM);
#endif

	console_printf("\tBooting...\n");

	console_printf("%i ", -1234567890);
	for(int i = 0; i < 5; i++)
		console_printf("%i", 1234567890);

	console_printf("\nMagic: %x%h", 0xDEADBEEF, 0xCAFE0101);
}
