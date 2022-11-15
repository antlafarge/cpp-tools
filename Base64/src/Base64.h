#pragma once

#include <cstdint>

/// @brief Base64 namespace
namespace Base64
{
	/// @brief Encoding and decoding result codes
	/// @note [0x00] Success
	/// [0x01..0x1F] Warnings
	/// [0x20..0x3F] Encoding / Decoding errors
	/// [0x40..0x5F] Encoding only errors
	/// [0x60..0x7F] Decoding only errors
	/// [0x80..0xFF] Reserved
	enum class Result : uint8_t
	{
		/// @brief Success
		Success = 0x00, // 0

		/// @brief Warning indicating missing base64 padding characters '='
		MissingPaddingCharacters = 0x01, // 1

		/// @brief Warning indicating the padding bits were not set to 0 by the base64 encoder
		/// There may be a bug in the base64 encoder used
		InvalidPaddingBits = 0x02, // 2

		/// @brief Warning cf. MissingPaddingCharacters and cf. InvalidPaddingBits
		MissingPaddingCharactersAndInvalidPaddingBits = 0x03, // 3

		/// @brief Destination buffer size too small
		/// Allocate greater destination buffer
		InvalidDestinationBufferSize = 0x20, // 32

		/// @brief Invalid source buffer size
		/// Base64 encoded data size must be a multiple of 4
		InvalidSourceBufferSize = 0x60, // 96

		/// @brief Invalid base64 character
		/// Source data is not a valid base64 encoded string
		InvalidCharacter = 0x61 // 97
	};

	/// @brief Is a succeed result code
	/// @param result Result code
	/// @return boolean indicating a succeed result code (warnings are considered as succeed)
	bool succeed(Result result);

	/// @brief Compute the size needed for the encoded buffer allocation
	/// @param srcSize Source buffer size
	/// @return Size of the destination buffer to allocate
	uint64_t computeEncodedSize(uint64_t srcSize);

	/// @brief Encode source buffer to destination buffer
	/// @param srcPtr Source buffer
	/// @param srcSize Source buffer size
	/// @param dstPtr Destination buffer
	/// @param dstSize Destination buffer size
	/// @return Result cocde
	Result encode(const void* srcPtr, uint64_t srcSize, void* dstPtr, uint64_t& dstSize);

	/// @brief Compute the size needed for the decoded buffer allocation
	/// @param srcSize Source buffer size
	/// @param srcPtr Source buffer (optional)
	/// @return Size of the destination buffer to allocate
	uint64_t computeDecodedSize(uint64_t srcSize, const void* srcPtr = nullptr);

	/// @brief Decode source buffer to destination buffer
	/// @param srcPtr Source buffer
	/// @param srcSize Source buffer size
	/// @param dstPtr Destination buffer
	/// @param dstSize Destination buffer size (modified to the used byte count of the destination buffer size)
	/// @return Result cocde
	Result decode(const void* srcPtr, uint64_t srcSize, void* dstPtr, uint64_t& dstSize);
}
