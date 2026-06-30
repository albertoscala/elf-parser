# elf-parser

A header-only C++ ELF64 binary parser built around `mmap`, zero-copy byte cursors, and the official ELF-64 object file format spec.

## Description

`elf-parser` is a work-in-progress, header-only C++ library for parsing 64-bit ELF (Executable and Linkable Format) binaries on Linux. The file is memory-mapped with `mmap` and read directly from the mapped bytes — no intermediate buffers or copies beyond the small fixed-size fields themselves. Every multi-byte ELF primitive (addresses, offsets, half-words, words, etc.) is modeled as its own type that knows how to read itself off a cursor and reinterpret its raw bytes as the correct integer width and signedness, which keeps the parsing code declarative: building a struct's fields in file order is enough to fully decode it.

The implementation follows the **ELF-64 Object File Format** specification, as published by uClibc:
[https://uclibc.org/docs/elf-64-gen.pdf](https://uclibc.org/docs/elf-64-gen.pdf)

Currently implemented:
- ELF primitive types (`elf_types.hpp`)
- ELF Header parsing and pretty-printing (`elf_header.hpp`)

Not yet implemented (stubbed out):
- ELF Program Header Table (`elf_program_header.hpp`)
- ELF Section Header Table (`elf_section_header.hpp`)

---

## ELF Types (`elf_types.hpp`)

Defines the fixed-width primitive types used throughout the ELF-64 spec, each as a small wrapper class around a `std::array<std::byte, N>`:

| Type | Size | Meaning |
|---|---|---|
| `Elf64_Addr` | 8 bytes | Unsigned program address |
| `Elf64_Off` | 8 bytes | Unsigned file offset |
| `Elf64_Half` | 2 bytes | Unsigned medium integer |
| `Elf64_Word` | 4 bytes | Unsigned integer |
| `Elf64_Sword` | 4 bytes | Signed integer |
| `Elf64_Xword` | 8 bytes | Unsigned long integer |
| `Elf64_Sxword` | 8 bytes | Signed long integer |

Each type has a constructor that takes a `const std::byte*&` cursor: it copies the appropriate number of bytes out of the buffer **and** advances the cursor past them as a side effect. This means decoding a struct is just a sequence of `field = Elf64_X(cursor);` assignments in file order — the cursor walks itself forward automatically. A `.value()` accessor reconstructs the little-endian integer on demand.

---

## ELF Header (`elf_header.hpp`)

Implements `ELF_Header`, covering the full 64-byte ELF64 header as defined in the spec, including:

- The 16-byte `e_ident` array (magic number, file class, data encoding, ELF version, OS/ABI, ABI version), indexed via the `ELF_Identification` enum
- `e_type`, `e_machine`, `e_version`, `e_entry`, `e_phoff`, `e_shoff`, `e_flags`, `e_ehsize`, `e_phentsize`, `e_phnum`, `e_shentsize`, `e_shnum`, `e_shstrndx`

Supporting enums map raw header values to their named spec constants: `ELF_Class`, `ELF_Data_Encoding`, `ELF_OSABI_Identifier`, and `ELF_Object_File_Type`. Private string-conversion helpers turn these into human-readable labels, and an `operator<<` overload prints the header in a `readelf`-style format.

`ELF_Header` is constructed directly from a pointer into the mapped file, decoding every field in spec order off a single advancing cursor.

### Usage

```cpp
#include "elf.hpp"

int main()
{
    elf::ELF parsed_elf("/bin/ls");

    std::cout << parsed_elf.header;

    return 0;
}
```

Example output:

```
ELF Header:
  Magic:        7f 45 4c 46 
  Class:        ELFCLASS64
  Data:         ELFDATA2LSB
  Version:      1
  OS/ABI:       ELFOSABI_SYSV
  ABI Version:  0
  Type:         ET_DYN
  Machine:      0x3e
  Version:      1
  Entry point:  0x6ab0
  PHT offset:   64
  SHT offset:   142536
  Flags:        0x0
  Header size:  64 bytes
  PHT entry:    56 bytes
  PHT count:    13
  SHT entry:    64 bytes
  SHT count:    30
  SHSTRTAB idx: 29
```

---

## ELF Program Header (`elf_program_header.hpp`)

**TBD** — `ELF_Program_Header` and `ELF_Program_Header_Table` are defined but parsing is not yet implemented.

---

## ELF Section Header (`elf_section_header.hpp`)

**TBD** — `ELF_Section_Header` and `ELF_Section_Header_Table` are defined but parsing is not yet implemented.

---

## Tests

Parsing correctness is validated against a small set of real, purpose-built ELF binaries rather than synthetic byte arrays, so that each test exercises the parser against the actual layout quirks a given binary category produces.

```
tests/
├── minimal
│   └── test_static
├── obj_file
│   └── test_obj.o
├── shared_lib
│   └── test_lib.so
├── stripped_binary
│   └── test_stripped
└── with_debug
    └── test_debug
```

- **`minimal/test_static`** — A minimal, statically-linked executable (`ET_EXEC`/`ET_DYN` with no dynamic dependencies). Acts as the baseline sanity check: confirms `e_ident`, magic bytes, class/data/version/OS-ABI decoding, and the core header fields (`e_entry`, `e_phoff`, `e_shoff`, counts/sizes) are read correctly with the simplest possible segment/section layout.

- **`obj_file/test_obj.o`** — A relocatable object file (`ET_REL`). Unlike executables, relocatable objects typically have `e_entry == 0`, no program headers (`e_phnum == 0`), and rely entirely on the section header table plus relocation sections (`SHT_REL`/`SHT_RELA`). This validates that the parser correctly handles a header with an empty/absent program header table.

- **`shared_lib/test_lib.so`** — A shared object (`ET_DYN`). Exercises dynamic-linking-related sections (`.dynamic`, `.dynsym`, `.hash`/`.gnu.hash`) and PT_DYNAMIC/PT_LOAD-style program header entries once the program header table is implemented; for now it validates header-level detection of `ET_DYN` and its associated entry point conventions (load-address-relative).

- **`stripped_binary/test_stripped`** — An executable with symbol table and debug sections removed via `strip`. Confirms the parser doesn't assume the presence of `.symtab`/`.debug_*` sections and degrades gracefully — section count and `e_shstrndx` will differ from an unstripped build of the same source.

- **`with_debug/test_debug`** — The same source built with debug info retained (e.g. `-g`, DWARF sections present: `.debug_info`, `.debug_line`, `.debug_str`, etc.). Used as the contrasting case against the stripped binary, and as the future fixture for any debug-section-aware parsing.

Each fixture currently has header-level coverage planned (magic, class, data encoding, version, OS/ABI, type, entry point, offsets/counts/sizes). Program header and section header table coverage will be added per-fixture as those parsers are implemented.