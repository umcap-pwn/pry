#include "pry/elf.h"
#include "pry/file.h"
#include <string.h>

static enum elf_err elf_validate_header(const Elf64_Ehdr *e, size_t file_size)
{
	/*
	 * For the first version of the project,
	 * several choices were made to facilitate implementation.
	 * Among them:
	 *  - only host endianness is supported
	 *  - only 64-bit binaries are supported
	 */

	if (memcmp(e->e_ident, ELFMAG, SELFMAG) != 0)
		return ELF_ERR_BAD_MAGIC;

	if (e->e_ident[EI_DATA] == ELFDATA2LSB)
		return ELF_ERR_BAD_ENDIAN;
	if (e->e_ident[EI_CLASS] != ELFCLASS64)
		return ELF_ERR_BAD_CLASS;
	if (e->e_ident[EI_VERSION] != EV_CURRENT)
		return ELF_ERR_BAD_VERSION;

	if (e->e_version != EV_CURRENT)
		return ELF_ERR_BAD_VERSION;

	if (e->e_phoff > file_size)
		return ELF_ERR_BAD_PHDR;
	if (((size_t)e->e_phnum * e->e_phentsize) > file_size - e->e_phoff)
		return ELF_ERR_BAD_PHDR;

	if (e->e_shoff > file_size)
		return ELF_ERR_BAD_SHDR;
	if (((size_t)e->e_shnum * e->e_shentsize) > file_size - e->e_shoff)
		return ELF_ERR_BAD_SHDR;

	return ELF_OK;
}

/*
 * Function to open elf file and fill up the elf struct
 */
enum elf_err elf_open(struct elf *elf, const char *path)
{
	memset(elf, 0, sizeof *elf);
	struct file f = {0};
	if (file_open(&f, path) < 0)
		return ELF_ERR_IO;

	if (f.size <= sizeof(Elf64_Ehdr)) {
		file_close(&f);
		return ELF_ERR_TOO_SMALL;
	}
	Elf64_Ehdr *e = f.map;

	enum elf_err err = elf_validate_header(e, f.size);
	if (err)
		return err;

	*elf = (struct elf){
	    .file = f,
	    .elf_class = ELFCLASS64,
	    .endian = ELFDATA2LSB,
	    .e_type = e->e_type,
	    .e_machine = e->e_machine,
	    .ehdr = f.map,
	    .phdr = (char *)f.map + e->e_phoff,
	    .shdr = (char *)f.map + e->e_shoff,
	    .phnum = e->e_phnum,
	    .phentsize = e->e_phentsize,
	    .shnum = e->e_shnum,
	    .shentsize = e->e_shentsize,
	    .shstrndx = e->e_shstrndx,
	};

	return 0;
}

void elf_close(struct elf *elf)
{
	file_close(&elf->file);
	memset(elf, 0, sizeof(struct elf));
}
