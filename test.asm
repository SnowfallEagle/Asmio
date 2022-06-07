; CDECL
%include "asmio.inc"

global _start

section .text
_start: getc                ; get character
        putc    al          ; put character
        exit    0           ; successful exit
