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
;; Print check
        print   "print..."
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
        puts dword [edi]        ; put string from 1 arg
        add     edi, 4          ; get next address
        jmp     .lp             ; jump to loop

;; Check string operations
.next:
        sub     esp, 256        ; 256 byte string
        mov     esi, esp        ; string == esi

        gets    esi, 256        ; get string
        cmp     eax, -1         ; if fail
        je      .clean          ;   jump to clean

;; Check open/close operations
        open    fn, AO_W, MO_ALL    ; create file for all for writing
        cmp     eax, -1             ; if -1 returned
        je      .clean              ;   jump to clean
        fputs   eax, esi            ; write to file
        close   eax                 ; close file

.clean: add     esp, 256    ; free string and integer
        exit                ; successful exit
