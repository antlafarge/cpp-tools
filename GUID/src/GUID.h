#pragma once

#include <string>

enum class GuidFormat
{
    N = 0,
    D = 1,
    B = 2,
    P = 3,
    X = 4,
    Base64 = 5
};

class GUID
{
public:

    GUID() = default;

    GUID(const GUID& other) = default;

    GUID(const std::string& guidStr);

    ~GUID() = default;

    GUID& operator=(const GUID& right) = default;

    bool operator==(const GUID& right);

    bool operator!=(const GUID& right);

    std::string toString(GuidFormat format = GuidFormat::Base64);

    static std::string toString(const GUID& guid, GuidFormat format = GuidFormat::Base64);

    static GUID toGUID(const std::string& guidStr);

    static GUID generate();

private:

    uint64_t _g1 = 0;
    uint64_t _g2 = 0;
};
