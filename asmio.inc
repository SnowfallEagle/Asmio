;;; License: LGPL-2.1
;;; Copyright (C) 2022 Sergey Nikonov (17seannnn)
;;; CDECL - macroses'll spoil eax, ecx, edx registers

;; Input/Output definitions
%define stdin  0
%define stdout 1
%define stderr 2
%define EOF -1

;; Open definitions
%define O_RDONLY 0x000
%define O_WRONLY 0x001
%define O_RDWR   0x002
%define O_CREAT  0x040
%define O_EXCL   0x080
%define O_TRUNC  0x200
%define O_APPEND 0x400

;; Additional Open definitions
%define AO_R  0x000
%define AO_R+ 0x002
%define AO_W  0x241
%define AO_W+ 0x242
%define AO_A  0x440

;; Mode Open definitions
%define MO_ALL     666q
%define MO_CREATOR 600q

;; Macroses
; _syscall(n, ...)
; return eax
%macro _syscall 1-4
        push    ebx         ; save ebx

    %if %0 == 4             ; get args
        push    %4
    %endif
    %if %0 >= 3
        push    %3
    %endif
    %if %0 >= 2
        push    %2
    %endif
        push    %1

        pop     eax         ; store them in registers
    %if %0 >= 2
        pop     ebx
    %endif
    %if %0 >= 3
        pop     ecx
    %endif
    %if %0 >= 4
        pop     edx
    %endif

        int     0x80        ; syscall

        pop     ebx         ; restore ebx
%endmacro

; vcall(function, ...)
%macro vcall 1-*
  %rep %0 - 1                       ; push all args from the end
    %rotate -1
        push    %1
  %endrep                           ; function become last in the list of args
  %rotate -1                        ; make it first
        call    %1                  ; call it
        add     esp, (%0 - 1) * 4   ; clean stack
%endmacro

; exit(status)
%macro exit 0-1 0
        _syscall    1, %1   ; exit with status in first arg
%endmacro

; read(file_descriptor, buffer, count)
; return how much characters have been read, EOF on end of file
%macro read 3
        _syscall    3, %1, %2, %3
%endmacro

; write(file_descriptor, buffer, count)
; return how much characters have been written
%macro write 3
        _syscall    4, %1, %2, %3
%endmacro

; open(file_path that have to has null-byte)
; return file descriptor, -1 on error
%macro open 2-3
    %if %0 == 3
        _syscall    5, %1, %2, %3
    %else
        _syscall    5, %1, %2
    %endif
%endmacro

; close(file_descriptor)
; return 0 on success, -1 on error
%macro close 1
        _syscall    6, %1
%endmacro

; getargc(register)
; return argc, works only on start when esp is not spoiled
%macro getargc 1
        mov     %1, [esp]
%endmacro

; getargv(register)
; return address of first arg pointer, to go through add 4 to
;   the address and get value from memory to access string
%macro getargv 1
        lea     %1, [esp+4]
%endmacro

; getc()
; return character in AL, -1 on EOF
%macro getc 0
        pushf               ; save flags
        push    edi         ; save edi
        sub     esp, 4      ; local variable for buffer
        mov     edi, esp    ; edi == buffer

        read    stdin, edi, 1   ; read from stdout, in edi, 1 character
        test    eax, eax        ; if read count > 1
        jnz     %%success       ;   jump to success
        xor     eax, eax        ; indicate error, eax = -1
        not     eax
        jmp     %%return        ; return value
%%success:
        mov     al, [edi]   ; [edi] -> al

%%return:
        add     esp, 4      ; free local variable
        pop     edi         ; restore edi
        popf                ; restore flags
%%end:
%endmacro

; putc(1 byte register)
; return 1 on success, 0 on fail
%macro putc 1
        pushf               ; save flags
        push    esi         ; save esi
        sub     esp, 4      ; local variable
        mov     esi, esp    ; esi == buffer

        mov byte [esi], %1      ; first arg -> local variable
        write   stdout, esi, 1  ; write in stdout, from esi, 1 character

        add     esp, 4      ; free memory
        pop     esi         ; restore esi
        popf                ; restore flags
%endmacro

; strlen(null-terminated string)
; return string length
%macro strlen 1
        pushf               ; save flags

        mov     edx, %1     ; first arg -> edx
        xor     eax, eax    ; clean counter

%%lp:
        mov     cl, [edx]   ; [edx] -> cl
        test    cl, cl      ; if ecx == 0
        jz      %%return    ;   jump to return
        inc     eax         ; increment counter
        inc     edx         ; increment address
        jmp     %%lp        ; jump to loop

%%return:
        popf                ; restore flags
%endmacro

; puts(null-terminated string)
; return non-zero value on success
%macro puts 1
        pushf               ; save flags
        push    esi         ; save esi

        mov     esi, %1             ; first arg -> esi
        strlen  esi                 ; string length -> eax
        write   stdout, esi, eax    ; write in stdout, from esi, eax characters
        putc    10                  ; new line

        pop     esi         ; restore esi
        popf                ; restore flags
%endmacro

; gets(buffer, buffer size)
; return -1 on error, 0 on success
%macro gets 2
        pushf               ; save flags
        push    edi         ; save edi

        push dword %2       ; get args
        push    %1
        pop     edi         ; eax = buffer
        pop     ecx         ; ecx = buffer size

        test    ecx, ecx    ; if ecx != 0
        jnz     %%get       ;   jump to get string
        mov     eax, -1     ; error -> eax
        jmp     %%return    ; jump to return

%%get:  read    stdin, edi, ecx ; read string
        test    eax, eax        ; eax contains how much characters`ve 
                                ;   been read
        jz      %%null          ; if nothing was read
                                ;   just set null byte in buf[0]
        dec     eax             ; but if we read something
                                ;   we should set 0 on '\n'
%%null: mov byte [edi+eax], 0   ; set null-byte
        mov     eax, 0          ; return success
%%return:
        pop     edi         ; restore edi
        popf                ; restore flags
%endmacro

; print(string)
; macro print function, return non-zero value on success
%macro print 1
        pushf               ; save flags

        jmp     %%do                    ; jump to do
%%str   db      %1, 10                  ; define string
%%strln equ     $-%%str                 ; its len
%%do:   write   stdout, %%str, %%strln  ; write

        popf                ; restore flags
%endmacro
