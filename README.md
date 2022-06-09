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
