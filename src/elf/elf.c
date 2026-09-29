#include "pry/elf.h"
#include "pry/cmd.h"
#include "pry/util.h"
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

/*
 * Function to open elf file and fill up the elf struct
 * Both arguments must be nonnul valid pointers
 */
int elf_open(struct elf *elf, const char *path)
{
	/* Elf_open is not idempotent. caller must provide empty struct elf;
	 * memory leaks possible otherwise */
	memset(elf, 0, sizeof(struct elf));

	int fd = open(path, 0, O_RDONLY);
	if (fd == -1) {
		perror("cannot open file");
		exit(EXIT_FAILURE);
	}

	struct stat file_stat = {0};
	int res = fstat(fd, &file_stat);
	if (res == -1) {
		perror("cannot stat file");
	}

	/* By now and for the forseeable future
	 * tool would support only regular files */
	if (!S_ISREG(file_stat.st_mode)) {
		die("refusing to process \"%s\": it is not a regular file", path);
	}

	void *map = mmap(NULL, (size_t)file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	struct elf e = {
	    fd,	  map,	(size_t)file_stat.st_size,

	    0,	  0,	0,
	    0,

	    NULL, NULL, 0,
	    0,	  NULL, 0,
	    0,	  0,
	};

	*elf = e;

	close(fd);
	return 0;
}

void elf_close(struct elf *elf)
{
	munmap(elf->map, elf->size);
	close(elf->fd);
}
