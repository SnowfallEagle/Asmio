asmio
=====

## Set of macroses for simple input/output in x86 Netwide Assembler (NASM) for Linux

### How to use

#### Include in code
``` nasm
; Copy asmio.inc in your project and move this header to your program
%include "asmio.inc"
```

#### Usage examples
You can find usage examples in test.asm file.
Also, you can build this test with make command.

#### Table of file descriptor definitions
| Definition | Meaning |
|---|---|
| stdin  | standard input file descriptor    |
| stdout | standard output file descriptor   |
| stderr | standard error file descriptor    |
| EOF    | some macros return can return EOF |

#### Table of open definitions
| Definition | Meaning |
|---|---|
| O_RDONLY | only reading         |
| O_WRONLY | only reading         |
| O_RDWR   | reading/writing      |
| O_CREAT  | create file          |
| O_EXCL   | have to create file  |
| O_TRUNC  | truncate file        |
| O_APPEND | write in end of file |

#### Table of additional open definitions
| Definition | Meaning |
|---|---|
| AO_R  | only reading              |
| AO_R+ | reading/writing           |
| AO_W  | truncate, writing         |
| AO_W+ | truncate, reading/writing |
| AO_A  | writing in end of file    |

#### Table of mode open definiitions
| Definition | Meaning |
|---|---|
| MO_ALL   | reading/writing for all   |
| MO_OWNER | reading/writing for owner |

#### Table of macros
| Macro | Args | Return |
|---|---|---|
| _syscall | 1-4 args for system calls | same as syscall you calling |
| vcall | 1-* args, first have to be label | same as your function |
| strlen | null-terminated string | its length |
| exit | exit status or nothing on success | nothing can be returned |
| read | file descriptor, buffer, count | how much chars were read |
| write | file descriptor, buffer, count | how much chars were wrote |
| open | file path, how to open, mode only if we create file | file descriptor |
| close | file descriptor | don't check it |
| getargc | where to move args count | nothing |
| getargv | where to move args variables | nothing |
| fgetc | file descriptor | character |
| getc | nothing | character |
| fputc | file descriptor, character | non-zero on success |
| putc | character | non-zero on success
| fputs | file descriptor, null-terminated string | non-zero value on success|
| puts | null-terminated string | non-zero value on success |
| fgets | file descriptor, buffer, size | non-zero value on success |
| gets | string, size | non-zero value |
| fprint file desctiptor, string in "quotes" | non-zero value on success |
| print| string in "quotes" | non-zero on success |
