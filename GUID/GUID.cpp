#include "GUID.h"

#include "../Base64/src/Base64.helpers.h"

#include <cstring>
#include <stdexcept>

GUID::GUID(const std::string& guidStr)
{
    (*this) = toGUID(guidStr);
}

bool GUID::operator==(const GUID& right)
{
    return (_g1 == right._g1 && _g2 == right._g2);
}

bool GUID::operator!=(const GUID& right)
{
    return (_g1 != right._g1 || _g2 != right._g2);
}

std::string GUID::toString(GuidFormat format)
{
    return toString(*this, format);
}

std::string GUID::toString(const GUID& guid, GuidFormat format)
{
    if (format == GuidFormat::Base64) [[likely]]
    {
        return Base64::encode(&guid._g1, sizeof(guid._g1) + sizeof(guid._g2));
    }
    else if (format == GuidFormat::N) [[unlikely]]
    {
        char output[33] = {};
        snprintf(output, 33, "%08X%04hX%04hX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX", (uint32_t)(guid._g1 >> 32), (uint16_t)(guid._g1 >> 16), (uint16_t)(guid._g1), (uint8_t)(guid._g2 >> 56), (uint8_t)(guid._g2 >> 48), (uint8_t)(guid._g2 >> 40), (uint8_t)(guid._g2 >> 32), (uint8_t)(guid._g2 >> 24), (uint8_t)(guid._g2 >> 16), (uint8_t)(guid._g2 >> 8), (uint8_t)(guid._g2));
        return std::string(output, 32);
    }
    else if (format == GuidFormat::D) [[unlikely]]
    {
        char output[37] = {};
        snprintf(output, 37, "%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX", (uint32_t)(guid._g1 >> 32), (uint16_t)(guid._g1 >> 16), (uint16_t)(guid._g1), (uint8_t)(guid._g2 >> 56), (uint8_t)(guid._g2 >> 48), (uint8_t)(guid._g2 >> 40), (uint8_t)(guid._g2 >> 32), (uint8_t)(guid._g2 >> 24), (uint8_t)(guid._g2 >> 16), (uint8_t)(guid._g2 >> 8), (uint8_t)(guid._g2));
        return std::string(output, 36);
    }
    else if (format == GuidFormat::B) [[unlikely]]
    {
        char output[39] = {};
        snprintf(output, 39, "{%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}", (uint32_t)(guid._g1 >> 32), (uint16_t)(guid._g1 >> 16), (uint16_t)(guid._g1), (uint8_t)(guid._g2 >> 56), (uint8_t)(guid._g2 >> 48), (uint8_t)(guid._g2 >> 40), (uint8_t)(guid._g2 >> 32), (uint8_t)(guid._g2 >> 24), (uint8_t)(guid._g2 >> 16), (uint8_t)(guid._g2 >> 8), (uint8_t)(guid._g2));
        return std::string(output, 38);
    }
    else if (format == GuidFormat::P) [[unlikely]]
    {
        char output[39] = {};
        snprintf(output, 39, "(%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX)", (uint32_t)(guid._g1 >> 32), (uint16_t)(guid._g1 >> 16), (uint16_t)(guid._g1), (uint8_t)(guid._g2 >> 56), (uint8_t)(guid._g2 >> 48), (uint8_t)(guid._g2 >> 40), (uint8_t)(guid._g2 >> 32), (uint8_t)(guid._g2 >> 24), (uint8_t)(guid._g2 >> 16), (uint8_t)(guid._g2 >> 8), (uint8_t)(guid._g2));
        return std::string(output, 38);
    }
    else if (format == GuidFormat::X) [[unlikely]]
    {
        char output[69] = {};
        snprintf(output, 69, "{0x%08X,0x%04hX,0x%04hX,{0x%02hhX,0x%02hhX,0x%02hhX,0x%02hhX,0x%02hhX,0x%02hhX,0x%02hhX,0x%02hhX}}", (uint32_t)(guid._g1 >> 32), (uint16_t)(guid._g1 >> 16), (uint16_t)(guid._g1), (uint8_t)(guid._g2 >> 56), (uint8_t)(guid._g2 >> 48), (uint8_t)(guid._g2 >> 40), (uint8_t)(guid._g2 >> 32), (uint8_t)(guid._g2 >> 24), (uint8_t)(guid._g2 >> 16), (uint8_t)(guid._g2 >> 8), (uint8_t)(guid._g2));
        return std::string(output, 68);
    }

    throw std::runtime_error("Bad GUID format");
}

GUID GUID::toGUID(const std::string& guidStr)
{
    GUID guid;

    uint32_t* part1 = ((uint32_t*)(&guid._g1)) + 1;
    uint16_t* part2 = ((uint16_t*)(&guid._g1)) + 1;
    uint16_t* part3 = ((uint16_t*)(&guid._g1));
    uint8_t* part4 = ((uint8_t*)(&guid._g2)) + 7;
    uint8_t* part5 = ((uint8_t*)(&guid._g2)) + 6;
    uint8_t* part6 = ((uint8_t*)(&guid._g2)) + 5;
    uint8_t* part7 = ((uint8_t*)(&guid._g2)) + 4;
    uint8_t* part8 = ((uint8_t*)(&guid._g2)) + 3;
    uint8_t* part9 = ((uint8_t*)(&guid._g2)) + 2;
    uint8_t* part10 = ((uint8_t*)(&guid._g2)) + 1;
    uint8_t* part11 = ((uint8_t*)(&guid._g2));

    const char* data = guidStr.c_str();

    int scanRet = 0;

    size_t guidSize = guidStr.size();
    if (guidSize == 24)
    {
        auto res = Base64::decode(guidStr);
        auto* dataPtr = res.data();
        std::memcpy(&guid._g1, dataPtr, sizeof(guid._g1));
        std::memcpy(&guid._g2, dataPtr + sizeof(guid._g1), sizeof(guid._g2));
        scanRet = 11;
    }
    else if (guidSize == 32)
    {
        scanRet = sscanf(data, "%08X%04hX%04hX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX", part1, part2, part3, part4, part5, part6, part7, part8, part9, part10, part11);
    }
    else if (guidSize == 36)
    {
        scanRet = sscanf(data, "%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX", part1, part2, part3, part4, part5, part6, part7, part8, part9, part10, part11);
    }
    else if (guidSize == 38)
    {
        if (data[0] == '{')
        {
            scanRet = sscanf(data, "{%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}", part1, part2, part3, part4, part5, part6, part7, part8, part9, part10, part11);
        }
        else
        {
            scanRet = sscanf(data, "(%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX)", part1, part2, part3, part4, part5, part6, part7, part8, part9, part10, part11);
        }
    }
    else if (guidSize == 68)
    {
        scanRet = sscanf(data, "{0x%08X,0x%04hX,0x%04hX,{0x%02hhX,0x%02hhX,0x%02hhX,0x%02hhX,0x%02hhX,0x%02hhX,0x%02hhX,0x%02hhX}}", part1, part2, part3, part4, part5, part6, part7, part8, part9, part10, part11);
    }

    if (scanRet != 11)
    {
        throw std::logic_error("Unvalid GUID, format should be XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX or Base64 (24 characters)");
    }

    return guid;
}

GUID GUID::generate()
{
    GUID guid;
    for (int i = 0; i < 8; i++)
    {
        guid._g1 <<= 8;
        guid._g1 |= std::rand();
        guid._g2 <<= 8;
        guid._g2 |= std::rand();
    }
    return guid;
}
