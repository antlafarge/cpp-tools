#include "UUID.h"

#include "Base64.helpers.h"

#include <cstring>
#include <stdexcept>
#include <time.h>

#if defined(_MSC_VER)
#   define UUID_SSCANF(...) sscanf_s(__VA_ARGS__)
#else
#   define UUID_SSCANF(...) sscanf(__VA_ARGS__)
#endif

#if _HAS_CXX17 == 1
#   define UUID_STRING_GET_DATA(str) str.data()
#else
#   define UUID_STRING_GET_DATA(str) const_cast<char*>(str.data())
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

UUID::UUID(const char* uuidStr)
{
    fromString(uuidStr);
}

UUID::UUID(const std::string& uuidStr)
{
    fromString(uuidStr);
}

UUID::UUID(const uint8_t* srcPtr, bool fromVariant2)
{
    uint8_t* dstPtr = reinterpret_cast<uint8_t*>(&_part1);
    if (!fromVariant2) [[likely]]
    {
        for (int i = 0; i < 16; i++)
        {
            dstPtr[i] = srcPtr[i];
        }
    }
    else [[unlikely]]
    {
        dstPtr[0] = srcPtr[3];
        dstPtr[1] = srcPtr[2];
        dstPtr[2] = srcPtr[1];
        dstPtr[3] = srcPtr[0];
        dstPtr[4] = srcPtr[5];
        dstPtr[5] = srcPtr[4];
        dstPtr[6] = srcPtr[7];
        dstPtr[7] = srcPtr[6];
        for (int i = 8; i < 16; i++)
        {
            dstPtr[i] = srcPtr[i];
        }
    }
}

bool UUID::operator==(const UUID& right) const
{
    return (_part1 == right._part1 && _part2 == right._part2);
}

bool UUID::operator!=(const UUID& right) const
{
    return (_part1 != right._part1 || _part2 != right._part2);
}

std::string UUID::toString(UUID::Format format) const
{
    if (format == UUID::Format::D) [[likely]]
    {
        const size_t size = 37;
        std::string output(size, 0);
        const uint8_t* srcPtr = reinterpret_cast<const uint8_t*>(&_part1);
        snprintf(UUID_STRING_GET_DATA(output), size, "%02hhx%02hhx%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx", srcPtr[0], srcPtr[1], srcPtr[2], srcPtr[3], srcPtr[4], srcPtr[5], srcPtr[6], srcPtr[7], srcPtr[8], srcPtr[9], srcPtr[10], srcPtr[11], srcPtr[12], srcPtr[13], srcPtr[14], srcPtr[15]);
        output.resize(size - 1);
        return output;
    }
    else if (format == UUID::Format::N) [[unlikely]]
    {
        const size_t size = 33;
        std::string output(size, 0);
        const uint8_t* srcPtr = reinterpret_cast<const uint8_t*>(&_part1);
        snprintf(UUID_STRING_GET_DATA(output), size, "%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx", srcPtr[0], srcPtr[1], srcPtr[2], srcPtr[3], srcPtr[4], srcPtr[5], srcPtr[6], srcPtr[7], srcPtr[8], srcPtr[9], srcPtr[10], srcPtr[11], srcPtr[12], srcPtr[13], srcPtr[14], srcPtr[15]);
        output.resize(size - 1);
        return output;
    }
    else if (format == UUID::Format::B) [[unlikely]]
    {
        const size_t size = 39;
        std::string output(size, 0);
        const uint8_t* srcPtr = reinterpret_cast<const uint8_t*>(&_part1);
        snprintf(UUID_STRING_GET_DATA(output), size, "{%02hhx%02hhx%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx}", srcPtr[0], srcPtr[1], srcPtr[2], srcPtr[3], srcPtr[4], srcPtr[5], srcPtr[6], srcPtr[7], srcPtr[8], srcPtr[9], srcPtr[10], srcPtr[11], srcPtr[12], srcPtr[13], srcPtr[14], srcPtr[15]);
        output.resize(size - 1);
        return output;
    }
    else if (format == UUID::Format::P) [[unlikely]]
    {
        const size_t size = 39;
        std::string output(size, 0);
        const uint8_t* srcPtr = reinterpret_cast<const uint8_t*>(&_part1);
        snprintf(UUID_STRING_GET_DATA(output), size, "(%02hhx%02hhx%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx)", srcPtr[0], srcPtr[1], srcPtr[2], srcPtr[3], srcPtr[4], srcPtr[5], srcPtr[6], srcPtr[7], srcPtr[8], srcPtr[9], srcPtr[10], srcPtr[11], srcPtr[12], srcPtr[13], srcPtr[14], srcPtr[15]);
        output.resize(size - 1);
        return output;
    }
    else if (format == UUID::Format::X) [[unlikely]]
    {
        const size_t size = 69;
        std::string output(size, 0);
        const uint8_t* srcPtr = reinterpret_cast<const uint8_t*>(&_part1);
        snprintf(UUID_STRING_GET_DATA(output), size, "{0x%02hhx%02hhx%02hhx%02hhx,0x%02hhx%02hhx,0x%02hhx%02hhx,{0x%02hhx,0x%02hhx,0x%02hhx,0x%02hhx,0x%02hhx,0x%02hhx,0x%02hhx,0x%02hhx}}", srcPtr[0], srcPtr[1], srcPtr[2], srcPtr[3], srcPtr[4], srcPtr[5], srcPtr[6], srcPtr[7], srcPtr[8], srcPtr[9], srcPtr[10], srcPtr[11], srcPtr[12], srcPtr[13], srcPtr[14], srcPtr[15]);
        output.resize(size - 1);
        return output;
    }
    else if (format == UUID::Format::URN) [[unlikely]]
    {
        return "urn:uuid:" + toString(UUID::Format::D);
    }
    else if (format == UUID::Format::Base64) [[unlikely]]
    {
        return Base64::encode(&_part1, sizeof(_part1) + sizeof(_part2));
    }

    throw std::runtime_error("Bad UUID format");
}

UUID& UUID::fromString(const char* uuidPtr, std::size_t uuidSize)
{
    uint8_t* dstPtr = reinterpret_cast<uint8_t*>(&_part1);

    int scanRes = 0;

    if (uuidSize == 36) [[likely]]
    {
        scanRes = UUID_SSCANF(uuidPtr, "%02hhx%02hhx%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx", dstPtr, dstPtr + 1, dstPtr + 2, dstPtr + 3, dstPtr + 4, dstPtr + 5, dstPtr + 6, dstPtr + 7, dstPtr + 8, dstPtr + 9, dstPtr + 10, dstPtr + 11, dstPtr + 12, dstPtr + 13, dstPtr + 14, dstPtr + 15);
    }
    else if (uuidSize == 32) [[unlikely]]
    {
        scanRes = UUID_SSCANF(uuidPtr, "%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx", dstPtr, dstPtr + 1, dstPtr + 2, dstPtr + 3, dstPtr + 4, dstPtr + 5, dstPtr + 6, dstPtr + 7, dstPtr + 8, dstPtr + 9, dstPtr + 10, dstPtr + 11, dstPtr + 12, dstPtr + 13, dstPtr + 14, dstPtr + 15);
    }
    else if (uuidSize == 38) [[unlikely]]
    {
        if (uuidPtr[0] == '{')
        {
            scanRes = UUID_SSCANF(uuidPtr, "{%02hhx%02hhx%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx}", dstPtr, dstPtr + 1, dstPtr + 2, dstPtr + 3, dstPtr + 4, dstPtr + 5, dstPtr + 6, dstPtr + 7, dstPtr + 8, dstPtr + 9, dstPtr + 10, dstPtr + 11, dstPtr + 12, dstPtr + 13, dstPtr + 14, dstPtr + 15);
        }
        else
        {
            scanRes = UUID_SSCANF(uuidPtr, "(%02hhx%02hhx%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx)", dstPtr, dstPtr + 1, dstPtr + 2, dstPtr + 3, dstPtr + 4, dstPtr + 5, dstPtr + 6, dstPtr + 7, dstPtr + 8, dstPtr + 9, dstPtr + 10, dstPtr + 11, dstPtr + 12, dstPtr + 13, dstPtr + 14, dstPtr + 15);
        }
    }
    else if (uuidSize == 68) [[unlikely]]
    {
        scanRes = UUID_SSCANF(uuidPtr, "{0x%02hhx%02hhx%02hhx%02hhx,0x%02hhx%02hhx,0x%02hhx%02hhx,{0x%02hhx,0x%02hhx,0x%02hhx,0x%02hhx,0x%02hhx,0x%02hhx,0x%02hhx,0x%02hhx}}", dstPtr, dstPtr + 1, dstPtr + 2, dstPtr + 3, dstPtr + 4, dstPtr + 5, dstPtr + 6, dstPtr + 7, dstPtr + 8, dstPtr + 9, dstPtr + 10, dstPtr + 11, dstPtr + 12, dstPtr + 13, dstPtr + 14, dstPtr + 15);
    }
    else if (uuidSize == 45) [[unlikely]]
    {
        return fromString(uuidPtr + 9, 36);
    }
    else if (uuidSize == 24) [[unlikely]]
    {
        uint8_t dstPtr[16];
        uint64_t dstSize = 16;
        if (Base64::succeed(Base64::decode(uuidPtr, uuidSize, dstPtr, dstSize))) [[likely]]
        {
            const uint64_t* srcPtr2 = reinterpret_cast<const uint64_t*>(dstPtr);
            _part1 = srcPtr2[0];
            _part2 = srcPtr2[1];
            scanRes = 16;
        }
    }

        if (scanRes != 16) [[unlikely]]
        {
            throw std::logic_error("Unvalid UUID format");
        }

    return *this;
}

UUID& UUID::fromString(const char* uuidStr)
{
    return fromString(uuidStr, strlen(uuidStr));
}

UUID& UUID::fromString(const std::string& uuidStr)
{
    return fromString(uuidStr.c_str(), uuidStr.size());
}

UUID UUID::generate()
{
    static int randBits = 0;
    if (randBits == 0) [[unlikely]]
    {
        srand((uint32_t)time(NULL));
        for (int r = RAND_MAX; r > 0; r >>= 1) randBits++;
    }

    UUID uuid;
    for (int i = 0; i < 64; i += randBits)
    {
        uuid._part1 = ((uuid._part1 << randBits) | rand());
        uuid._part2 = ((uuid._part2 << randBits) | rand());
    }
    return uuid;
}

#pragma GCC diagnostic pop
