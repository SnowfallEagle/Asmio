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
        sub     esp, 256    ; 256 byte string

        mov     esi, esp    ; esp -> esi
        read    stdin, esi, 256 ; read string
        cmp     eax, EOF    ; if not EOF
        je      .clean      ;   jump to clean
        write   stdout, esi, eax    ; write what we`ve read

;; Check open/close operations
		open	fn, 0x241, 666q	; open file
		cmp		eax, -1		; if -1 returned
		je		.clean		;   jump to clean
		write	eax, esi, eax	; write to file
        close   eax         ; close file

.clean: add     esp, 256    ; free string
        exit                ; successful exit
