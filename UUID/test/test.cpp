#include <cassert>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

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
    stream << "UUID_64[" << std::hex << "0x" << std::setw(16) << ptr64[0] << " 0x" << std::setw(16) << ptr64[1] << "]" << std::endl;

    stream << "UUID__8[";
    const uint8_t* ptr8 = reinterpret_cast<const uint8_t*>(&uuid);
    for (int i = 0; i < 16; i++)
    {
        stream << "0x" << std::hex << std::setw(2) << (uint16_t) * (ptr8 + i);
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
    test("123e4567-e89b-12d3-a456-426614174000", UUID::Format::D);
    test("123e4567e89b12d3a456426614174000", UUID::Format::N);
    test("{123e4567-e89b-12d3-a456-426614174000}", UUID::Format::B);
    test("(123e4567-e89b-12d3-a456-426614174000)", UUID::Format::P);
    test("{0x123e4567,0xe89b,0x12d3,{0xa4,0x56,0x42,0x66,0x14,0x17,0x40,0x00}}", UUID::Format::X);
    test("urn:uuid:123e4567-e89b-12d3-a456-426614174000", UUID::Format::URN);
    test("Ej5FZ+ibEtOkVkJmFBdAAA==", UUID::Format::Base64);

    test2(UUID("00112233-4455-6677-8899-AABBCCDDEEFF"), "00112233-4455-6677-8899-aabbccddeeff");

    uint8_t uuidPtr[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    test2(UUID(uuidPtr), "00112233-4455-6677-8899-aabbccddeeff");

    uint8_t uuidPtr2[16] = { 0x33, 0x22, 0x11, 0x00, 0x55, 0x44, 0x77, 0x66, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
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
        << generated.toString(UUID::Format::Base64) << std::endl
        << "[version|variant]=[" << (int)generated.getVersion() << "|" << (int)generated.getVariant() << "]" << std::endl
        << std::endl;

    for (uint8_t version = 1; version <= 5; version++)
    {
        generated.setVersion(version);
        std::cout << "setVersion " << (int)version << std::endl;
        uint8_t version2 = generated.getVersion();
        std::cout << "getVersion " << (int)version2 << std::endl;
        bool passed = (version2 == version);
        std::cout << (passed ? "PASSED" : "FAILED") << std::endl
            << std::endl;
        assert(passed);
    }

    for (uint8_t variant = 0; variant <= 3; variant++)
    {
        generated.setVariant(variant);
        std::cout << "setVariant " << (int)variant << std::endl;
        uint8_t variant2 = generated.getVariant();
        std::cout << "getVariant " << (int)variant2 << std::endl;
        bool passed = (variant2 == variant);
        std::cout << (passed ? "PASSED" : "FAILED") << std::endl
            << std::endl;
        assert(passed);
    }

    return 0;
}
