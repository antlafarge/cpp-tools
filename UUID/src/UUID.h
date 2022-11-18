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

    /// @brief Create from 128 bits raw pointer
    /// @param srcPtr Source pointer
    /// @note Variant1 format : { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF }
    /// @note Variant2 format : { 0x33, 0x22, 0x11, 0x00, 0x55, 0x44, 0x77, 0x66, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
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

    /// @brief UUID assignment from c-string
    /// @param right Source c-string
    /// @return Current UUID
    UUID& operator=(const char* uuidStr);

    /// @brief UUID assignment from string
    /// @param right Source string
    /// @return Current UUID
    UUID& operator=(const std::string& uuidStr);

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

    /// @brief get UUID version
    /// @return UUID version
    /// @note Possible versions : 1, 2, 3, 4, 5
    uint8_t getVersion() const;

    /// @brief get UUID variant
    /// @return UUID variant
    /// @note Possible variants : 0, 1, 2, 3
    uint8_t getVariant() const;

    /// @brief set UUID version
    /// @param version UUID version
    /// @note Possible versions : 1, 2, 3, 4, 5
    void setVersion(uint8_t version);

    /// @brief set UUID variant
    /// @param variant UUID variant
    /// @note Possible variants : 0, 1, 2, 3
    void setVariant(uint8_t variant);

    /// @brief Generate a new UUID (variant 1; version 4 (fully randomized))
    /// @return Generated UUID
    static UUID generate();

private:

    UUID& fromString(const char* uuidPtr, std::size_t uuidSize);

    uint64_t _part1 = 0ULL;
    uint64_t _part2 = 0ULL;
};
