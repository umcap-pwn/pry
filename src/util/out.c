#include "pry/out.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static bool color_on;

void out_init(FILE *stream, enum out_color_mode mode)
{
	switch (mode) {
	case OUT_COLOR_ALWAYS:
		color_on = true;
		return;
	case OUT_COLOR_NEVER:
		color_on = false;
		return;
	case OUT_COLOR_AUTO:
		break;
	}

	const char *nocolor = getenv("NO_COLOR");
	const char *term = getenv("TERM");
	color_on = isatty(fileno(stream)) && !(nocolor && nocolor[0] != '\0') &&
		   !(term && strcmp(term, "dumb") == 0);
}

const char *out_color(enum out_color c)
{
	if (!color_on)
		return "";

	static const char *table[] = {
	    [OUT_RESET] = "\033[0m", [OUT_BOLD] = "\033[1m",   [OUT_DIM] = "\033[2m",
	    [OUT_RED] = "\033[31m",  [OUT_GREEN] = "\033[32m", [OUT_YELLOW] = "\033[33m",
	    [OUT_BLUE] = "\033[34m", [OUT_CYAN] = "\033[36m",
	};

	return table[c];
}
