#pragma once

#include "Base64.h"

#include <string>
#include <vector>

/// @brief Base64 namespace
namespace Base64
{
	/// @brief Encode from buffer to string
	/// @param srcPtr Source buffer
	/// @param srcSize Source buffer size
	/// @return Encoded string
	std::string encode(const void* srcPtr, uint64_t srcSize);

	/// @brief Encode from vector to string
	/// @param srcPtr Source buffer as vector
	/// @return Encoded string
	std::string encode(const std::vector<uint8_t>& src);

	/// @brief Encode from string to string
	/// @param srcPtr Source buffer as string
	/// @return Encoded string
	std::string encode(const std::string& src);

	/// @brief Decode from buffer to vector
	/// @param srcPtr Source buffer
	/// @param srcSize Source buffer size
	/// @return Decoded vector
	std::vector<uint8_t> decode(const void* srcPtr, uint64_t srcSize);

	/// @brief Decode from vector to vector
	/// @param srcPtr Source buffer as vector
	/// @return Decoded vector
	std::vector<uint8_t> decode(const std::vector<uint8_t>& src);

	/// @brief Decode from string to vector
	/// @param srcPtr Source buffer as string
	/// @return Decoded vector
	std::vector<uint8_t> decode(const std::string& src);
}
