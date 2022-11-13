#pragma once

#include "Base64.h"

#include <string>
#include <vector>

namespace Base64
{
	std::string encode(const void* srcPtr, uint64_t srcSize);

	std::string encode(const std::vector<uint8_t>& src);

	std::string encode(const std::string& src);

	std::vector<uint8_t> decode(const void* srcPtr, uint64_t srcSize);

	std::vector<uint8_t> decode(const std::vector<uint8_t>& src);

	std::vector<uint8_t> decode(const std::string& src);
}
