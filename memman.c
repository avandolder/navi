/* memman.c
 * author: Adam Vandolder
 * created: December 14, 2014
 * last edited: December 14, 2014
 *
 * Deals with the GDT/paging and the MMU. */

#include <stdint.h>

#include "memman.h"

uint32_t page_directory [1024];
uint32_t page_table [1024];

void
init_page_directory ()
{
}
