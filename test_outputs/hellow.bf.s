section .data
tape: times 30000 dw 0
tape_len: equ tape - $$

section .text
global _start
_start:
; r8 is the tape pointer
    inc byte [tape + r8]
    inc byte [tape + r8]
    inc byte [tape + r8]
    inc byte [tape + r8]
    inc byte [tape + r8]
    inc byte [tape + r8]
    inc byte [tape + r8]
    inc byte [tape + r8]
    inc byte [tape + r8]
    inc byte [tape + r8]
    inc byte [tape + r8]
open_0:
    cmp byte [tape + r8], 0
    je close_0
    inc r8
    inc byte [tape + r8]
    inc byte [tape + r8]
    inc byte [tape + r8]
    inc byte [tape + r8]
    inc byte [tape + r8]
    inc byte [tape + r8]
    inc byte [tape + r8]
    dec r8
    dec byte [tape + r8]
    jmp open_0
close_0:
    inc r8
    inc byte [tape + r8]
    inc byte [tape + r8]
_exit:
; use the exit sycall to exit
    mov rax, 0x3c 	 	 ; syscall exit (60)
    mov rdi, 0x00 	 	 ; error code 0
    syscall
