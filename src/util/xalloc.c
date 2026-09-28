/*
 * src/util/xalloc.c - Malloc error handling wrappers
 *
 * In this small cli-oriented project, OOM handling would be unnecessarily
 * complex. Therefore, the set of x-functions is introduced in this file.
 * These functions wrap basic libc functions to enforce non-null return or
 * die on OOM.
 *
 * Invariant: all alloc functions in this file return valid heap pointers,
 * and crash with error message on OOM.
 */

#include "pry/util.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *xmalloc(size_t size)
{
	void *ptr = malloc(size);
	if (!ptr) {
		die("Cannot allocate the buffer with size %zu: out of memory!", size);
	}
	return ptr;
}

void *xcalloc(size_t memb_size, size_t count)
{
	void *ptr = calloc(memb_size, count);
	if (!ptr) {
		die("Cannot allocate the buffer with size %zu: out of memory!", memb_size * count);
	}
	return ptr;
}

void *xrealloc(void *buf, size_t size)
{
	void *ptr = realloc(buf, size);
	if (!ptr) {
		die("Cannot reallocate the buffer with new size %zu: out of memory!", size);
	}
	return ptr;
}

char *xstrdup(const char *str)
{
	size_t len = strlen(str);
	char *alloc = xmalloc(len + 1);
	memcpy(alloc, str, len);
	alloc[len] = '\0';
	return alloc;
}

char *xstrndup(const char *str, size_t len)
{
	char *alloc = xmalloc(len + 1);
	memcpy(alloc, str, len);
	alloc[len] = '\0';
	return alloc;
}

void die(const char *fmt, ...)
{
	va_list ap;
	fprintf(stderr, "%s: ", PROGNAME);
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	exit(EXIT_FAILURE);
}
