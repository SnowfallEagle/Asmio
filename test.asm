;;; License: LGPL-2.1
;;; Copyright (C) 2022 Sergey Nikonov (17seannnn)
;;; CDECL

%include "asmio.inc"

global _start

section .data
fn      db "log", 0

section .text
test_vcall:
        mov     eax, [esp+4]    ; get first argument
        add     eax, "0"        ; make character from 1 number integer
        putc    al              ; put character
        putc    10              ; new line
        ret                     ; return

_start:
;; Show arguments count
        getargc eax             ; argc -> eax
        sub     esp, 4          ; local variable for argc
        mov     edi, esp        ; edi = &argc
        mov     [edi], eax      ; eax -> argc
        vcall   test_vcall, eax ; test vcall macros
        mov     eax, [edi]      ; get argc
        add     esp, 4          ; free local variable

;; Show all arguments
        getargv edi             ; argv -> edi
.lp:    mov     ebx, [edi]      ; argv string address -> ebx
        test    ebx, ebx        ; if ebx == 0
        jz      .next           ;   jump to next
        puts    [edi]           ; put string from 1 arg
        add     edi, 4          ; get next address
        jmp     .lp             ; jump to loop

;; Check string operations
.next:
        sub     esp, 260        ; 256 byte string + 4 byte integer
        lea     esi, [esp+4]    ; string == esi
        mov     edi, esp        ; integer == edi

        gets    esi, 256            ; get string
        cmp     eax, -1             ; if fail
        je      .clean              ;   jump to clean
        puts    esi                 ; put this string

;; Check open/close operations
        strlen  esi                     ; string length -> eax
        mov     [edi], eax              ; eax -> integer
        open    fn, AO_W, MO_ALL        ; create file for all for writing
        cmp     eax, -1                 ; if -1 returned
        je      .clean                  ;   jump to clean
        write   eax, esi, dword [edi]   ; write to file
        close   eax                     ; close file

.clean: add     esp, 260    ; free string and integer
        exit                ; successful exit
