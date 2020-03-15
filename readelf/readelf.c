/* This is a simplefied ELF reader.
 * You can contact me if you find any bugs.
 *
 * Luming Wang<wlm199558@126.com>
 */

#include "kerelf.h"
#include <stdio.h>

#define EI_DATA (5)

uint32_t rev32Word(uint32_t word) {
    uint32_t revWord = 0;
    revWord = word >> 24;
    revWord |= (word >> 8) & 0x0000FF00;
    revWord |= (word << 8) & 0x00FF0000;
    revWord |= (word << 24) & 0xFF000000;

    return revWord;
}

uint16_t rev16Word(uint16_t half) {
    uint16_t revHalf = 0;
    revHalf = half >> 8;
    revHalf |= (half << 8) & 0xFF00;

    return revHalf;
}

/* Overview:
 *   Check whether it is a ELF file.
 *
 * Pre-Condition:
 *   binary must longer than 4 byte.
 *
 * Post-Condition:
 *   Return 0 if `binary` isn't an elf. Otherwise
 * return 1.
 */
int is_elf_format(u_char *binary)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
        if (ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
                ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
                ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
                ehdr->e_ident[EI_MAG3] == ELFMAG3) {
                return 1;
        }

        return 0;
}

/* Overview:
 *   read an elf format binary file. get ELF's information
 *
 * Pre-Condition:
 *   `binary` can't be NULL and `size` is the size of binary.
 *
 * Post-Condition:
 *   Return 0 if success. Otherwise return < 0.
 *   If success, output address of every section in ELF.
 */

/*
    Exercise 1.2. Please complete func "readelf". 
*/
int readelf(u_char *binary, int size)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;

        int Nr;

        int isBigEndian = 0;

        Elf32_Shdr *shdr = NULL;
        Elf32_Phdr *phdr = NULL;

        u_char *ptr_sh_table = NULL;
        Elf32_Half sh_entry_count;
        Elf32_Half sh_entry_size;

        u_char *ptr_ph_table = NULL;
        Elf32_Half ph_entry_count;
        Elf32_Half ph_entry_size;
        // check whether `binary` is a ELF file.
        if (size < 4 || !is_elf_format(binary)) {
                printf("not a standard elf format\n");
                return 0;
        }
        // check if is Big Endian
        isBigEndian = ehdr->e_ident[EI_DATA] == 2;

        // get section table addr, section header number and section header size.
		int real_shoff = isBigEndian ? rev32Word(ehdr->e_shoff) : ehdr->e_shoff;
        shdr = (Elf32_Shdr *)(real_shoff + (u_char *)binary);
		sh_entry_count = isBigEndian ? rev16Word(ehdr->e_shnum) : ehdr->e_shnum;
		sh_entry_size = isBigEndian ? rev16Word(ehdr->e_shentsize) : ehdr->e_shentsize;

        // get program table addr, program header number and program header size.
		int real_phoff = isBigEndian ? rev32Word(ehdr->e_phoff) : ehdr->e_phoff;
        phdr = (Elf32_Phdr *)(real_phoff + (u_char *)binary);
		ph_entry_count = isBigEndian ? rev16Word(ehdr->e_phnum) : ehdr->e_phnum;
		ph_entry_size = isBigEndian ? rev16Word(ehdr->e_phentsize) : ehdr->e_phentsize;

        // for each section header, output section number and section addr.
		/*
        int i = 0;
		for (i = 0; i < sh_entry_count; i++) {
            uint32_t real_sh_addr = isBigEndian ? rev32Word(shdr[i].sh_addr) : shdr[i].sh_addr;
			printf("%d:0x%x\n", i, real_sh_addr);
		}
        */

        int i = 0;
		for (i = 0; i < ph_entry_count; i++) {
            uint32_t real_ph_filesz = isBigEndian ? rev32Word(phdr[i].p_filesz) : phdr[i].p_filesz;
            uint32_t real_ph_memsz = isBigEndian ? rev32Word(phdr[i].p_memsz) : phdr[i].p_memsz;

			printf("%d:0x%x,0x%x\n", i, real_ph_filesz, real_ph_memsz);
		}

        // hint: section number starts at 0.


        return 0;
}

