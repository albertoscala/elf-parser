#include <iostream>

#include "../include/elf.hpp"

int main()
{
    elf::ELF elf("tests/minimal/test_static");

    std::cout << elf.header << "\n";
}