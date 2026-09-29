/*
 * include/pry/elf.h - definitions for elf type and elf-related functions.
 */

#ifndef PRY_ELF_H
#define PRY_ELF_H

#include <elf.h>
#include <stddef.h>
#include <stdint.h>

/*
 * The elf type, which holds the handles, key properties
 * and mappings for an elf file. It is assumed that a single instance
 * of this structure exists thoroughout the entire program runtime.
 */
struct elf {
	/* Part 1: ownership */
	int fd;
	void *map;
	size_t size;

	/* Part 2: classification */
	int elf_class;
	int endian;
	uint16_t e_type;
	uint16_t e_machine;

	/* Part 3: mapping */
	const void *ehdr;
	const void *phdr;
	size_t phnum;
	size_t phentsize;
	const void *shdr;
	size_t shnum;
	size_t shentsize;
	size_t shstrndx;
};

int elf_open(struct elf *elf, const char *path);
void elf_close(struct elf *elf);

#endif /* PRY_ELF_H */
