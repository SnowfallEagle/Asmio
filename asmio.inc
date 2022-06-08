;;; License: LGPL-2.1
;;; Copyright (C) 2022 Sergey Nikonov (17seannnn)

;; Standard definitions
%define stdin  0
%define stdout 1
%define stderr 2

%define EOF -1

%define O_RDONLY 0x000
%define O_WRONLY 0x001
%define O_RDWR   0x002
%define O_CREAT  0x040
%define O_EXCL   0x080
%define O_TRUNC  0x200
%define O_APPEND 0x400

;; Macroses
; exit(status)
%macro exit 0-1 0
        mov     ebx, %1     ; status -> ebx
        mov     eax, 1      ; exit
        int     0x80        ; syscall
%endmacro

; read(file_descriptor, buffer, count)
; returns how much characters have been read, EOF on end of file
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

; open(file_path that have to has null-byte)
; returns file descriptor, -1 on error
%macro open 2-3
		pushf				; save flags

	%if %0 == 3				; put args in registers
		push	%3
	%endif
		push	%2
		push	%1
		pop		ebx
		pop		ecx
	%if %0 == 3
		pop		edx
	%endif

		mov		eax, 5		; open
		int		0x80		; syscall

		popf				; restore flags
%endmacro

; close(file_descriptor)
; returns 0 on success, -1 on error
%macro close 1
        mov     ebx, %1     ; first arg -> ebx
        mov     eax, 6      ; close
        int     0x80        ; syscall
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
        mov     al, [edi]   ; [edi] -> al

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
