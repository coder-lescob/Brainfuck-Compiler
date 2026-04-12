section .data
tape: times 30000 dw 0
tape_len: equ tape - $$

section .text
global _start
_start:
; r8 is the tape pointer

    mov rax, 0x00        ; read sysacall
    mov rdi, 0x01        ; stdin
    mov rsi, tape        ; buf
    add rsi, r8
    mov rdx, 0x01        ; a single char
    syscall

    inc byte [tape + r8]

    mov rax, 0x01        ; write syscall
    mov rdi, 0x01        ; stdout
    mov rsi, tape        ; buf
    add rsi, r8
    mov rdx, 0x01        ; a single char
    syscall

_exit:
; use the exit sycall to exit
    mov rax, 0x3c      ; syscall exit (60)
    mov rdi, 0x00      ; error code 0
    syscall
