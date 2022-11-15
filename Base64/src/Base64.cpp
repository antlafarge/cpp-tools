#include "Base64.h"

#if defined(_MSC_VER)
#	include <Windows.h>
#endif

#if ((defined(__BIG_ENDIAN__) && __BIG_ENDIAN__ == 1) || (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)) || ((defined(REG_DWORD) && (REG_DWORD == REG_DWORD_BIG_ENDIAN)))) // IS BIG ENDIAN
#	define BASE64_SWAP_BYTES(data) data
#else // DEFAULT IS LITTLE ENDIAN
#	if defined(_MSC_VER)
#		define BASE64_SWAP_BYTES(data) _byteswap_ulong(data)
#	else
#		define BASE64_SWAP_BYTES(data) __builtin_bswap32(data)
#	endif
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

#define BASE64_COMPUTE_DECODED_SIZE(srcSize, srcPtr) \
if (srcPtr != nullptr && srcSize >= 2) [[likely]] \
{ \
	const uint8_t* srcPtr2 = static_cast<const uint8_t*>(srcPtr); \
	if (srcPtr2[srcSize - 2] == '=') \
	{ \
		srcSize -= 2; \
	} \
	else if (srcPtr2[srcSize - 1] == '=') \
	{ \
		srcSize--; \
	} \
} \
uint64_t dstSize2 = ((srcSize * 3) / 4);

namespace Base64
{
	namespace
	{
		/// @brief Encoding table
		static const char* encodingTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		/// @brief Decoding table
		static const uint8_t decodingTable[256] =
		{
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   62, 0xFF, 0xFF, 0xFF,   63,
			  52,   53,   54,   55,   56,   57,   58,   59,   60,   61, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF,    0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,
			  15,   16,   17,   18,   19,   20,   21,   22,   23,   24,   25, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
			  41,   42,   43,   44,   45,   46,   47,   48,   49,   50,   51, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
		};
	}

	bool succeed(Result result)
	{
		return ((static_cast<uint8_t>(result) & 0xF0) <= 0x1F);
	}

	uint64_t computeEncodedSize(uint64_t srcSize)
	{
		return ((srcSize + 2) / 3) * 4;
	}

	Result encode(const void* srcPtr, uint64_t srcSize, void* dstPtr, uint64_t& dstSize)
	{
		uint64_t dstSize2 = computeEncodedSize(srcSize);
		if (dstSize < dstSize2) [[unlikely]]
		{
			return Result::InvalidDestinationBufferSize;
		}
		dstSize = dstSize2;

		const uint8_t* srcPtr2 = static_cast<const uint8_t*>(srcPtr);
		uint8_t* dstPtr2 = static_cast<uint8_t*>(dstPtr);

		uint64_t dstIndex = 0;
		int lastBytesCount = srcSize % 3;
		const uint8_t* srcPtr2End = srcPtr2 + srcSize - lastBytesCount;

		// [AAAAAABB|BBBBCCCC|CCDDDDDD|--------]

		for (; srcPtr2 < srcPtr2End; srcPtr2 += 3)
		{
			const uint32_t data = BASE64_SWAP_BYTES(*reinterpret_cast<const uint32_t*>(srcPtr2));
			dstPtr2[dstIndex++] = encodingTable[(data >> 26) & 0x3f]; // [00AAAAAA]
			dstPtr2[dstIndex++] = encodingTable[(data >> 20) & 0x3f]; // [00BBBBBB]
			dstPtr2[dstIndex++] = encodingTable[(data >> 14) & 0x3f]; // [00CCCCCC]
			dstPtr2[dstIndex++] = encodingTable[(data >> 8) & 0x3f]; //  [00DDDDDD]
		}

		if (lastBytesCount == 2)
		{
			const uint32_t data = BASE64_SWAP_BYTES(*reinterpret_cast<const uint32_t*>(srcPtr2));
			dstPtr2[dstIndex++] = encodingTable[(data >> 26) & 0x3f]; // [00AAAAAA]
			dstPtr2[dstIndex++] = encodingTable[(data >> 20) & 0x3f]; // [00BBBBBB]
			dstPtr2[dstIndex++] = encodingTable[(data >> 14) & 0x3c]; // [00CCCC00]
		}
		else if (lastBytesCount == 1)
		{
			const uint32_t data = BASE64_SWAP_BYTES(*reinterpret_cast<const uint32_t*>(srcPtr2));
			dstPtr2[dstIndex++] = encodingTable[(data >> 26) & 0x3f]; // [00AAAAAA]
			dstPtr2[dstIndex++] = encodingTable[(data >> 20) & 0x30]; // [00BB0000]
		}

		while (dstIndex % 4 != 0)
		{
			dstPtr2[dstIndex++] = '=';
		}

		return Result::Success;
	}

	uint64_t computeDecodedSize(uint64_t srcSize, const void* srcPtr)
	{
		BASE64_COMPUTE_DECODED_SIZE(srcSize, srcPtr);
		return dstSize2;
	}

	Result decode(const void* srcPtr, uint64_t srcSize, void* dstPtr, uint64_t& dstSize)
	{
		uint64_t srcSizeMod4 = srcSize % 4;
		if (srcSizeMod4 == 1) [[unlikely]]
		{
			return Result::InvalidSourceBufferSize;
		}

		BASE64_COMPUTE_DECODED_SIZE(srcSize, srcPtr);
		if (dstSize < dstSize2) [[unlikely]]
		{
			return Result::InvalidDestinationBufferSize;
		}
		dstSize = dstSize2;

		const uint8_t* srcPtr2 = static_cast<const uint8_t*>(srcPtr);
		uint8_t* dstPtr2 = static_cast<uint8_t*>(dstPtr);

		for (; srcSize >= 4; srcSize -= 4)
		{
			// [--AAAAAA]
			uint8_t A = decodingTable[srcPtr2[0]];

			if (A == 0xFF) [[unlikely]]
			{
				return Result::InvalidCharacter;
			}

			// [--bbBBBB]
			uint8_t B = decodingTable[srcPtr2[1]];

			if (B == 0xFF) [[unlikely]]
			{
				return Result::InvalidCharacter;
			}

			(*(dstPtr2++)) = ((A << 2) | (B >> 4)); // [AAAAAAbb]

			// [--ccccCC]
			uint8_t C = decodingTable[srcPtr2[2]];

			if (C == 0xFF) [[unlikely]]
			{
				return Result::InvalidCharacter;
			}

			(*(dstPtr2++)) = ((B << 4) | (C >> 2)); // [BBBBcccc]

			// [--dddddd]
			uint8_t D = decodingTable[srcPtr2[3]];

			if (D == 0xFF) [[unlikely]]
			{
				return Result::InvalidCharacter;
			}

			(*(dstPtr2++)) = ((C << 6) | D); // [CCdddddd]

			srcPtr2 += 4;
		}

		Result result = (srcSizeMod4 == 0 ? Result::Success : Result::MissingPaddingCharacters);

		if (srcSize >= 2)
		{
			// [--AAAAAA]
			uint8_t A = decodingTable[srcPtr2[0]];

			if (A == 0xFF) [[unlikely]]
			{
				return Result::InvalidCharacter;
			}

			// [--bbBBBB]
			uint8_t B = decodingTable[srcPtr2[1]];

			if (B == 0xFF) [[unlikely]]
			{
				return Result::InvalidCharacter;
			}

			(*(dstPtr2++)) = ((A << 2) | (B >> 4)); // [AAAAAAbb]

			if (srcSize >= 3)
			{
				// [--ccccCC]
				uint8_t C = decodingTable[srcPtr2[2]];

				if (C != 0xFF) [[likely]]
				{
					(*(dstPtr2++)) = ((B << 4) | (C >> 2)); // [BBBBcccc]

					if (C & 0x03) [[unlikely]]
					{
						result = static_cast<Result>(static_cast<uint8_t>(result) | static_cast<uint8_t>(Result::InvalidPaddingBits));
					}

					return result;
				}

				if (srcPtr2[2] != '=') [[likely]]
				{
					return Result::InvalidCharacter;
				}
			}

			if (B & 0x0f) [[unlikely]]
			{
				result = static_cast<Result>(static_cast<uint8_t>(result) | static_cast<uint8_t>(Result::InvalidPaddingBits));
			}

			return result;
		}

		return Result::Success;
	}
}

#pragma GCC diagnostic pop
