.section .text
.global _reset
_reset:
    ldr sp, =_stack_top
    bl _start
    b .
