#pragma once

#include <array>

namespace elf_types
{
    using Elf64_Addr    = std::array<std::byte, 8>;     // Unsigned program address
    using Elf64_Off     = std::array<std::byte, 8>;     // Unsigned file offset
    using Elf64_Half    = std::array<std::byte, 2>;     // Unsigned medium integer
    using Elf64_Word    = std::array<std::byte, 4>;     // Unsigned integer
    using Elf64_Sword   = std::array<std::byte, 4>;     // Signed integer
    using Elf64_Xword   = std::array<std::byte, 8>;     // Unsigned long integer
    using Elf64_Sxword  = std::array<std::byte, 8>;     // Signed long integer
}