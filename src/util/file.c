
#include "pry/file.h"
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <assert.h>

int file_open(struct file *f, const char *path)
{

	assert(f->map == NULL);
	memset(f, 0, sizeof(struct file));

	int fd = open(path, 0, O_RDONLY);
	if (fd == -1) {
		close(fd);
		return -1;
	}

	struct stat file_stat = {0};
	int res = fstat(fd, &file_stat);
	if (res == -1) {
		close(fd);
		return -1;
	}

	/* By now and for the forseeable future
	 * tool would support only regular files */
	if (!S_ISREG(file_stat.st_mode)) {
		errno = EINVAL;
		close(fd);
		return -1;
	}

	if (file_stat.st_size <= 0) {
		errno = EINVAL;
		close(fd);
		return -1;
	}

	void *map = mmap(NULL, (size_t)file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED) {
		close(fd);
		return -1;
	}

	*f = (struct file){fd, map, (size_t)file_stat.st_size};
	return 0;
}

void file_close(struct file *f)
{
	if (f->map) {
		munmap(f->map, f->size);
		close(f->fd);
	}
	memset(f, 0, sizeof(struct file));
}
