#pragma once

#include <cstdint>
#include <iostream>

#define JSON_DEBUG false

#define JSON_DEFAULT_ENCODING JSON::Encoding::UTF8

#define JSON_DEFAULT_ENCODING_WSTRING Encoding::UTF16BE

#define JSON_DEFAULT_WRITE_UTF8_BOM false

#define JSON_DEFAULT_WRITE_UTF16_BOM true

#if ((defined(__BIG_ENDIAN__) && __BIG_ENDIAN__ == 1) || (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)) || ((defined(REG_DWORD) && (REG_DWORD == REG_DWORD_BIG_ENDIAN))))
#	define JSON_PLATFORM_IS_BIG_ENDIAN true
#else
#	define JSON_PLATFORM_IS_LITTLE_ENDIAN true
#endif

#define JSON_UNGET(stream, size) for (int32_t i = 0; i < size; i++) { stream.unget(); }

#define JSON_COPY_BUFFER_ON_HEAP true
#define JSON_COPY_BUFFER_SIZE 1024000

constexpr uint32_t JSON_EOF = static_cast<uint32_t>(EOF);

namespace JSON
{
	enum class Encoding : uint8_t
	{
		Unknown			= 0x00,
		// UTF-8
		UTF8			= 0x01,
		// UTF-16
		UTF16			= 0x02,
		UTF16LE			= 0x12,
		UTF16BE			= 0x22,
		// UTF-32
		UTF32			= 0x04,
		UTF32LE			= 0x14,
		UTF32BE			= 0x24,
		// Endianness
		LittleEndian	= 0x10,
		BigEndian		= 0x20
	};

	Encoding operator|(Encoding lhs, Encoding rhs);

	Encoding operator&(Encoding lhs, Encoding rhs);

	bool hasFlags(Encoding encoding, Encoding flags, bool acceptUnknown = false);

	uint32_t codePointToUtf8(uint32_t codePoint);

	uint32_t utf8ToCodePoint(uint32_t unicode);

	uint32_t codePointToUtf16(uint32_t codePoint, Encoding encoding = Encoding::UTF16BE);

	uint32_t utf16ToCodePoint(uint32_t unicode, Encoding encoding = Encoding::UTF16BE);

	uint32_t utf16ToCodePoint(wchar_t firstPart, wchar_t secondPart, Encoding encoding = Encoding::UTF16BE);

	uint32_t codePointToUtf32(uint32_t codePoint, Encoding encoding = Encoding::UTF16BE);

	uint32_t utf32ToCodePoint(uint32_t unicode, Encoding encoding = Encoding::UTF16BE);

	uint32_t codePointToUnicode(uint32_t codePoint, Encoding encoding);

	uint32_t unicodeToCodePoint(uint32_t unicode, Encoding encoding);

	void serializeCodePointChar(std::ostream& dstStream, uint32_t codePoint, Encoding dstEncoding);

	void serializeCodePointChar(std::string& dstString, uint32_t codePoint, Encoding dstEncoding);

	void serializeCodePointChar(std::wostream& dstStream, uint32_t codePoint, Encoding dstEncoding);

	void serializeCodePointChar(std::wstring& dstString, uint32_t codePoint, Encoding dstEncoding);

	template<class TOStream = std::ostream, class TIStream = std::istream>
	void convertStream(TOStream& dstStream, TIStream& srcStream, Encoding dstEncoding, Encoding srcEncoding)
	{
		uint32_t codePoint = 0;
		while (true)
		{
			readChar(srcStream, codePoint, srcEncoding);
			if (codePoint == JSON_EOF) [[unlikely]]
			{
				return;
			}
			serializeCodePointChar(dstStream, codePoint, dstEncoding);
		}
	}

	void convert(std::ostream& dstStream, std::istream& srcStream, Encoding dstEncoding, Encoding srcEncoding);

	void convert(std::string& dstString, std::istream& srcStream, Encoding dstEncoding, Encoding srcEncoding);
	void convert(std::string& dstString, std::wistream& srcStream, Encoding dstEncoding, Encoding srcEncoding);
	void convert(std::wstring& dstString, std::istream& srcStream, Encoding dstEncoding, Encoding srcEncoding);
	void convert(std::wstring& dstString, std::wistream& srcStream, Encoding dstEncoding, Encoding srcEncoding);

	void convert(std::ostream& dstStream, const std::string& srcString, Encoding dstEncoding, Encoding srcEncoding);
	void convert(std::ostream& dstStream, const std::wstring& srcString, Encoding dstEncoding, Encoding srcEncoding);
	void convert(std::wostream& dstStream, const std::string& srcString, Encoding dstEncoding, Encoding srcEncoding);
	void convert(std::wostream& dstStream, const std::wstring& srcString, Encoding dstEncoding, Encoding srcEncoding);

	void convert(std::string& dstString, const std::string& srcString, Encoding dstEncoding, Encoding srcEncoding);
	void convert(std::string& dstString, const std::wstring& srcString, Encoding dstEncoding, Encoding srcEncoding);
	void convert(std::wstring& dstString, const std::string& srcString, Encoding dstEncoding, Encoding srcEncoding);
	void convert(std::wstring& dstString, const std::wstring& srcString, Encoding dstEncoding, Encoding srcEncoding);

	void readBOM(std::istream& srcStream, Encoding& srcEncoding);
	void writeBOM(std::ostream& dstStream, Encoding dstEncoding);

	// Read a character from the stream by using the specified encoding, modifying the codePoint, and returning the bytes read count
	int32_t readChar(std::istream& srcSstream, uint32_t& codePoint, Encoding srcEncoding);

	// Read a character from the stream by using the specified encoding, modifying the codePoint, and returning the bytes read count
	int32_t readChar(std::wistream& srcStream, uint32_t& codePoint, Encoding srcEncoding);

	// Read a character from the stream by skipping spaces/newlines characters, and by using the specified encoding, modifying the codePoint, and returning the bytes read count
	// 
	// Allowed JSON whitespace characters :
	// 0x20 <=> 0b00100000 : SP (SPace)
	// 0x09 <=> 0b00001001 : HT (Horizontal Tab)
	// 0x0D <=> 0b00001101 : CR (Carriage Return)
	// 0x0A <=> 0b00001010 : LF (Line Feed)
	template<class TIStream = std::istream>
	int32_t readCharNoSpaces(TIStream& srcStream, uint32_t& codePoint, Encoding srcEncoding)
	{
		int32_t size = 0;
		do
		{
			size = readChar(srcStream, codePoint, srcEncoding);
		} while (codePoint == 0x20 || ((codePoint & 0x80) && (codePoint == 0x09 || codePoint == 0x0D || codePoint == 0x0A)));

		return size;
	}
}
