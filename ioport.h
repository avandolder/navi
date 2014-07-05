#include <stdint.h>

// inportb provides a wrapper around the inb opcode.
static inline uint8_t
inportb ( uint16_t port )
{
	uint8_t data;
	asm volatile ( "inb %1, %0" : "=a" (data) : "Nd" (port) );
	return data;
}

// outportb provides a wrapper around the outb opcode.
static inline void
outportb ( uint16_t port, uint8_t data )
{
	asm volatile ( "outb %0, %1" : : "a" (data), "Nd" (port) );
}

// inportw provides a wrapper around the inw opcode.
static inline uint16_t
inportw ( uint16_t port )
{
	uint16_t data;
	asm volatile ( "inw %1, %0" : "=a" (data) : "Nd" (port) );
	return data;
}

// outportw provides a wrapper around the outw opcode.
static inline void
outportw ( uint16_t port, uint16_t data )
{
	asm volatile ( "outw %0, %1" : : "a" (data), "Nd" (port) );
}

// inportl provides a wrapper around the inl opcode.
static inline uint32_t
inportl ( uint16_t port )
{
	uint32_t data;
	asm volatile ( "inl %1, %0" : "=a" (data) : "Nd" (port) );
	return data;
}

// outportl provides a wrapper around the outl opcode.
static inline void
outportl ( uint16_t port, uint32_t data )
{
	asm volatile ( "outl %0, %1" : : "a" (data), "Nd" (port) );
}
