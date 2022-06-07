; write(file_descriptor, buffer, buffer_size)
; returns how much chars have been written
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

; return character in AL, -1 on EOF
%macro getc 0
        pushf               ; save flags
        sub     esp, 4      ; local variable for character

        mov     eax, 3      ; read
        mov     ebx, 0      ; stdin
        mov     ecx, esp    ; buffer
        mov     edx, 1      ; 1 character
        int     0x80        ; syscall

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
        write   1, esi, 1   ; write in stdout, from esi, 1 character

        add     esi, 4      ; free memory
        pop     ebx         ; restore ebx
        popf                ; restore flags
%endmacro
