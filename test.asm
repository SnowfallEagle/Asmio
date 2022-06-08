; CDECL
%include "asmio.inc"

global _start

section .data
fn      db "log", 0

section .text
;; Check char operations
_start: getc                ; get character
        putc    al          ; put character

;; Check string operations
        sub     esp, 260    ; 256 byte string + 4 byte integer
        lea     esi, [esp+4]    ; string == esi
        lea     edi, [esp]  ; integer == edi

        read    stdin, esi, 256 ; read string
        cmp     eax, EOF    ; if not EOF
        je      .clean      ;   jump to clean
        mov     [edi], eax  ; eax -> integer
        write   stdout, esi, eax    ; write what we`ve read

;; Check open/close operations
        open    fn, AO_W, 666q   ; create file for writing, truncate it
        cmp     eax, -1     ; if -1 returned
        je      .clean      ;   jump to clean
        write   eax, esi, dword [edi] ; write to file
        close   eax         ; close file

.clean: add     esp, 260    ; free string and integer
        exit                ; successful exit
