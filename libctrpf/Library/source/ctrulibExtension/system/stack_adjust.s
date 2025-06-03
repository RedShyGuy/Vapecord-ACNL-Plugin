
	.arm
	.align 2

	.global	initLib
	.type	initLib,	%function
initLib:
	ldr	r2, =saved_stack
	str	sp, [r2]
	str	lr, [r2,#4]
	bl	__system_allocateHeaps
	bl	__libc_init_array
	ldr	r2, =saved_stack
	ldr	lr, [r2,#4]
 	bx	lr


	.global	__ctru_exit
	.type	__ctru_exit,	%function

__ctru_exit:
	@bl	__libc_fini_array
	bl	__appExit

	@ldr	r2, =saved_stack
	@ldr	sp, [r2]
	b	__libctru_exit

	.bss
	.align 2
saved_stack:
	.space 8
