#include "pry/file.h"
#include "pry/cmd.h"
#include "pry/util.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

static char strip_to_printable(char c)
{
	if (c >= ' ' && c <= '~')
		return c;
	else
		return '.';
}

static void print_bytes_as_string(const char *str, size_t count)
{
	struct strbuf s = {0};
	strbuf_init(&s);
	strbuf_append(&s, str, count + 1);
	for (size_t i = 0; i < count; i++)
		s.buf[i] = strip_to_printable(s.buf[i]);
	s.buf[count] = '\0';
	// size_t _ = write(STDOUT_FILENO, s.buf, count);
	printf("%s", s.buf);
	strbuf_free(&s);
}

int cmd_hex(int argc, char **argv)
{
	struct file f = {0};
	int err = file_open(&f, argv[1]);
	if (err) {
		printf("%d", err);
		return -1;
	}

	for (size_t i = 0; i < f.size; i += 2) {
		if (i % 16 == 0) {
			if (i != 0) {
				printf(" ");
				print_bytes_as_string((char *)f.map + i - 16, 16);
				printf("\n");
			}
			printf("%.8x: ", i);
		}
		printf("%.2x%.2x ", ((unsigned char *)f.map)[i], ((unsigned char *)f.map)[i + 1]);
	}


	return 0;
}
