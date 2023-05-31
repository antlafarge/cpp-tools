#pragma once

#include <cstdint>
#include <iostream>

#define JSON_DEBUG false

#define JSON_DEFAULT_ENCODING JSON::Encoding::UTF8

#define JSON_DEFAULT_ENCODING_WSTRING (Encoding::UTF16 | Encoding::BigEndian)

#define JSON_DEFAULT_WRITE_UTF8_BOM false

#define JSON_DEFAULT_WRITE_UTF16_BOM true

#if ((defined(__BIG_ENDIAN__) && __BIG_ENDIAN__ == 1) || (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)) || ((defined(REG_DWORD) && (REG_DWORD == REG_DWORD_BIG_ENDIAN))))
#	define JSON_PLATFORM_IS_BIG_ENDIAN 1
#else
#	define JSON_PLATFORM_IS_LITTLE_ENDIAN 1
#endif

#define JSON_READ_CHAR(stream, codePoint, encoding) readChar(stream, codePoint, encoding)

#define JSON_READ_CHAR_NO_SPACES(stream, codePoint, encoding) readCharNoSpaces(stream, codePoint, encoding)

#define JSON_UNGET(stream, size) for (int i = 0; i < size; i++) { stream.unget(); }

namespace JSON
{
	enum class Encoding : uint8_t
	{
		None = 0,
		UTF8 = 0x1,
		UTF16 = 0x2,
		UTF32 = 0x4,
		LittleEndian = 0x40,
		BigEndian = 0x80
	};

	Encoding operator|(Encoding lhs, Encoding rhs);

	Encoding operator&(Encoding lhs, Encoding rhs);

	uint32_t codePointToUtf8(uint32_t codePoint);

	uint32_t utf8ToCodePoint(uint32_t unicode);

	uint32_t codePointToUtf16(uint32_t codePoint, Encoding encoding = Encoding::BigEndian);

	uint32_t utf16ToCodePoint(uint32_t unicode, Encoding encoding = Encoding::BigEndian);

	uint32_t utf16ToCodePoint(wchar_t firstPart, wchar_t secondPart, Encoding encoding = Encoding::BigEndian);

	uint32_t codePointToUtf32(uint32_t codePoint, Encoding encoding = Encoding::BigEndian);

	uint32_t utf32ToCodePoint(uint32_t unicode, Encoding encoding = Encoding::BigEndian);

	uint32_t codePointToUnicode(uint32_t codePoint, Encoding encoding);

	uint32_t unicodeToCodePoint(uint32_t unicode, Encoding encoding);

	int readChar(std::istream& stream, int32_t& codePoint, Encoding& encoding);

	int readCharNoSpaces(std::istream& stream, int32_t& codePoint, Encoding& encoding);

	void serializeUnicodeChar(std::ostream& stream, int32_t unicode, Encoding encoding);

	void serializeUnicodeChar(std::string& dst, int32_t codePoint, Encoding encoding);

	void serializeUnicodeChar(std::wostream& stream, int32_t unicode, Encoding encoding);

	void serializeUnicodeChar(std::wstring& dst, int32_t codePoint, Encoding encoding);

	void utf16ToUtf8(std::ostream& dst, const std::wstring& src, Encoding srcEncoding = Encoding::UTF16 | Encoding::BigEndian);

	void utf16ToUtf8(std::string& dst, const std::wstring& src, Encoding srcEncoding = Encoding::UTF16 | Encoding::BigEndian);

	void utf8ToUtf16(std::wostream& dst, const std::string& src, Encoding dstEncoding = Encoding::UTF16 | Encoding::BigEndian);

	void utf8ToUtf16(std::wstring& dst, const std::string& src, Encoding dstEncoding = Encoding::UTF16 | Encoding::BigEndian);

	void utf8ToUtf16(std::ostream& dst, const std::string& src, Encoding dstEncoding = Encoding::UTF16 | Encoding::BigEndian);

	void utf8ToUtf16(std::string& dst, const std::string& src, Encoding dstEncoding = Encoding::UTF16 | Encoding::BigEndian);

	void writeBOM(std::ostream& stream, Encoding encoding);
}
