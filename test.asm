; CDECL
%include "asmio.inc"

global _start

section .text
;; Check char operations
_start: getc                ; get character
        putc    al          ; put character

;; Check string operations
        sub     esp, 256    ; 256 byte string

        mov     esi, esp    ; esp -> esi
        read    stdin, esi, 256 ; read string
        cmp     eax, EOF    ; if not EOF
        je      .clean      ;   jump to clean
        write   stdout, esi, eax    ; write what we`ve read

.clean: add     esp, 256    ; free string

        exit                ; successful exit
