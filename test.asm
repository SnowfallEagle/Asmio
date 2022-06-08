; CDECL
%include "asmio.inc"

global _start

section .data
fn		db "log", 0

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
        mov     ebx, O_WRONLY   ; new file flags
        or      ebx, O_CREAT
        or      ebx, O_TRUNC
		open	fn, ebx, 666q ; open file
		cmp		eax, -1		; if -1 returned
		je		.clean		;   jump to clean
		write	eax, esi, dword [edi] ; write to file
        close   eax         ; close file

.clean: add     esp, 260    ; free string and integer
        exit                ; successful exit
