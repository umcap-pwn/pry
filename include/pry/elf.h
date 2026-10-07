/*
 * include/pry/elf.h - definitions for elf type and elf-related functions.
 */

#ifndef PRY_ELF_H
#define PRY_ELF_H

#include "pry/file.h"
#include <elf.h>
#include <stddef.h>
#include <stdint.h>

/*
 * The elf type, which holds the handles, key properties
 * and mappings for an elf file. It is assumed that a single instance
 * of this structure exists thoroughout the entire program runtime.
 *
 * `elf` is built on top of `file` structure and used only within
 * elf-specific subcomands. For general file-related defenitions
 * see "include/pry/file.h"
 */
struct elf {
	struct file file;

	int elf_class;
	int endian;
	uint16_t e_type;
	uint16_t e_machine;

	const void *ehdr, *phdr, *shdr;
	size_t phnum, phentsize, shnum, shentsize, shstrndx;
};

enum elf_err {
	ELF_OK = 0,
	ELF_ERR_IO,
	ELF_ERR_TOO_SMALL,
	ELF_ERR_BAD_MAGIC,
	ELF_ERR_BAD_CLASS,
	ELF_ERR_BAD_ENDIAN,
	ELF_ERR_BAD_VERSION,
	ELF_ERR_BAD_PHDR,
	ELF_ERR_BAD_SHDR,
};

/*
 *  On ELF_ERR_IO errno is set and indicating error
 *  cause; it must be used before any other syscall
 */
enum elf_err elf_open(struct elf *elf, const char *path);
void elf_close(struct elf *elf);

#endif /* PRY_ELF_H */
