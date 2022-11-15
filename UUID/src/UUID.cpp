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

UUID::UUID(const std::string& uuidStr)
{
    (*this) = toUUID(uuidStr);
}

UUID::UUID(const void* srcPtr)
{
    const uint64_t* srcPtr2 = reinterpret_cast<const uint64_t*>(srcPtr);
    _part1 = srcPtr2[0];
    _part2 = srcPtr2[1];
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
    return toString(*this, format);
}

std::string UUID::toString(const UUID& uuid, UUID::Format format)
{
    if (format == UUID::Format::D) [[likely]]
    {
        const size_t size = 37;
        std::string output(size, 0);
        snprintf(UUID_STRING_GET_DATA(output), size, "%08x-%04hx-%04hx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx", (uint32_t)(uuid._part1 >> 32), (uint16_t)(uuid._part1 >> 16), (uint16_t)(uuid._part1), (uint8_t)(uuid._part2 >> 56), (uint8_t)(uuid._part2 >> 48), (uint8_t)(uuid._part2 >> 40), (uint8_t)(uuid._part2 >> 32), (uint8_t)(uuid._part2 >> 24), (uint8_t)(uuid._part2 >> 16), (uint8_t)(uuid._part2 >> 8), (uint8_t)(uuid._part2));
        output.resize(size - 1);
        return output;
    }
    else if (format == UUID::Format::N) [[unlikely]]
    {
        const size_t size = 33;
        std::string output(size, 0);
        snprintf(UUID_STRING_GET_DATA(output), size, "%08x%04hx%04hx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx", (uint32_t)(uuid._part1 >> 32), (uint16_t)(uuid._part1 >> 16), (uint16_t)(uuid._part1), (uint8_t)(uuid._part2 >> 56), (uint8_t)(uuid._part2 >> 48), (uint8_t)(uuid._part2 >> 40), (uint8_t)(uuid._part2 >> 32), (uint8_t)(uuid._part2 >> 24), (uint8_t)(uuid._part2 >> 16), (uint8_t)(uuid._part2 >> 8), (uint8_t)(uuid._part2));
        output.resize(size - 1);
        return output;
    }
    else if (format == UUID::Format::B) [[unlikely]]
    {
        const size_t size = 39;
        std::string output(size, 0);
        snprintf(UUID_STRING_GET_DATA(output), size, "{%08x-%04hx-%04hx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx}", (uint32_t)(uuid._part1 >> 32), (uint16_t)(uuid._part1 >> 16), (uint16_t)(uuid._part1), (uint8_t)(uuid._part2 >> 56), (uint8_t)(uuid._part2 >> 48), (uint8_t)(uuid._part2 >> 40), (uint8_t)(uuid._part2 >> 32), (uint8_t)(uuid._part2 >> 24), (uint8_t)(uuid._part2 >> 16), (uint8_t)(uuid._part2 >> 8), (uint8_t)(uuid._part2));
        output.resize(size - 1);
        return output;
    }
    else if (format == UUID::Format::P) [[unlikely]]
    {
        const size_t size = 39;
        std::string output(size, 0);
        snprintf(UUID_STRING_GET_DATA(output), size, "(%08x-%04hx-%04hx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx)", (uint32_t)(uuid._part1 >> 32), (uint16_t)(uuid._part1 >> 16), (uint16_t)(uuid._part1), (uint8_t)(uuid._part2 >> 56), (uint8_t)(uuid._part2 >> 48), (uint8_t)(uuid._part2 >> 40), (uint8_t)(uuid._part2 >> 32), (uint8_t)(uuid._part2 >> 24), (uint8_t)(uuid._part2 >> 16), (uint8_t)(uuid._part2 >> 8), (uint8_t)(uuid._part2));
        output.resize(size - 1);
        return output;
    }
    else if (format == UUID::Format::X) [[unlikely]]
    {
        const size_t size = 69;
        std::string output(size, 0);
        snprintf(UUID_STRING_GET_DATA(output), size, "{0x%08x,0x%04hx,0x%04hx,{0x%02hhx,0x%02hhx,0x%02hhx,0x%02hhx,0x%02hhx,0x%02hhx,0x%02hhx,0x%02hhx}}", (uint32_t)(uuid._part1 >> 32), (uint16_t)(uuid._part1 >> 16), (uint16_t)(uuid._part1), (uint8_t)(uuid._part2 >> 56), (uint8_t)(uuid._part2 >> 48), (uint8_t)(uuid._part2 >> 40), (uint8_t)(uuid._part2 >> 32), (uint8_t)(uuid._part2 >> 24), (uint8_t)(uuid._part2 >> 16), (uint8_t)(uuid._part2 >> 8), (uint8_t)(uuid._part2));
        output.resize(size - 1);
        return output;
    }
    else if (format == UUID::Format::URN) [[unlikely]]
    {
        return "urn:uuid:" + toString(uuid, UUID::Format::D);
    }
    else if (format == UUID::Format::Base64) [[unlikely]]
    {
        return Base64::encode(&uuid._part1, sizeof(uuid._part1) + sizeof(uuid._part2));
    }

    throw std::runtime_error("Bad UUID format");
}

UUID UUID::toUUID(const std::string& uuidStr)
{
    UUID uuid;

    uint32_t* part1 = ((uint32_t*)(&uuid._part1)) + 1;
    uint16_t* part2 = ((uint16_t*)(&uuid._part1)) + 1;
    uint16_t* part3 = ((uint16_t*)(&uuid._part1));
    uint8_t* part4 = ((uint8_t*)(&uuid._part2)) + 7;
    uint8_t* part5 = ((uint8_t*)(&uuid._part2)) + 6;
    uint8_t* part6 = ((uint8_t*)(&uuid._part2)) + 5;
    uint8_t* part7 = ((uint8_t*)(&uuid._part2)) + 4;
    uint8_t* part8 = ((uint8_t*)(&uuid._part2)) + 3;
    uint8_t* part9 = ((uint8_t*)(&uuid._part2)) + 2;
    uint8_t* part10 = ((uint8_t*)(&uuid._part2)) + 1;
    uint8_t* part11 = ((uint8_t*)(&uuid._part2));

    const char* data = uuidStr.c_str();

    int scanRes = 0;

    size_t uuidSize = uuidStr.size();
    if (uuidSize == 36) [[likely]]
    {
        scanRes = UUID_SSCANF(data, "%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX", part1, part2, part3, part4, part5, part6, part7, part8, part9, part10, part11);
    }
    else if (uuidSize == 32) [[unlikely]]
    {
        scanRes = UUID_SSCANF(data, "%08X%04hX%04hX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX", part1, part2, part3, part4, part5, part6, part7, part8, part9, part10, part11);
    }
    else if (uuidSize == 38) [[unlikely]]
    {
        if (data[0] == '{')
        {
            scanRes = UUID_SSCANF(data, "{%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}", part1, part2, part3, part4, part5, part6, part7, part8, part9, part10, part11);
        }
        else
        {
            scanRes = UUID_SSCANF(data, "(%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX)", part1, part2, part3, part4, part5, part6, part7, part8, part9, part10, part11);
        }
    }
    else if (uuidSize == 68) [[unlikely]]
    {
        scanRes = UUID_SSCANF(data, "{0x%08X,0x%04hX,0x%04hX,{0x%02hhX,0x%02hhX,0x%02hhX,0x%02hhX,0x%02hhX,0x%02hhX,0x%02hhX,0x%02hhX}}", part1, part2, part3, part4, part5, part6, part7, part8, part9, part10, part11);
    }
    else if (uuidSize == 45) [[unlikely]]
    {
        return toUUID(uuidStr.substr(9, 36));
    }
    else if (uuidSize == 24) [[unlikely]]
    {
        std::vector<uint8_t> res = Base64::decode(uuidStr);
        const uint64_t* srcPtr2 = reinterpret_cast<const uint64_t*>(res.data());
        uuid._part1 = srcPtr2[0];
        uuid._part2 = srcPtr2[1];
        scanRes = 11;
    }

    if (scanRes != 11) [[unlikely]]
    {
        throw std::logic_error("Unvalid UUID format");
    }

    return uuid;
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
