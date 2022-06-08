;;; License: LGPL-2.1
;;; Copyright (C) 2022 Sergey Nikonov (17seannnn)
;;; CDECL

%include "asmio.inc"

global _start

section .data
fn      db "log", 0

section .text
_start:
;; Show arguments count
        getargc eax             ; argc -> eax
        add     eax, "0"        ; make character from 1 number integer
        putc    al              ; put character
        putc    10              ; new line

;; Show argv[0]
        getargv edi             ; argv -> edi
        puts    [edi]           ; put string from 1 arg

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
        open    fn, AO_W, MO_ALL    ; create file for all for writing
        cmp     eax, -1     ; if -1 returned
        je      .clean      ;   jump to clean
        write   eax, esi, dword [edi] ; write to file
        close   eax         ; close file

.clean: add     esp, 260    ; free string and integer
        exit                ; successful exit
