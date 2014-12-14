/* fat.h
 * author: Adam Vandolder
 * created: July 4, 2014
 * last edited: July 4, 2014
 *
 * Contains the functions and structs necessary for interfacing with the FAT32
 * driver of navi. */

#include <stdbool.h>

struct fat_file
{
	int curr_sector;
	int bytes_read_from_curr_sector;
	bool last_sector;
	bool end_of_file;
};

struct fat_file* open_file ( char* path );
bool close_file ( struct fat_file* );
bool read_file ( struct fat_file* file, int num_of_bytes, char buffer[]);
