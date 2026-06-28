#pragma once

// C++ libs
#include <cstdint>
#include <array>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include <cstddef>

// C (linux) libs
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Custom libs
#include "elf_header.hpp"
#include "elf_program_header.hpp"
#include "elf_section_header.hpp"

namespace elf
{

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

    class ELF
    {
    private:
        // ELF_Header header;
        // ELF_Program_Header_Table program_header_table;
        // ELF_Section_Header_Table section_header_table;
    public:
        elf_header::ELF_Header header;
        elf_program_header::ELF_Program_Header_Table program_header_table;
        elf_section_header::ELF_Section_Header_Table section_header_table;

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
            header = elf_header::ELF_Header(static_cast<std::byte*>(elf_file), size);

            // Read ELF Program Header Table
            //program_header_table = ELF_Program_Header_Table(static_cast<std::byte*>(elf_file));

            // Read ELF Section Header Table
            //section_header_table = ELF_Section_Header_Table(static_cast<std::byte*>(elf_file));

            munmap(elf_file, size);
        }

        ~ELF() = default;
    };
}