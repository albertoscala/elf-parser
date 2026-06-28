#include <iostream>

#include "../include/elf.hpp"

int main()
{
    // Minimal test
    elf::ELF elf_minimal_test("tests/minimal/test_static");
    std::cout << elf_minimal_test.header << "\n";

    // Minimal test
    elf::ELF elf_obj_file("tests/obj_file/test_obj.o");
    std::cout << elf_obj_file.header << "\n";

    // Minimal test
    elf::ELF elf_shared_lib("tests/shared_lib/test_lib.so");
    std::cout << elf_shared_lib.header << "\n";

    // Minimal test
    elf::ELF elf_stripped_binary("tests/stripped_binary/test_stripped");
    std::cout << elf_stripped_binary.header << "\n";

    // Minimal test
    elf::ELF elf_with_debug("tests/with_debug/test_debug");
    std::cout << elf_with_debug.header << "\n";
}