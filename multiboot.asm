;
; multiboot.asm
; author: Adam Vandolder
; created: July 4, 2014
; last edited: July 4, 2014
;
; This is the entry point of the entire navi operating system.
; It's booted by grub (or another multiboot-compliant boot-loader),
; then calls up the rest of the kernel, which then runs the OS.
;

	format	elf
	use32

;
; Multiboot constants
;

MB_MAGIC	equ 1BADB002h
MB_PAGE_ALIGN	equ (1 shl 0)
MB_MEM_INFO	equ (1 shl 1)
MB_FLAGS	equ MB_PAGE_ALIGN or MB_MEM_INFO
MB_CHECKSUM	equ -(MB_MAGIC + MB_FLAGS)

;
; Multiboot header
;
	section '.multiboot'

	align	4
	dd MB_MAGIC
	dd MB_FLAGS
	dd MB_CHECKSUM

;
; 16kb Stack for the Kernel
;
	section '.bootstrap_stack'

	align	4
    stack_bottom:
	times	16384 db 0
    stack_top:

;
; Start of code
;

	section '.text'

;
; Starting point
;
	public	start
	extrn	kernel_main

    start:
	;
	; Set up the stack by making esp set to the top of it (it grows down)
	;
	mov	esp,stack_top

	;
	; Call kernel_main in kernel.c, which continues loading and running
	; the navi kernel.
	;
	call	kernel_main

	;
	; After kernel has returned (OS finished execution), stop interrupts
	; from being handled and halt the CPU. Jump to a hang label, just in
	; case this fails from stopping execution.
	;
	cli
	hlt

      .hang:
	jmp	.hang
