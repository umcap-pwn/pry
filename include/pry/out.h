#ifndef PRY_OUT_H
#define PRY_OUT_H

#include <stdbool.h>
#include <stdio.h>

enum out_color {
	OUT_RESET = 0,
	OUT_BOLD,
	OUT_DIM,
	OUT_RED,
	OUT_GREEN,
	OUT_YELLOW,
	OUT_BLUE,
	OUT_CYAN,
};

enum out_color_mode {
	OUT_COLOR_AUTO = 0,
	OUT_COLOR_ALWAYS,
	OUT_COLOR_NEVER,
};

void out_init(FILE *stream, enum out_color_mode mode);

const char *out_color(enum out_color c);

#endif
