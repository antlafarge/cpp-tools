#include <cassert>
#include <cctype>
#include <iomanip>
#include <iostream>

#include "UUID.h"

std::string toLower(const std::string& str)
{
    std::string result = str;
    for (char& c : result)
    {
        c = (char)std::tolower((int)c);
    }
    return result;
}

std::string debugUUID(const UUID& uuid)
{
    std::stringstream stream;
    stream << std::setfill('0');

    const uint64_t* ptr64 = reinterpret_cast<const uint64_t*>(&uuid);
    stream << "UUID_64[" << std::hex << "0x" << std::setw(16) << ptr64[0]  << " 0x" << std::setw(16) << ptr64[1] << "]" << std::endl;

    stream << "UUID__8[";
    const uint8_t* ptr8 = reinterpret_cast<const uint8_t*>(&uuid);
    for (int i = 0; i < 16; i++)
    {
        stream << "0x" << std::hex << std::setw(2) << (uint16_t)*(ptr8 + i);
        if (i < 15)
        {
            stream << " ";
        }
    }
    stream << "]";

    return stream.str();
}

void test(const std::string& uuidStr, UUID::Format format)
{
    std::cout << "Source=" << uuidStr << " (" << uuidStr.size() << ")" << std::endl;
    std::string uuidStr2;
    if (format != UUID::Format::Base64)
    {
        uuidStr2 = toLower(uuidStr);
        std::cout << "Lower =" << uuidStr2 << " (" << uuidStr2.size() << ")" << std::endl;
    }
    else
    {
        uuidStr2 = uuidStr;
    }
    std::string uuidResult = UUID(uuidStr).toString(format);
    std::cout << "Result=" << uuidResult << " (" << uuidResult.size() << ")" << std::endl;
    bool equal = (uuidStr2 == uuidResult);
    std::cout << (equal ? "PASSED" : "FAILED") << std::endl
        << std::endl;
    assert(equal);
}

void test2(const UUID& uuid, const std::string& uuidStr)
{
    std::cout << debugUUID(uuid) << std::endl;
    std::string uuidStr2 = uuid.toString();
    std::cout << uuidStr2 << std::endl;
    bool equal = (uuidStr2 == uuidStr);
    std::cout << (equal ? "PASSED" : "FAILED") << std::endl
        << std::endl;
    assert(equal);
}

int main()
{
    std::string uuidStr1 = "F9168C5E-CEB2-4faa-B6BF-329BF39FA1E4";
    std::string uuidStr2 = "F9168C5ECEB24FAAB6BF329BF39FA1E4";
    std::string uuidStr3 = "{F9168C5E-CEB2-4faa-B6BF-329BF39FA1E4}";
    std::string uuidStr4 = "(F9168C5E-CEB2-4faa-B6BF-329BF39FA1E4)";
    std::string uuidStr5 = "{0xF9168C5E,0xCEB2,0x4faa,{0xB6,0xBF,0x32,0x9B,0xF3,0x9F,0xA1,0xE4}}";
    std::string uuidStr6 = "urn:uuid:F9168C5E-CEB2-4faa-B6BF-329BF39FA1E4";
    std::string uuidStr7 = "qk+yzl6MFvnkoZ/zmzK/tg==";

    test(uuidStr1, UUID::Format::D);
    test(uuidStr2, UUID::Format::N);
    test(uuidStr3, UUID::Format::B);
    test(uuidStr4, UUID::Format::P);
    test(uuidStr5, UUID::Format::X);
    test(uuidStr6, UUID::Format::URN);
    test(uuidStr7, UUID::Format::Base64);

    test2(UUID("00112233-4455-6677-8899-AABBCCDDEEFF"), "00112233-4455-6677-8899-aabbccddeeff");

    uint8_t uuidPtr[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    test2(UUID(uuidPtr), "00112233-4455-6677-8899-aabbccddeeff");

    uint8_t uuidPtr2[16] = { 0x33, 0x22, 0x11, 0x00, 0x55, 0x44, 0x77, 0x66, /*0xC8*/0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    test2(UUID(uuidPtr2, true), "00112233-4455-6677-8899-aabbccddeeff");

    test2(UUID("ABEiM0RVZneImaq7zN3u/w=="), "00112233-4455-6677-8899-aabbccddeeff");

    UUID generated = UUID::generate();
    std::cout << "Generated:" << std::endl
        << generated.toString(UUID::Format::D) << std::endl
        << generated.toString(UUID::Format::N) << std::endl
        << generated.toString(UUID::Format::B) << std::endl
        << generated.toString(UUID::Format::P) << std::endl
        << generated.toString(UUID::Format::X) << std::endl
        << generated.toString(UUID::Format::URN) << std::endl
        << generated.toString(UUID::Format::Base64) << std::endl;

    return 0;
}
