@---------------------------------------------------------------------------------
@ 3DS processor selection
@---------------------------------------------------------------------------------
    .cpu mpcore
@---------------------------------------------------------------------------------

@---------------------------------------------------------------------------------
    .section ".crt0"
    .global _start, __service_ptr
@---------------------------------------------------------------------------------
    .align 4
    .arm
@---------------------------------------------------------------------------------
_start:
@---------------------------------------------------------------------------------
    b startup
__service_ptr:
    .word 0 @ Pointer to service handle override list -- necessary to use some of ctrulib functions
startup:
    @ Save return address
    mov     r4, lr

    @ Clear the BSS section
    ldr     r0, =__bss_start__
    ldr     r1, =__bss_end__
    sub     r1, r1, r0
    bl      ClearMem

    @ Jump to plugin init code
    ldr     R0, [SP]
    bl      __entrypoint

    @ Jump back to plugin loader hook
    mov     pc, r4

@---------------------------------------------------------------------------------
@ Clear memory to 0x00 if length != 0
@  r0 = Start Address
@  r1 = Length
@---------------------------------------------------------------------------------
ClearMem:
@---------------------------------------------------------------------------------
    mov     r2, #3     @ Round down to nearest word boundary
    add     r1, r1, r2 @ Shouldnt be needed
    bics    r1, r1, r2 @ Clear 2 LSB (and set Z)
    bxeq    lr         @ Quit if copy size is 0

    mov     r2, #0
ClrLoop:
    stmia   r0!, {r2}
    subs    r1, r1, #4
    bne     ClrLoop

    bx      lr
