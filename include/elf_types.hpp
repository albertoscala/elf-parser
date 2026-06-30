#pragma once

#include <array>

namespace elf_types
{
    // Unsigned program address
    class Elf64_Addr
    {
    private:
        std::array<std::byte, 8> data;
    public:
        Elf64_Addr() = default;
        
        Elf64_Addr(const std::byte*& cursor)
        {
            std::copy(cursor, cursor + data.size(), data.data());
            cursor += data.size();
        }
        
        inline std::uint64_t value() const
        {
            std::uint64_t value{0};
            for (std::size_t i = 0; i < data.size(); ++i)
                value |= std::to_integer<std::uint64_t>(data[i]) << (i * 8);
            return value;
        }

        ~Elf64_Addr() = default;
    };

    // Unsigned file offset
    class Elf64_Off
    {
    private:
        std::array<std::byte, 8> data;
    public:
        Elf64_Off() = default;
        
        Elf64_Off(const std::byte*& cursor)
        {
            std::copy(cursor, cursor + data.size(), data.data());
            cursor += data.size();
        }
        
        inline std::uint64_t value() const
        {
            std::uint64_t value{0};
            for (std::size_t i = 0; i < data.size(); ++i)
                value |= std::to_integer<std::uint64_t>(data[i]) << (i * 8);
            return value;
        }

        ~Elf64_Off() = default;
    };

    // Unsigned medium integer
    class Elf64_Half
    {
    private:
        std::array<std::byte, 2> data;
    public:
        Elf64_Half() = default;
        
        Elf64_Half(const std::byte*& cursor)
        {
            std::copy(cursor, cursor + data.size(), data.data());
            cursor += data.size();
        }

        inline std::uint16_t value() const
        {
            std::uint16_t value{0};
            for (std::size_t i = 0; i < data.size(); ++i)
                value |= std::to_integer<std::uint16_t>(data[i]) << (i * 8);
            return value;
        }
        
        ~Elf64_Half() = default;
    };

    // Unsigned integer
    class Elf64_Word
    {
    private:
        std::array<std::byte, 4> data;
    public:
        Elf64_Word() = default;
        
        Elf64_Word(const std::byte*& cursor)
        {
            std::copy(cursor, cursor + data.size(), data.data());
            cursor += data.size();
        }

        inline std::uint32_t value() const
        {
            std::uint32_t value{0};
            for (std::size_t i = 0; i < data.size(); ++i)
                value |= std::to_integer<std::uint32_t>(data[i]) << (i * 8);
            return value;
        }
        
        ~Elf64_Word() = default;
    };

    // Signed integer
    class Elf64_Sword
    {
    private:
        std::array<std::byte, 4> data;
    public:
        Elf64_Sword() = default;
        
        Elf64_Sword(const std::byte*& cursor)
        {
            std::copy(cursor, cursor + data.size(), data.data());
            cursor += data.size();
        }

        inline std::int32_t value() const
        {
            std::int32_t value{0};
            for (std::size_t i = 0; i < data.size(); ++i)
                value |= std::to_integer<std::int32_t>(data[i]) << (i * 8);
            return value;
        }

        ~Elf64_Sword() = default;
    };

    // Unsigned long integer
    class Elf64_Xword
    {
    private:
        std::array<std::byte, 8> data;
    public:
        Elf64_Xword() = default;
        
        Elf64_Xword(const std::byte*& cursor)
        {
            std::copy(cursor, cursor + data.size(), data.data());
            cursor += data.size();
        }

        inline std::uint64_t value() const
        {
            std::uint64_t value{0};
            for (std::size_t i = 0; i < data.size(); ++i)
                value |= std::to_integer<std::uint64_t>(data[i]) << (i * 8);
            return value;
        }

        ~Elf64_Xword() = default;
    };

    // Signed long integer
    class Elf64_Sxword
    {
    private:
        std::array<std::byte, 8> data;
    public:
        Elf64_Sxword() = default;
        
        Elf64_Sxword(const std::byte*& cursor)
        {
            std::copy(cursor, cursor + data.size(), data.data());
            cursor += data.size();
        }

        inline std::int64_t value() const
        {
            std::int64_t value{0};
            for (std::size_t i = 0; i < data.size(); ++i)
                value |= std::to_integer<std::int64_t>(data[i]) << (i * 8);
            return value;
        }

        ~Elf64_Sxword() = default;


    };

}