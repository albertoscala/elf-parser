#pragma once

// C++ libs
#include <cstdint>
#include <array>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <iomanip>

// C (linux) libs
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace elf
{
    using Elf64_Addr    = std::array<std::byte, 8>;  // Unsigned program address
    using Elf64_Off     = std::array<std::byte, 8>;  // Unsigned file offset
    using Elf64_Half    = std::array<std::byte, 2>; // Unsigned medium integer
    using Elf64_Word    = std::array<std::byte, 4>;  // Unsigned integer
    using Elf64_Sword   = std::array<std::byte, 4>;  // Signed integer
    using Elf64_Xword   = std::array<std::byte, 8>;  // Unsigned long integer
    using Elf64_Sxword  = std::array<std::byte, 8>;  // Signed long integer

    //     Relocatable File                       Loadable File
    // +----------------------+             +----------------------+
    // |      ELF_HEADER      |             |      ELF_HEADER      |
    // +----------------------+             +----------------------+
    // | Program Header table |             | Program Header table |  
    // |      (otpional)      |             +----------------------+
    // +----------------------+             |      Segment 1       |
    // |      Section 1       |             +----------------------+
    // +----------------------+             |      Segment 2       |
    // |      Section 2       |             +----------------------+
    // +----------------------+             |         ...          |
    // |         ...          |             +----------------------+
    // +----------------------+             | Section Header Table |
    // |      Section N       |             |      (otpional)      |
    // +----------------------+             +----------------------+
    // | Section Header Table |             
    // +----------------------+                          

    enum class ELF_Identification : std::uint8_t
    {
        EI_MAG0 = 0,        // File identification
        EI_MAG1 = 1,
        EI_MAG2 = 2,
        EI_MAG3 = 3,
        EI_CLASS = 4,       // File class
        EI_DATA = 5,        // Data encoding
        EI_VERSION = 6,     // File version
        EI_OSABI = 7,       // OS/ABI identification
        EI_ABIVERSION = 8,  // ABI version
        EI_PAD = 9,         // Start of padding bytes
        EI_NIDENT = 16,     // Size of e_ident[]
    };

    enum class ELF_Class : std::uint8_t
    {
        ELFCLASS32 = 1, // 32-bit objects
        ELFCLASS64 = 2, // 64-bit objects
    };

    enum class ELF_Data_Encoding : std::uint8_t
    {
        ELFDATA2LSB = 1, // Object file data structures are little-endian
        ELFDATA2MSB = 2, // Object file data structures are big-endian
    };

    enum class ELF_OSABI_Identifier : std::uint8_t
    {
        ELFOSABI_SYSV = 0,          // System V ABI
        ELFOSABI_HPUX = 1,          // HP-UX operating system
        ELFOSABI_STANDALONE = 255,  // Standalone (embedded) application
    };

    enum class ELF_Object_File_Type : std::uint16_t
    {
        ET_NONE = 0,        // No file type
        ET_REL = 1,         // Relocatable object file
        ET_EXEC = 2,        // Executable file
        ET_DYN = 3,         // Shared object file
        ET_CORE = 4,        // Core file
        ET_LOOS = 0xFE00,   // Environment-specific use
        ET_HIOS = 0xFEFF,
        ET_LOPROC = 0xFF00, // Processor-specific use
        ET_HIPROC = 0xFFFF,
    };

    class ELF_Header
    {
        std::array<std::uint8_t, 16> e_ident;   /* ELF identification */
        Elf64_Half e_type;                      /* Object file type */
        Elf64_Half e_machine;                   /* Machine type */
        Elf64_Word e_version;                   /* Object file version */
        Elf64_Addr e_entry;                     /* Entry point address */
        Elf64_Off e_phoff;                      /* Program header offset */
        Elf64_Off e_shoff;                      /* Section header offset */
        Elf64_Word e_flags;                     /* Processor-specific flags */
        Elf64_Half e_ehsize;                    /* ELF header size */
        Elf64_Half e_phentsize;                 /* Size of program header entry */
        Elf64_Half e_phnum;                     /* Number of program header entries */
        Elf64_Half e_shentsize;                 /* Size of section header entry */
        Elf64_Half e_shnum;                     /* Number of section header entries */
        Elf64_Half e_shstrndx;                  /* Section name string table index */
    public:
        ELF_Header() = default;
        ELF_Header(const std::byte* elf_file, std::size_t size)
        {
            const std::byte* cursor = elf_file;

            auto read = [&](auto& field)
            {
                std::copy(cursor, cursor + field.size(), field.data());
                cursor += field.size();
            };

            read(e_ident);
            read(e_type);
            read(e_machine);
            read(e_version);
            read(e_entry);
            read(e_phoff);
            read(e_shoff);
            read(e_flags);
            read(e_ehsize);
            read(e_phentsize);
            read(e_phnum);
            read(e_shentsize);
            read(e_shnum);
            read(e_shstrndx);
        }
    
        ~ELF_Header() = default;

        friend std::ostream& operator<<(std::ostream& os, const ELF_Header& elf_header)
        {
            os << "ELF Header:\n";
            os << "  Magic:     ";
            for (auto b : elf_header.e_ident)
                os << std::hex << std::setw(2) << std::setfill('0') 
                << static_cast<int>(b) << " ";
            os << "\n";
            os << "  Class:     " << static_cast<int>(elf_header.e_ident[4]) << "\n";
            os << "  Type:      " << /* your read_half helper */ << "\n";
            // etc...
            return os;
        }

    };

    class ELF_Program_Header
    {
        Elf64_Word p_type;      /* Type of segment */
        Elf64_Word p_flags;     /* Segment attributes */
        Elf64_Off p_offset;     /* Offset in file */
        Elf64_Addr p_vaddr;     /* Virtual address in memory */
        Elf64_Addr p_paddr;     /* Reserved */
        Elf64_Xword p_filesz;   /* Size of segment in file */
        Elf64_Xword p_memsz;    /* Size of segment in memory */
        Elf64_Xword p_align;    /* Alignment of segment */
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
        Elf64_Word sh_name;         /* Section name */
        Elf64_Word sh_type;         /* Section type */
        Elf64_Xword sh_flags;       /* Section attributes */
        Elf64_Addr sh_addr;         /* Virtual address in memory */
        Elf64_Off sh_offset;        /* Offset in file */
        Elf64_Xword sh_size;        /* Size of section */
        Elf64_Word sh_link;         /* Link to other section */
        Elf64_Word sh_info;         /* Miscellaneous information */
        Elf64_Xword sh_addralign;   /* Address alignment boundary */
        Elf64_Xword sh_entsize;     /* Size of entries, if section has table */
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

    class ELF
    {
    private:
        ELF_Header header;
        ELF_Program_Header_Table program_header_table;
        ELF_Section_Header_Table section_header_table;
    public:
        ELF(std::string_view elf_file_path)
        {
            int elf_fd = -1;
            std::size_t size = 0;
            void* elf_file = nullptr;

            // Open the ELF file
            elf_fd = open(elf_file_path.data(), O_RDONLY);

            // Get ELF file size
            struct stat sb;
            fstat(elf_fd, &sb);
            size = sb.st_size;

            // Memory Map ELF file
            elf_file = mmap(
                nullptr,
                size,
                PROT_READ,
                MAP_PRIVATE,
                elf_fd,
                0
            );

            close(elf_fd);

            // Start reading
            
            // Read ELF Header
            header = ELF_Header(static_cast<std::byte*>(elf_file), size);

            // Read ELF Program Header Table
            //program_header_table = ELF_Program_Header_Table(static_cast<std::byte*>(elf_file));

            // Read ELF Section Header Table
            //section_header_table = ELF_Section_Header_Table(static_cast<std::byte*>(elf_file));

            munmap(elf_file, size);
        }

        ~ELF() = default;
    };
}