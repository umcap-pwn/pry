#include "pry/cmd.h"
#include "pry/util.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

const struct cmd cmds[] = {
    {"hex", "hexdump / extract, vaddr-offset", cmd_hex},
    {"elf", "ELF headers, sections, symbols ", cmd_elf},
    {"str", "extract strings                ", cmd_str},
    {"sec", "hardening/mitigations check    ", cmd_sec},
    {"asm", "disassemble with capstone      ", cmd_asm},
    {"inf", "one-screen summary             ", cmd_inf},
};
const size_t cmds_count = sizeof(cmds) / sizeof(cmds[0]);

void cmd_usage(void)
{
	die("usage stub");
}

int main(int argc, char **argv)
{
	if (argc < 2)
		cmd_usage();

	for (size_t i = 0; i < cmds_count; i++) {
		if (strcmp(argv[1], cmds[i].name) == 0) {
			return cmds[i].entry(argc - 1, argv + 1);
		}
	}

	die("unknown command: %s", argv[1]);
}
