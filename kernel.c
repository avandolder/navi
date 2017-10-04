#include <stdbool.h>
#include <stdint.h>

#include "console.h"
#include "ioport.h"
#include "keyboard.h"

void
kernel_main (void)
{
	console_init(80, 25, COLOR4(COLOR_BLUE, COLOR_WHITE));

	console_printf("navi v0.1.1");

#ifdef BUILDNUM
	// BUILDNUM was defined on the commandline.
	console_printf("\tBuild #%i", BUILDNUM);
#endif

	console_printf("\nBooting...\n");
	
	for (;;) {
	    uint8_t sc;
	    
	    do {
	        sc = inportb(0x60);
	    } while	(sc > MAX_SCANCODE);
	    
	    while (inportb(0x60) != sc + 0x80);
	    
	    console_printf("You pressed the %c key.\n", scancode[sc]);
	}
}

