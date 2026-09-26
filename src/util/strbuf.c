/*
 * src/util/strbuf.c - functions for strbuf struct.
 *
 * This file contains definitions of strbuf-related functions.
 * struct strbuf sb = {0} is a vaild state and it might be passed
 * to the strbuf_append() function directly.
 * Contains ptr to heap allocation, so strbuf_free() is necessary.
 *
 * struct strbuf {
 * 	char  *buf;	<- null-terminated contents
 * 	size_t len;	<- the length of the string
 * 	size_t cap;	<- reserved buffer capacity
 * };
 *
 * Invariants: if (strbuf != NULL) { buf[len] == 0x00; len < cap; }
 * buf, if not null, points to the valid allocation of size cap.
 */

#include "pry/util.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef STRBUF_INIT_CAP
#define STRBUF_INIT_CAP 64 /* Fallback safe default */
#endif

void strbuf_init(struct strbuf *sb)
{
	if (sb->buf)
		return;
	char *buf = xcalloc(1, STRBUF_INIT_CAP);
	sb->buf = buf;
	sb->cap = STRBUF_INIT_CAP;
	sb->len = 0;
}

void strbuf_free(struct strbuf *sb)
{
	free(sb->buf);
	memset(sb, 0, sizeof(struct strbuf)); /* Ignores padding */
}

static void strbuf_grow(struct strbuf *sb, size_t needed)
{
	if (needed > SIZE_MAX / 2 || sb->cap > SIZE_MAX / 2)
		die("strbuf overflow");
	size_t new_cap = sb->cap * 2;
	if (new_cap < needed)
		new_cap = needed;

	sb->buf = xrealloc(sb->buf, new_cap);
	sb->cap = new_cap;
}

void strbuf_append(struct strbuf *sb, const char *str, size_t str_len)
{
	if (!sb->buf)
		strbuf_init(sb);

	if (str_len > sb->cap - (sb->len + 1))
		strbuf_grow(sb, str_len + (sb->len + 1));

	memmove(sb->buf + sb->len, str, str_len);
	sb->len += str_len;
	sb->buf[sb->len] = '\0'; /* xrealloc from grow doesn't null the buffer */
}

void strbuf_appendf(struct strbuf *sb, const char *fmt, ...)
{
	if (!sb->buf)
		strbuf_init(sb);

	va_list ap, ap2;
	int n;

	va_start(ap, fmt);
	va_copy(ap2, ap);
	n = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);

	if (n < 0)
		die("vsnprintf failed!");

	if ((size_t)n > sb->cap - (sb->len + 1))
		strbuf_grow(sb, (size_t)n + (sb->len + 1));

	vsnprintf(sb->buf + sb->len, sb->cap - sb->len, fmt, ap2);
	va_end(ap2);

	sb->len += (size_t)n;
	/* vsnprintf nulls len'th byte of buf */
}

void strbuf_reset(struct strbuf *sb)
{
	if (!sb->buf)
		strbuf_init(sb);
	sb->len = 0;
	sb->buf[sb->len] = '\0';
}
