#pragma once

#include <cstdint>
#include <vector>

#include "elf_types.hpp"

namespace elf_section_header
{
    enum class ELF_Section_Type : std::uint32_t
    {
        SHT_NULL = 0,               // Marks an unused section header
        SHT_PROGBITS = 1,           // Contains information defined by the program
        SHT_SYMTAB = 2,             // Contains a linker symbol table
        SHT_STRTAB = 3,             // Contains a string table
        SHT_RELA = 4,               // Contains “Rela” type relocation entries
        SHT_HASH = 5,               // Contains a symbol hash table
        SHT_DYNAMIC = 6,            // Contains dynamic linking tables
        SHT_NOTE = 7,               // Contains note information
        SHT_NOBITS = 8,             // Contains uninitialized space; does not occupy any space in the file
        SHT_REL = 9,                // Contains “Rel” type relocation entries
        SHT_SHLIB = 10,             // Reserved
        SHT_DYNSYM = 11,            // Contains a dynamic loader symboltable
        SHT_LOOS = 0x60000000,      // Environment-specific use
        SHT_HIOS = 0x6FFFFFFF,
        SHT_LOPROC = 0x70000000,    // Processor-specific use
        SHT_HIPROC = 0x7FFFFFFF,
    };

    enum class ELF_Section_Attributes : std::uint32_t
    {
        SHF_WRITE = 0x1,            // Section contains writable data
        SHF_ALLOC = 0x2,            // Section is allocated in memory image of program
        SHF_EXECINSTR = 0x4,        // Section contains executable instructions
        SHF_MASKOS = 0x0F000000,    // Environment-specific use
        SHF_MASKPROC = 0xF0000000,  // Processor-specific use
    };

    class ELF_Section_Header
    {
        elf_types::Elf64_Word sh_name;         /* Section name */
        elf_types::Elf64_Word sh_type;         /* Section type */
        elf_types::Elf64_Xword sh_flags;       /* Section attributes */
        elf_types::Elf64_Addr sh_addr;         /* Virtual address in memory */
        elf_types::Elf64_Off sh_offset;        /* Offset in file */
        elf_types::Elf64_Xword sh_size;        /* Size of section */
        elf_types::Elf64_Word sh_link;         /* Link to other section */
        elf_types::Elf64_Word sh_info;         /* Miscellaneous information */
        elf_types::Elf64_Xword sh_addralign;   /* Address alignment boundary */
        elf_types::Elf64_Xword sh_entsize;     /* Size of entries, if section has table */
    };

    //TODO: String tables

    //TODO: Symbol table

    class ELF_Section_Header_Table
    {
    private:
        std::vector<ELF_Section_Header> section_headers;
    public:
        ELF_Section_Header_Table() = default;
        ELF_Section_Header_Table(std::byte* elf_file)
        {

        }
    };
}