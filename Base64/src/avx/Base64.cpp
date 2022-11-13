#include "Base64.h"

#include <array>
#include <bitset>
#include <cstring>
#include <immintrin.h>
#include <iomanip>
#include <iostream>
#include <vector>

#if defined(_WIN32)
#	if defined(__GNUC__)
#		include <cpuid.h>
#		define BASE64_CPU_SUPPORTS(instructionsSetName) __builtin_cpu_supports(#instructionsSetName)
#	elif defined(_MSC_VER)
#		include <Windows.h>
#		include <intrin.h>
class InstructionSet
{
	// forward declarations
	class InstructionSet_Internal;

public:
	// getters
	static std::string Vendor(void) { return CPU_Rep.vendor_; }
	static std::string Brand(void) { return CPU_Rep.brand_; }

	static bool SSE3(void) { return CPU_Rep.f_1_ECX_[0]; }
	static bool PCLMULQDQ(void) { return CPU_Rep.f_1_ECX_[1]; }
	static bool MONITOR(void) { return CPU_Rep.f_1_ECX_[3]; }
	static bool SSSE3(void) { return CPU_Rep.f_1_ECX_[9]; }
	static bool FMA(void) { return CPU_Rep.f_1_ECX_[12]; }
	static bool CMPXCHG16B(void) { return CPU_Rep.f_1_ECX_[13]; }
	static bool SSE41(void) { return CPU_Rep.f_1_ECX_[19]; }
	static bool SSE42(void) { return CPU_Rep.f_1_ECX_[20]; }
	static bool MOVBE(void) { return CPU_Rep.f_1_ECX_[22]; }
	static bool POPCNT(void) { return CPU_Rep.f_1_ECX_[23]; }
	static bool AES(void) { return CPU_Rep.f_1_ECX_[25]; }
	static bool XSAVE(void) { return CPU_Rep.f_1_ECX_[26]; }
	static bool OSXSAVE(void) { return CPU_Rep.f_1_ECX_[27]; }
	static bool AVX(void) { return CPU_Rep.f_1_ECX_[28]; }
	static bool F16C(void) { return CPU_Rep.f_1_ECX_[29]; }
	static bool RDRAND(void) { return CPU_Rep.f_1_ECX_[30]; }

	static bool MSR(void) { return CPU_Rep.f_1_EDX_[5]; }
	static bool CX8(void) { return CPU_Rep.f_1_EDX_[8]; }
	static bool SEP(void) { return CPU_Rep.f_1_EDX_[11]; }
	static bool CMOV(void) { return CPU_Rep.f_1_EDX_[15]; }
	static bool CLFSH(void) { return CPU_Rep.f_1_EDX_[19]; }
	static bool MMX(void) { return CPU_Rep.f_1_EDX_[23]; }
	static bool FXSR(void) { return CPU_Rep.f_1_EDX_[24]; }
	static bool SSE(void) { return CPU_Rep.f_1_EDX_[25]; }
	static bool SSE2(void) { return CPU_Rep.f_1_EDX_[26]; }

	static bool FSGSBASE(void) { return CPU_Rep.f_7_EBX_[0]; }
	static bool BMI1(void) { return CPU_Rep.f_7_EBX_[3]; }
	static bool HLE(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[4]; }
	static bool AVX2(void) { return CPU_Rep.f_7_EBX_[5]; }
	static bool BMI2(void) { return CPU_Rep.f_7_EBX_[8]; }
	static bool ERMS(void) { return CPU_Rep.f_7_EBX_[9]; }
	static bool INVPCID(void) { return CPU_Rep.f_7_EBX_[10]; }
	static bool RTM(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[11]; }
	static bool AVX512F(void) { return CPU_Rep.f_7_EBX_[16]; }
	static bool RDSEED(void) { return CPU_Rep.f_7_EBX_[18]; }
	static bool ADX(void) { return CPU_Rep.f_7_EBX_[19]; }
	static bool AVX512PF(void) { return CPU_Rep.f_7_EBX_[26]; }
	static bool AVX512ER(void) { return CPU_Rep.f_7_EBX_[27]; }
	static bool AVX512CD(void) { return CPU_Rep.f_7_EBX_[28]; }
	static bool SHA(void) { return CPU_Rep.f_7_EBX_[29]; }

	static bool PREFETCHWT1(void) { return CPU_Rep.f_7_ECX_[0]; }

	static bool LAHF(void) { return CPU_Rep.f_81_ECX_[0]; }
	static bool LZCNT(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_ECX_[5]; }
	static bool ABM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[5]; }
	static bool SSE4a(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[6]; }
	static bool XOP(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[11]; }
	static bool TBM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[21]; }

	static bool SYSCALL(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[11]; }
	static bool MMXEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[22]; }
	static bool RDTSCP(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[27]; }
	static bool _3DNOWEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[30]; }
	static bool _3DNOW(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[31]; }

private:
	static const InstructionSet_Internal CPU_Rep;

	class InstructionSet_Internal
	{
	public:

		InstructionSet_Internal()
			: nIds_{ 0 },
			nExIds_{ 0 },
			isIntel_{ false },
			isAMD_{ false },
			f_1_ECX_{ 0 },
			f_1_EDX_{ 0 },
			f_7_EBX_{ 0 },
			f_7_ECX_{ 0 },
			f_81_ECX_{ 0 },
			f_81_EDX_{ 0 },
			data_{},
			extdata_{}
		{
			//int cpuInfo[4] = {-1};
			std::array<int, 4> cpui;

			// Calling __cpuid with 0x0 as the function_id argument
			// gets the number of the highest valid function ID.
			__cpuid(cpui.data(), 0);
			nIds_ = cpui[0];

			for (unsigned int i = 0; i <= nIds_; ++i)
			{
				__cpuidex(cpui.data(), i, 0);
				data_.push_back(cpui);
			}

			// Capture vendor string
			char vendor[0x20];
			memset(vendor, 0, sizeof(vendor));
			*reinterpret_cast<int*>(vendor) = data_[0][1];
			*reinterpret_cast<int*>(vendor + 4) = data_[0][3];
			*reinterpret_cast<int*>(vendor + 8) = data_[0][2];
			vendor_ = vendor;
			if (vendor_ == "GenuineIntel")
			{
				isIntel_ = true;
			}
			else if (vendor_ == "AuthenticAMD")
			{
				isAMD_ = true;
			}

			// load bitset with flags for function 0x00000001
			if (nIds_ >= 1)
			{
				f_1_ECX_ = data_[1][2];
				f_1_EDX_ = data_[1][3];
			}

			// load bitset with flags for function 0x00000007
			if (nIds_ >= 7)
			{
				f_7_EBX_ = data_[7][1];
				f_7_ECX_ = data_[7][2];
			}

			// Calling __cpuid with 0x80000000 as the function_id argument
			// gets the number of the highest valid extended ID.
			__cpuid(cpui.data(), 0x80000000);
			nExIds_ = cpui[0];

			char brand[0x40];
			memset(brand, 0, sizeof(brand));

			for (unsigned int i = 0x80000000; i <= nExIds_; ++i)
			{
				__cpuidex(cpui.data(), i, 0);
				extdata_.push_back(cpui);
			}

			// load bitset with flags for function 0x80000001
			if (nExIds_ >= 0x80000001)
			{
				f_81_ECX_ = extdata_[1][2];
				f_81_EDX_ = extdata_[1][3];
			}

			// Interpret CPU brand string if reported
			if (nExIds_ >= 0x80000004)
			{
				memcpy(brand, extdata_[2].data(), sizeof(cpui));
				memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
				memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
				brand_ = brand;
			}
		};

		unsigned int nIds_;
		unsigned int nExIds_;
		std::string vendor_;
		std::string brand_;
		bool isIntel_;
		bool isAMD_;
		std::bitset<32> f_1_ECX_;
		std::bitset<32> f_1_EDX_;
		std::bitset<32> f_7_EBX_;
		std::bitset<32> f_7_ECX_;
		std::bitset<32> f_81_ECX_;
		std::bitset<32> f_81_EDX_;
		std::vector<std::array<int, 4>> data_;
		std::vector<std::array<int, 4>> extdata_;
	};
};
// Initialize static member data
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;
#		define BASE64_CPU_SUPPORTS(instructionsSetName) InstructionSet:: ## instructionsSetName()
#	endif
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

#define CMP(val) (val ? 0xff : 0x00)
#define AND_NOT(a, b) (~a & b)

void logHex(const std::string &name, void*data)
{
	std::cout << name << " =\t0x[ ";
	uint8_t*dataPtr = (uint8_t*)data;
	for (int intCount = 0; intCount < 8; intCount++)
	{
		std::cout << std::hex << std::setfill('0') << std::uppercase;
		for (int byteCount = 3; byteCount >= 0; byteCount++)
		{
			std::cout << std::setw(2) << static_cast<uint16_t>(dataPtr[intCount * 4 + byteCount]);
		}
		std::cout << ' ';
	}
	std::cout << "]" << std::endl;
}

void logBin(const std::string &name, void*data)
{
	std::cout << name << " =\t0b[ ";
	uint8_t*dataPtr = (uint8_t*)data;
	for (int intCount = 0; intCount < 8; intCount++)
	{
		for (int byteCount = 3; byteCount >= 0; byteCount--)
		{
			std::cout << std::hex << std::setfill('0') << std::uppercase << std::setw(8) << std::bitset<8>(dataPtr[intCount * 4 + byteCount]);
			if (byteCount > 0)
			{
				std::cout << "|";
			}
		}
		std::cout << " ";
	}
	std::cout << "]" << std::endl;
}

void log(const std::string &name, void*data)
{
	// logHex(name, data);
	logBin(name, data);
}

namespace Base64
{
	namespace
	{
		/// @brief Encoding table
		static const char* encodingTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		/// @brief Decoding
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

		size_t dstIndex = 0;
		__m256i dataInitial = _mm256_set1_epi32(0);
		(void)dataInitial;
		__m256i dataShuffled = _mm256_set1_epi32(0);
		(void)dataShuffled;
		__m256i dataTmp = _mm256_set1_epi32(0);
		(void)dataTmp;
		__m256i dataFinal = _mm256_set1_epi32(0);
		__m256i shuf = _mm256_setr_epi8(
			6, 5, 4, 6,
			9, 8, 7, 9,
			12, 11, 10, 12,
			15, 14, 13, 15,
			2, 1, 0, 2,
			5, 4, 3, 5,
			8, 7, 6, 8,
			11, 10, 9, 11);
		(void)shuf;
		__m256i shuf2 = _mm256_setr_epi8(
			5, 4, 6, 5,
			8, 7, 9, 8,
			11, 10, 12, 11,
			14, 13, 15, 14,
			1, 0, 2, 1,
			4, 3, 5, 4,
			7, 6, 8, 7,
			10, 9, 11, 10);
		(void)shuf2;
		const __m256i m0000003F = _mm256_set1_epi32(0x0000003F);
		(void)m0000003F;
		const __m256i m00003F00 = _mm256_set1_epi32(0x00003F00);
		(void)m00003F00;
		const __m256i m003F0000 = _mm256_set1_epi32(0x003F0000);
		(void)m003F0000;
		const __m256i m3F000000 = _mm256_set1_epi32(0x3F000000);
		(void)m3F000000;
		const __m256i m3F003F00 = _mm256_set1_epi32(0x3F003F00);
		(void)m3F003F00;
		const __m256i m003F003F = _mm256_set1_epi32(0x003F003F);
		(void)m003F003F;
		const __m256i m04000040 = _mm256_set1_epi32(0x04000040);
		(void)m04000040;
		const __m256i m01000010 = _mm256_set1_epi32(0x01000010);
		(void)m01000010;
		size_t blockSize = 0, charSize = 0;

		while (srcSize)
		{
			if (srcSize >= 24) [[likely]]
			{
				blockSize = 24;
				charSize = 0;
				dataInitial = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(srcPtr2 - 4));
			}
			else [[unlikely]]
			{
				blockSize = srcSize;
				if (blockSize % 3) [[likely]]
				{
					charSize = 1;
				}
				uint8_t byteArrayPtr2[24] = { 0 };
				std::memcpy(byteArrayPtr2, srcPtr2, blockSize);
				dataInitial = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(byteArrayPtr2 - 4));
			}

				// log("dataInitial", &dataInitial);

			srcSize -= blockSize;
			srcPtr2 += blockSize;
			charSize += (blockSize * 4) / 3;

			// // ======== first ========
			// {
			// 	dataShuffled = _mm256_shuffle_epi8(dataInitial, shuf);
			// 	// log("dataShuffled", &dataShuffled);

			// 	dataFinal = _mm256_and_si256(_mm256_srli_epi32(dataShuffled, 18), m0000003F);
			// 	dataFinal = _mm256_or_epi32(dataFinal, _mm256_and_si256(_mm256_srli_epi32(dataShuffled, 4), m00003F00));
			// 	dataFinal = _mm256_or_epi32(dataFinal, _mm256_and_si256(_mm256_slli_epi32(dataShuffled, 10), m003F0000));
			// 	dataFinal = _mm256_or_epi32(dataFinal, _mm256_and_si256(dataShuffled, m3F000000));
			// 	// log("dataFinal", &dataFinal);
			// }

			// ======== optimized ========
			{
				dataShuffled = _mm256_shuffle_epi8(dataInitial, shuf2);
				// log("dataShuffled", &dataShuffled);

				// dataFinal = (shiftRight AND m003F003F) OR (shiftLeft AND m3F003F00)
				dataFinal = _mm256_and_si256(_mm256_mulhi_epu16(dataShuffled, m04000040), m003F003F);
				dataFinal = _mm256_or_si256(dataFinal, _mm256_and_si256(_mm256_mullo_epi16(dataShuffled, m01000010), m3F003F00));
				// log("dataFinal", &dataFinal);
			}

			// // ======== XOP ======== (not finished) (AMD processors)
			// {
			// 	dataShuffled = _mm256_shuffle_epi8(dataInitial, shuf2);
			// 	// log("dataShuffled", &dataShuffled);

			// 	if (BASE64_CPU_SUPPORTS(XOP))
			// 	{
			// 		// 256
			// 		auto a = _mm256_set1_epi32(0);
			// 		log("a", &a);
			// 		auto b = _mm256_set1_epi32(1);
			// 		log("b", &b);
			// 		auto c = _mm256_set1_epi32(2);
			// 		log("c", &c);
			// 		auto d = _mm256_shldv_epi16(a, b, c);
			// 		log("d", &d);

			// 		// 128
			// 		auto a = _mm_set1_epi32(0);
			// 		log("a", &a);
			// 		auto b = _mm_set1_epi32(1);
			// 		log("b", &b);
			// 		auto c = _mm_set1_epi32(2);
			// 		log("c", &c);
			// 		auto d = _mm_shl_epi16(a, b);
			// 		log("d", &d);
			// 	}
			// 	// log("dataFinal", &dataFinal);
			// }

			// // ======== BMI ========
			// {
			// 	auto input = byteArrayPtr;

			// 	// input = [????????|????????|ccdddddd|bbbbcccc|aaaaaabb|gghhhhhh|ffffgggg|eeeeeeff]
			// 	const uint64_t lo = *reinterpret_cast<const uint64_t*>(input);
			// 	const uint64_t hi = *reinterpret_cast<const uint64_t*>(input + 6);

			// 	// t     = [00000000|00000000|eeeeeeff|ffffgggg|gghhhhhh|aaaaaabb|bbbbcccc|ccdddddd]
			// 	uint64_t t0 = __builtin_bswap64(lo) >> 16;
			// 	uint64_t t1 = __builtin_bswap64(hi) >> 16;

			// 	// exp   = [00eeeeee|00ffffff|00gggggg|00hhhhhh|00aaaaaa|00bbbbbb|00cccccc|00dddddd]
			// 	const uint64_t expanded_lo = _pdep_u32(lo, 0x3f3f3f3f3f3f3f3flu);
			// 	const uint64_t expanded_hi = _pdep_u32(hi, 0x3f3f3f3f3f3f3f3flu);

			// 	// indices = [m n o p|i j k l|e f g h|a b c d]
			// 	__m128i indices;

			// 	indices = _mm_insert_epi64(indices, expanded_lo, 0);
			// 	indices = _mm_insert_epi64(indices, expanded_hi, 1);

			// 	// indices = [l k j i|p o n m|d c b a|h g f e]
			// 	indices = _mm_shuffle_epi8(indices, magic_constant);
			// }

			uint8_t* finalBytes = (uint8_t*)&dataFinal;
			for (size_t byteCount = 0; byteCount < charSize; byteCount++)
			{
				dstPtr2[dstIndex++] = encodingTable[finalBytes[byteCount]];
			}
		}

		while (dstIndex % 4 != 0)
		{
			dstPtr2[dstIndex++] = '=';
		}

		return Result::Success;
	}

	uint64_t computeDecodedSize(uint64_t srcSize, const void* srcPtr)
	{
		if (srcPtr != nullptr && srcSize >= 2) [[likely]]
		{
			const uint8_t* srcPtr2 = static_cast<const uint8_t*>(srcPtr);
			if (srcPtr2[srcSize - 2] == '=')
			{
				srcSize -= 2;
			}
			else if (srcPtr2[srcSize - 1] == '=')
			{
				srcSize--;
			}
		}

		return ((srcSize * 3) / 4);
	}

	Result decode(const void* srcPtr, uint64_t srcSize, void* dstPtr, uint64_t& dstSize)
	{
		// TODO : This is the standard version, implement a avx version

		if ((srcSize % 4) == 1) [[unlikely]]
		{
			return Result::InvalidSourceBufferSize;
		}

		uint64_t dstSize2 = computeDecodedSize(srcSize, srcPtr);
		if (dstSize < dstSize2) [[unlikely]]
		{
			return Result::InvalidDestinationBufferSize;
		}
		dstSize = dstSize2;

		const uint8_t* srcPtr2 = static_cast<const uint8_t*>(srcPtr);
		uint8_t* dstPtr2 = reinterpret_cast<uint8_t*>(dstPtr);

		for (; srcSize >= 4; srcSize -= 4)
		{
			// [--AAAAAA]
			uint8_t A = decodingTable[srcPtr2[0]];

			if (A == 0xFF) [[unlikely]]
			{
				return Result::InvalidCharacter;
			}

			// [--BBbbbb]
			uint8_t B = decodingTable[srcPtr2[1]];

			if (B == 0xFF) [[unlikely]]
			{
				return Result::InvalidCharacter;
			}

			(*(dstPtr2++)) = ((A << 2) | (B >> 4)); // [AAAAAABB]

			// [--ccccCC]
			uint8_t C = decodingTable[srcPtr2[2]];

			if (C == 0xFF) [[unlikely]]
			{
				if (srcSize <= 4 && srcPtr2[2] == '=' && srcPtr2[3] == '=') [[likely]]
				{
					return Result::Success;
				}
				else [[unlikely]]
				{
					return Result::InvalidCharacter;
				}
			}

			(*(dstPtr2++)) = ((B << 4) | (C >> 2)); // [bbbbcccc]

			// [--DDDDDD]
			uint8_t D = decodingTable[srcPtr2[3]];

			if (D == 0xFF) [[unlikely]]
			{
				if (srcSize <= 4 && srcPtr2[3] == '=') [[likely]]
				{
					return Result::Success;
				}
				else [[unlikely]]
				{
					return Result::InvalidCharacter;
				}
			}

			(*(dstPtr2++)) = ((C << 6) | D); // [CCDDDDDD]

			srcPtr2 += 4;
		}

		if (srcSize >= 2)
		{
			// [--AAAAAA]
			uint8_t A = decodingTable[srcPtr2[0]];

			if (A == 0xFF) [[unlikely]]
			{
				return Result::InvalidCharacter;
			}

			// [--BBbbbb]
			uint8_t B = decodingTable[srcPtr2[1]];

			if (B == 0xFF) [[unlikely]]
			{
				return Result::InvalidCharacter;
			}

			(*(dstPtr2++)) = ((A << 2) | (B >> 4)); // [AAAAAABB]

			if (srcSize >= 3)
			{
				// [--cccc--]
				uint8_t C = decodingTable[srcPtr2[2]];

				if (C == 0xFF) [[unlikely]]
				{
					if (srcPtr2[2] == '=') [[likely]]
					{
						return Result::MissingPadding;
					}
					else [[unlikely]]
					{
						return Result::InvalidCharacter;
					}
				}

				(*(dstPtr2++)) = ((B << 4) | (C >> 2)); // [bbbbcccc]
			}

			return Result::MissingPadding;
		}

		return Result::Success;
	}
}

#pragma GCC diagnostic pop
