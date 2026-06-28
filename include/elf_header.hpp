#pragma once

#include <cstdint>
#include <string_view>
#include <cstddef>
#include <iomanip>

#include "elf_types.hpp"

namespace elf_header
{
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
        ELFOSABI_GNU = 3,           // GNU/Linux
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

    namespace
    {
        inline std::string_view elf_class_to_string(ELF_Class elf_class)
        {
            switch (elf_class)
            {
                case ELF_Class::ELFCLASS32: return "ELFCLASS32";
                case ELF_Class::ELFCLASS64: return "ELFCLASS64";
                default: return "Unknown";
            }
        }

        inline std::string_view elf_data_encoding_to_string(ELF_Data_Encoding elf_data_encoding)
        {
            switch (elf_data_encoding)
            {
                case ELF_Data_Encoding::ELFDATA2LSB: return "ELFDATA2LSB";
                case ELF_Data_Encoding::ELFDATA2MSB: return "ELFDATA2MSB";
                default: return "Unknown";
            }
        }

        inline std::string_view elf_osabi_identifier_to_string(ELF_OSABI_Identifier elf_osabi_idenfier)
        {
            switch (elf_osabi_idenfier)
            {
                case ELF_OSABI_Identifier::ELFOSABI_SYSV: return "ELFOSABI_SYSV";
                case ELF_OSABI_Identifier::ELFOSABI_HPUX: return "ELFOSABI_HPUX";
                case ELF_OSABI_Identifier::ELFOSABI_GNU: return "ELFOSABI_GNU";
                case ELF_OSABI_Identifier::ELFOSABI_STANDALONE: return "ELFOSABI_STANDALONE"; 
                default: return "Unknown";
            }
        }

        inline std::string_view elf_object_file_identifier_to_string(ELF_Object_File_Type elf_object_file_type)
        {
            switch (elf_object_file_type)
            {
                case ELF_Object_File_Type::ET_NONE: return "ET_NONE";
                case ELF_Object_File_Type::ET_REL: return "ET_REL";
                case ELF_Object_File_Type::ET_EXEC: return "ET_EXEC";
                case ELF_Object_File_Type::ET_DYN: return "ET_DYN";
                case ELF_Object_File_Type::ET_CORE: return "ET_CORE";
                case ELF_Object_File_Type::ET_LOOS: return "ET_LOOS";
                case ELF_Object_File_Type::ET_HIOS: return "ET_HIOS";
                case ELF_Object_File_Type::ET_LOPROC: return "ET_LOPROC";
                case ELF_Object_File_Type::ET_HIPROC: return "ET_HIPROC";
                default: return "Unknown";
            }
        }
    }

    class ELF_Header
    {
        std::array<std::byte, 16> e_ident;   /* ELF identification */
        elf_types::Elf64_Half e_type;                      /* Object file type */
        elf_types::Elf64_Half e_machine;                   /* Machine type */
        elf_types::Elf64_Word e_version;                   /* Object file version */
        elf_types::Elf64_Addr e_entry;                     /* Entry point address */
        elf_types::Elf64_Off e_phoff;                      /* Program header offset */
        elf_types::Elf64_Off e_shoff;                      /* Section header offset */
        elf_types::Elf64_Word e_flags;                     /* Processor-specific flags */
        elf_types::Elf64_Half e_ehsize;                    /* ELF header size */
        elf_types::Elf64_Half e_phentsize;                 /* Size of program header entry */
        elf_types::Elf64_Half e_phnum;                     /* Number of program header entries */
        elf_types::Elf64_Half e_shentsize;                 /* Size of section header entry */
        elf_types::Elf64_Half e_shnum;                     /* Number of section header entries */
        elf_types::Elf64_Half e_shstrndx;                  /* Section name string table index */
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
            // helper to index e_ident cleanly
            auto read_ident = [&](ELF_Identification idx) -> uint8_t
            {
                return std::to_integer<uint8_t>(
                    elf_header.e_ident[static_cast<std::size_t>(idx)]
                );
            };

            // helper to read a multi-byte field as uint16_t (little-endian)
            auto read_half = [](const elf_types::Elf64_Half& field) -> uint16_t
            {
                return static_cast<uint16_t>(
                    std::to_integer<uint16_t>(field[0]) |
                    std::to_integer<uint16_t>(field[1]) << 8
                );
            };

            auto read_word = [](const elf_types::Elf64_Word& field) -> uint32_t
            {
                return static_cast<uint32_t>(
                    std::to_integer<uint32_t>(field[0])        |
                    std::to_integer<uint32_t>(field[1]) << 8   |
                    std::to_integer<uint32_t>(field[2]) << 16  |
                    std::to_integer<uint32_t>(field[3]) << 24
                );
            };

            // helper to read a multi-byte field as uint64_t (little-endian)
            auto read_xword = [](const elf_types::Elf64_Xword& field) -> uint64_t
            {
                uint64_t value = 0;
                for (std::size_t i = 0; i < field.size(); ++i)
                    value |= std::to_integer<uint64_t>(field[i]) << (i * 8);
                return value;
            };

            os << "ELF Header:\n";

            // Magic
            os << "  Magic:        ";
            os << std::hex << std::setw(2) << std::setfill('0') 
                << static_cast<int>(
                    elf_header.e_ident[static_cast<std::uint8_t>(ELF_Identification::EI_MAG0)]
                ) << " ";
            os  << static_cast<std::uint8_t>(
                    elf_header.e_ident[static_cast<std::uint8_t>(ELF_Identification::EI_MAG1)]
                ) << " ";
            os << static_cast<std::uint8_t>(
                    elf_header.e_ident[static_cast<std::uint8_t>(ELF_Identification::EI_MAG2)]
                ) << " ";
            os << static_cast<std::uint8_t>(
                    elf_header.e_ident[static_cast<std::uint8_t>(ELF_Identification::EI_MAG3)]
                ) << " ";
            os << "\n";

            // e_ident fields
            os << "  Class:        " << elf_class_to_string(
                static_cast<ELF_Class>(read_ident(ELF_Identification::EI_CLASS))) << "\n";

            os << "  Data:         " << elf_data_encoding_to_string(
                static_cast<ELF_Data_Encoding>(read_ident(ELF_Identification::EI_DATA))) << "\n";

            os << "  Version:      " << std::dec 
            << static_cast<int>(read_ident(ELF_Identification::EI_VERSION)) << "\n";

            os << "  OS/ABI:       " << elf_osabi_identifier_to_string(
                static_cast<ELF_OSABI_Identifier>(read_ident(ELF_Identification::EI_OSABI))) << "\n";

            os << "  ABI Version:  " << std::dec 
            << static_cast<int>(read_ident(ELF_Identification::EI_ABIVERSION)) << "\n";

            // header fields
            os << "  Type:         " << elf_object_file_identifier_to_string(
                static_cast<ELF_Object_File_Type>(read_half(elf_header.e_type))) << "\n";

            os << "  Machine:      " << std::hex << "0x" 
            << read_half(elf_header.e_machine) << "\n";

            os << "  Version:      " << std::dec 
            << read_word(elf_header.e_version) << "\n";

            os << "  Entry point:  " << std::hex << "0x" 
            << read_xword(elf_header.e_entry) << "\n";

            os << "  PHT offset:   " << std::dec 
            << read_xword(elf_header.e_phoff) << "\n";

            os << "  SHT offset:   " << std::dec 
            << read_xword(elf_header.e_shoff) << "\n";

            os << "  Flags:        " << std::hex << "0x" 
            << read_word(elf_header.e_flags) << "\n";

            os << "  Header size:  " << std::dec 
            << read_half(elf_header.e_ehsize) << " bytes\n";

            os << "  PHT entry:    " << std::dec 
            << read_half(elf_header.e_phentsize) << " bytes\n";

            os << "  PHT count:    " << std::dec 
            << read_half(elf_header.e_phnum) << "\n";

            os << "  SHT entry:    " << std::dec 
            << read_half(elf_header.e_shentsize) << " bytes\n";

            os << "  SHT count:    " << std::dec 
            << read_half(elf_header.e_shnum) << "\n";

            os << "  SHSTRTAB idx: " << std::dec 
            << read_half(elf_header.e_shstrndx) << "\n";

            return os;
        }

    };
}