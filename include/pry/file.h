/*
 * include/pry/file.h - definitions for general file-related ops
 */

#ifndef PRY_FILE_H
#define PRY_FILE_H

#include <stddef.h>

/*
 * The structure for interfacing with file and it's contents
 *
 * Either `f == {0}` *or* `f` holds a valid fd *and*
 * a mapping of `size` bytes
 */
struct file {
	int fd;
	void *map;
	size_t size;
};

int file_open(struct file *f, const char *path);
void file_close(struct file *f);

#endif /* PRY_FILE_H */
