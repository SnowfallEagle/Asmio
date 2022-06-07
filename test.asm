; CDECL
%include "asmio.inc"

global _start

section .text
_start: getc                ; get character
        putc    al          ; put character

        mov     eax, 1      ; exit
        mov     ebx, 0      ; status 0
        int     0x80        ; syscall
