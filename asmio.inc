%define stdin  0
%define stdout 1
%define stderr 2

; exit(status)
%macro exit 1
        mov     ebx, %1     ; status -> ebx
        mov     eax, 1      ; exit
        int     0x80        ; syscall
%endmacro

; read(file_descriptor, buffer, count)
; returns how much characters have been read
%macro read 3
        push    ebx         ; save ebx
        pushf               ; save flags

        push    %3
        push    %2
        push    %1
        pop     ebx         ; file_descriptor
        pop     ecx         ; buffer
        pop     edx         ; count

        mov     eax, 3      ; read
        int     0x80        ; syscall

        pop     ebx         ; restore ebx
        popf                ; restore flags
%endmacro

; write(file_descriptor, buffer, count)
; returns how much characterss have been written
%macro write 3
        push    ebx         ; save ebx

        push    %3          ; push arguments because it can be lost
        push    %2
        push    %1
        pop     ebx
        pop     ecx
        pop     edx

        mov     eax, 4      ; write
        int     0x80        ; syscall

        pop     ebx         ; restore ebx
%endmacro

; getc()
; return character in AL, -1 on EOF
%macro getc 0
        pushf               ; save flags
        sub     esp, 4      ; local variable for character

        mov     edi, esp    ; esp -> edi, because we can't read from esp
        read    stdin, edi, 1   ; read from stdout, in edi, 1 character

        test    eax, eax    ; if read count > 1
        jnz     %%success   ;   jump to success
        mov     eax, -1     ; indicate error
        jmp     %%return    ; return value
%%success:
        mov     eax, [esp]  ; [esp] -> eax

%%return:
        add     esp, 4      ; free local variable
        popf                ; restore flags
%%end:
%endmacro

; putc(1 byte register)
; return 1 on success, 0 on fail
%macro putc 1
        pushf               ; save flags
        push    ebx         ; save ebx
        sub     esp, 4      ; local variable

        mov     [esp], %1   ; first arg -> local variable
        mov     esi, esp    ; esp -> esi, because we can't write from esp
        write   stdout, esi, 1  ; write in stdout, from esi, 1 character

        add     esi, 4      ; free memory
        pop     ebx         ; restore ebx
        popf                ; restore flags
%endmacro
