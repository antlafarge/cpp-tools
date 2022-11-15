#include <cassert>
#include <cctype>
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
    bool passed = (uuidStr2 == uuidResult);
    std::cout << (passed ? "PASSED" : "FAILED") << std::endl
        << std::endl;
    assert(passed);
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

    uint64_t uuidPtr[2] = { 0x1122334455667788, 0x9988776655443322 };
    std::string uuidStr8 = UUID(uuidPtr).toString();
    std::cout << uuidStr8 << std::endl;
    bool equal = (uuidStr8 == "11223344-5566-7788-9988-776655443322");
    std::cout << (equal ? "PASSED" : "FAILED") << std::endl
        << std::endl;
    assert(equal);

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
