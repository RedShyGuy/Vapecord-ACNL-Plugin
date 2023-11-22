.arm
.section .text.__aeabi_read_tp, "ax", %progbits
.global __aeabi_read_tp
.type __aeabi_read_tp, %function
.align 2
__aeabi_read_tp:
	stmfd sp!, {r1-r3, lr}
	bl	__getThreadLocalStorage
	ldmfd sp!, {r1-r3, pc}
