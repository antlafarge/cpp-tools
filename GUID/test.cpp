#include <iostream>
#include <string>

#include "GUID.h"

int main()
{
    std::string guidStr1 = "F9168C5ECEB24FAAB6BF329BF39FA1E4";
    std::string guidStr2 = "F9168C5E-CEB2-4faa-B6BF-329BF39FA1E4";
    std::string guidStr3 = "{F9168C5E-CEB2-4faa-B6BF-329BF39FA1E4}";
    std::string guidStr4 = "(F9168C5E-CEB2-4faa-B6BF-329BF39FA1E4)";
    std::string guidStr5 = "{0xF9168C5E,0xCEB2,0x4faa,{0xB6,0xBF,0x32,0x9B,0xF3,0x9F,0xA1,0xE4}}";

    std::cout << "source=" << guidStr1 << std::endl;
    std::string guidB64 = GUID(guidStr1).toString(GuidFormat::Base64);
    std::cout << "encode=" << guidB64 << std::endl;
    std::string guidN = GUID(guidB64).toString(GuidFormat::N);
    std::cout << "decode=" << guidN << std::endl;
    std::cout << (guidN == guidStr1 ? "PASSED" : "FAILED") << std::endl << std::endl;

    std::cout << GUID(guidStr1).toString(GuidFormat::N) << std::endl;
    std::cout << GUID(guidStr2).toString(GuidFormat::D) << std::endl;
    std::cout << GUID(guidStr3).toString(GuidFormat::B) << std::endl;
    std::cout << GUID(guidStr4).toString(GuidFormat::P) << std::endl;
    std::cout << GUID(guidStr5).toString(GuidFormat::X) << std::endl;

    return 0;
}
