#ifndef PRY_CMD_H
#define PRY_CMD_H

#include <stddef.h>
#include "pry/util.h"

/*
 *  An interface to the entry() of a subcommand.
 */
struct cmd {
	/* These two fields are mostly for user output */
	const char *name;
	const char *desc;
	/*
	 *  subcommand entry() pointer, mirrors `int main (int, char**)`
	 *  entry is called with argv[0] == command name,
	 *  argv[1..argc] - command args.
	 *  @return exit status, nonnull means error.
	 */
	int (*entry)(int argc, char **argv);
};

extern const struct cmd cmds[];
extern const size_t cmds_count;

void cmd_usage(void) NORETURN;

int cmd_hex(int argc, char **argv);
int cmd_elf(int argc, char **argv);
int cmd_str(int argc, char **argv);
int cmd_sec(int argc, char **argv);
int cmd_asm(int argc, char **argv);
int cmd_inf(int argc, char **argv);

#endif /* PRY_CMD_H */
