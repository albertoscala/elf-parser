#pragma once

#include <vector>

#include "elf_types.hpp"
#include "elf_header.hpp"

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
        ELF_Program_Header_Table(const elf_header::ELF_Header& elf_header, std::byte* elf_file, std::size_t size)
        {
            // const std::byte* cursor = elf_file + elf_header;

            // auto read = [&](auto& field)
            // {
            //     std::copy(cursor, cursor + field.size(), field.data());
            //     cursor += field.size();
            // };
        }
    
    };
}