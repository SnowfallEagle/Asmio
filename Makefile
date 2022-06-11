AS=nasm
ASFLAGS=-g -f elf
LDFLAGS=-m elf_i386
SCRIPT=test.sh

%.o: %.asm asmio.inc
	$(AS) $(ASFLAGS) $< -o $@

test: test.o
	$(LD) $(LDFLAGS) $^ -o $@
	$(SHELL) $(SCRIPT)

clean:
	$(RM) -f *.o *.s test log
