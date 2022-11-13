#include "Base64.helpers.h"

#include <stdexcept>

namespace Base64
{
    std::string encode(const void* srcPtr, uint64_t srcSize)
    {
        uint64_t dstSize = computeEncodedSize(srcSize);
        std::string dst(dstSize, 0);
#if _HAS_CXX17 == 1
        uint8_t* dstPtr = reinterpret_cast<uint8_t*>(dst.data());
#else
        uint8_t* dstPtr = reinterpret_cast<uint8_t*>(const_cast<char*>(dst.data()));
#endif
        Result res = encode(srcPtr, srcSize, dstPtr, dstSize);
        if (!succeed(res)) [[unlikely]]
        {
            throw std::runtime_error("base64_encode_" + std::to_string(static_cast<uint32_t>(res)));
        }
        return dst;
    }

    std::string encode(const std::vector<uint8_t>& src)
    {
        return encode(src.data(), src.size());
    }

    std::string encode(const std::string& src)
    {
        return encode(src.data(), src.size());
    }

    std::vector<uint8_t> decode(const void* srcPtr, uint64_t srcSize)
    {
        uint64_t dstSize = computeDecodedSize(srcSize, srcPtr);
        std::vector<uint8_t> dst(dstSize);
        Result res = decode(srcPtr, srcSize, dst.data(), dstSize);
        if (!succeed(res)) [[unlikely]]
        {
            throw std::runtime_error("base64_decode_" + std::to_string(static_cast<uint32_t>(res)));
        }
        return dst;
    }

    std::vector<uint8_t> decode(const std::vector<uint8_t>& src)
    {
        return decode(src.data(), src.size());
    }

    std::vector<uint8_t> decode(const std::string& src)
    {
        return decode(src.data(), src.size());
    }
}
