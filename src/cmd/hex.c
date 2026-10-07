#include "pry/cmd.h"
#include "pry/elf.h"
#include <stdio.h>
#include <unistd.h>

int cmd_hex(int argc, char **argv)
{
	struct elf e = {0};
	elf_open(&e, argv[1]);
	elf_open(&e, argv[1]);
	elf_open(&e, argv[1]);

	ssize_t _ = write(STDOUT_FILENO, e.file.map, e.file.size);
	elf_close(&e);
	return 0;
}
