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

_start: print   "> Arguments count"
        getargc eax             ; argc -> eax
        sub     esp, 4          ; local variable for argc
        mov     edi, esp        ; edi = &argc
        mov     [edi], eax      ; eax -> argc
        vcall   test_vcall, eax ; test vcall macros
        mov     eax, [edi]      ; get argc
        add     esp, 4          ; free local variable

        print   "> Arguments` variables"
        getargv edi             ; argv -> edi
.lp:    mov     ebx, [edi]      ; argv string address -> ebx
        test    ebx, ebx        ; if ebx == 0
        jz      .next           ;   jump to next
        puts dword [edi]        ; put string from 1 arg
        add     edi, 4          ; get next address
        jmp     .lp             ; jump to loop

.next:  print   "> Enter string"
        sub     esp, 256        ; 256 byte string
        mov     esi, esp        ; string == esi

        gets    esi, 256        ; get string
        cmp     eax, -1         ; if fail
        je      .clean          ;   jump to clean

        open    fn, AO_W, MO_ALL    ; create file for all for writing
        cmp     eax, -1             ; if -1 returned
        je      .clean              ;   jump to clean
        mov     edi, eax            ; edi = eax, because eax'll be spoiled
        fputs   edi, esi            ; write to file
        close   edi                 ; close to reopen
        open    fn, AO_R            ; open again to set seek on beginning
        mov     edi, eax            ; edi = fd
        print   "> Character from file"
        fgetc   edi                 ; get char from file
        putc    al                  ; put character
        putc    10                  ; put new line
        strlen  esi                 ; get length of string
        ;fgets   esi, eax            ; get what remain from this string
        print   "> What remain from this string"
        ;puts    esi
        close   edi                 ; close file

.clean: print   "> Check log file"
        add     esp, 256    ; free string and integer
        exit                ; successful exit
