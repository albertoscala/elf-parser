#pragma once

#include <vector>

#include "elf_types.hpp"

namespace elf_program_header
{
    class ELF_Program_Header
    {
        elf_types::Elf64_Word p_type;      /* Type of segment */
        elf_types::Elf64_Word p_flags;     /* Segment attributes */
        elf_types::Elf64_Off p_offset;     /* Offset in file */
        elf_types::Elf64_Addr p_vaddr;     /* Virtual address in memory */
        elf_types::Elf64_Addr p_paddr;     /* Reserved */
        elf_types::Elf64_Xword p_filesz;   /* Size of segment in file */
        elf_types::Elf64_Xword p_memsz;    /* Size of segment in memory */
        elf_types::Elf64_Xword p_align;    /* Alignment of segment */
    };

    class ELF_Program_Header_Table
    {
    private:
        std::vector<ELF_Program_Header> program_headers;
    public:
        ELF_Program_Header_Table() = default;
        ELF_Program_Header_Table(std::byte* elf_file)
        {

        }
    
    };
}