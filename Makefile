AS=nasm
ASFLAGS=-g -f elf
LDFLAGS=-m elf_i386

%.o: %.asm asmio.inc
	$(AS) $(ASFLAGS) $< -o $@

test: test.o
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	$(RM) -f *.o *.s test log
