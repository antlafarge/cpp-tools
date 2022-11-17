#pragma once

#include <string>

/// @brief UUID class
/// @note Use variant 1
class UUID
{
public:

    /// @brief UUID Format
    enum class Format : uint8_t
    {
        D = 0x0,
        N = 0x1,
        B = 0x2,
        P = 0x3,
        X = 0x4,
        URN = 0x5,
        Base64 = 0x10,
    };

    /// @brief Create new uninitialized UUID (set to 0)
    UUID() = default;

    /// @brief Create from c-string
    /// @param uuidStr Source c-string
    UUID(const char* uuidStr);

    /// @brief Create from string
    /// @param uuidStr Source string
    UUID(const std::string& uuidStr);

    /// @brief Create from 128 bits pointer
    /// @param srcPtr Source pointer
    UUID(const uint8_t* srcPtr, bool fromVariant2 = false);

    /// @brief Create from another
    /// @param other Source UUID
    UUID(const UUID& other) = default;

    /// UUID destructor
    ~UUID() = default;

    /// @brief UUID assignment
    /// @param right Another UUID
    /// @return Current UUID
    UUID& operator=(const UUID& right) = default;

    /// @brief UUID equality
    /// @param right Another UUID
    /// @return Equality result
    bool operator==(const UUID& right) const;

    /// @brief UUID non-equality
    /// @param right Another UUID
    /// @return Non-equality result
    bool operator!=(const UUID& right) const;

    /// @brief Format to string
    /// @param format Format to use
    /// @return Formated string
    std::string toString(Format format = Format::D) const;

    /// @brief Parse UUID from c-string
    /// @param uuidStr Source c-string
    /// @return Parsed UUID
    UUID& fromString(const char* uuidStr);

    /// @brief Parse UUID from string
    /// @param uuidStr Source string
    /// @return Parsed UUID
    UUID& fromString(const std::string& uuidStr);

    /// @brief Generate a new UUID (version 4, fully randomized)
    /// @return Generated UUID
    static UUID generate();

private:

    UUID& fromString(const char* uuidPtr, std::size_t uuidSize);

    uint64_t _part1 = 0ULL;
    uint64_t _part2 = 0ULL;
};
