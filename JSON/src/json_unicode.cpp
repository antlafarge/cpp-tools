#include "json_unicode.h"

#include <cassert>
#include <iomanip>
#include <sstream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

namespace JSON
{
	Encoding operator|(Encoding lhs, Encoding rhs)
	{
		return static_cast<Encoding>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	Encoding operator&(Encoding lhs, Encoding rhs)
	{
		return static_cast<Encoding>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
	}

	bool hasFlags(Encoding encoding, Encoding flags, bool acceptUnknown)
	{
		return ((encoding & flags) == flags) || (acceptUnknown && encoding == Encoding::Unknown);
	}

	uint32_t codePointToUtf8(uint32_t codePoint)
	{
		if (codePoint < 0x80) [[likely]]
		{
			// The code point is equal to the unicode character
		}
		else if (codePoint < 0x800) [[unlikely]]
		{
			codePoint = 0xC080
				| ((codePoint & 0x7C0) << 2)
				| (codePoint & 0x3F);
		}
		else if (codePoint < 0x10000) [[unlikely]]
		{
			if ((codePoint & 0xFFFE) == 0xFFFE) [[unlikely]]
			{
				throw std::invalid_argument("utf8_codepoint_invalid");
			}
			else if ((codePoint & 0xF800) == 0xD800) [[unlikely]]
			{
				throw std::invalid_argument("utf8_codepoint_surrogates");
			}
				codePoint = 0xE08080
				| ((codePoint & 0xF000) << 4)
				| ((codePoint & 0xFC0) << 2)
				| (codePoint & 0x3F);
		}
		else if (codePoint < 0x110000) [[unlikely]]
		{
			codePoint = 0xF0808080
				| ((codePoint & 0x1C0000) << 6)
				| ((codePoint & 0x3F000) << 4)
				| ((codePoint & 0xFC0) << 2)
				| (codePoint & 0x3F);
		}
		else [[unlikely]]
		{
			throw std::out_of_range("utf8_codepoint_invalid");
		}
		return codePoint;
	}

	uint32_t utf8ToCodePoint(uint32_t unicode)
	{
		if (unicode < 0x80) [[likely]]
		{
			// The unicode character is equal to the code point
		}
		else if (unicode < 0x10000) [[unlikely]]
		{
			if ((unicode & 0xC080) != 0xC080) [[unlikely]]
			{
				throw std::invalid_argument("utf8_invalid");
			}
				unicode = ((unicode >> 2) & 0x7C0)
				| (unicode & 0x3F);
		}
		else if (unicode < 0x1000000) [[unlikely]]
		{
			if ((unicode & 0xE08080) != 0xE08080) [[unlikely]]
			{
				throw std::invalid_argument("utf8_invalid");
			}
				unicode = ((unicode >> 4) & 0xF000)
				| ((unicode >> 2) & 0xFC0)
				| (unicode & 0x3F);
		}
		else if (unicode <= 0xFFFFFFFF) [[unlikely]]
		{
			if ((unicode & 0xF0808080) != 0xF0808080) [[unlikely]]
			{
				throw std::invalid_argument("utf8_invalid");
			}
				unicode = ((unicode >> 6) & 0x1C0000)
				| ((unicode >> 4) & 0x3F000)
				| ((unicode >> 2) & 0xFC0)
				| (unicode & 0x3F);
		}
		return unicode;
	}

	uint32_t codePointToUtf16(uint32_t codePoint, Encoding encoding)
	{
		if (codePoint < 0x10000) [[likely]]
		{
			if ((codePoint & 0xF800) == 0xD800) [[unlikely]]
			{
				throw std::invalid_argument("utf16_codepoint_surrogates");
			}
			else if ((codePoint & 0xFFFE) == 0xFFFE) [[unlikely]]
			{
				throw std::invalid_argument("utf16_codepoint_invalid");
			}
			else if (hasFlags(encoding, Encoding::LittleEndian)) [[unlikely]]
			{
				codePoint = ((codePoint & 0xFF) << 8) | ((codePoint & 0xFF00) >> 8);
			}
				// else the code point is equal to the unicode character
		}
		else if (codePoint < 0x110000) [[unlikely]]
		{
			codePoint -= 0x10000;
			if (hasFlags(encoding, Encoding::LittleEndian)) [[unlikely]]
			{
				codePoint = 0x00D800DC
					| ((codePoint & 0xC0000) << 12)
					| ((codePoint & 0xFC00) << 14)
					| ((codePoint & 0xC0000) >> 2)
					| ((codePoint & 0x300) >> 8)
					| ((codePoint & 0xFF) << 8);
			}
			else [[likely]]
			{
				codePoint = 0xD800DC00
					| ((codePoint & 0xFFC00) << 6)
					| (codePoint & 0x3FF);
			}
		}
		else [[unlikely]]
		{
			throw std::out_of_range("utf16_codepoint_overflow");
		}
		return codePoint;
	}

	uint32_t utf16ToCodePoint(uint32_t unicode, Encoding encoding)
	{
		if (hasFlags(encoding, Encoding::LittleEndian)) [[unlikely]]
		{
			if (unicode < 0x10000) [[likely]]
			{
				unicode = ((unicode << 8) & 0xFF00) | ((unicode >> 8) & 0xFF);
			}
			else if ((unicode & 0x00D800DC) == 0x00D800DC) [[unlikely]]
			{
				unicode = 0x10000
					+ (((unicode >> 12) & 0xC0000)
						| ((unicode >> 14) & 0xFC00)
						| ((unicode << 2) & 0xC0000)
						| ((unicode << 8) & 0x300)
						| ((unicode >> 8) & 0xFF));
			}
			else [[unlikely]]
			{
				throw std::out_of_range("utf16");
			}
		}
		else [[likely]]
		{
			if (unicode < 0x10000) [[likely]]
			{
				// The unicode character is equal to the code point
			}
			else if ((unicode & 0xD800DC00) == 0xD800DC00) [[unlikely]]
			{
				unicode = 0x10000
					+ (((unicode >> 6) & 0xFFC00)
						| (unicode & 0x3FF));
			}
			else [[unlikely]]
			{
				throw std::out_of_range("utf16");
			}
		}
		return unicode;
	}

	uint32_t utf16ToCodePoint(wchar_t firstPart, wchar_t secondPart, Encoding encoding)
	{
		uint32_t unicode;
		if (hasFlags(encoding, Encoding::LittleEndian)) [[likely]]
		{
			unicode = (static_cast<uint32_t>(firstPart) << 16) | static_cast<uint32_t>(secondPart);
		}
		else [[unlikely]]
		{
			unicode = static_cast<uint32_t>(firstPart) | (static_cast<uint32_t>(secondPart) << 16);
		}
		return utf16ToCodePoint(unicode, encoding);
	}

	uint32_t codePointToUtf32(uint32_t codePoint, Encoding encoding)
	{
		if (hasFlags(encoding, Encoding::LittleEndian)) [[likely]]
		{
			if ((codePoint & 0xFFFE) == 0xFFFE) [[unlikely]]
			{
				throw std::invalid_argument("utf32_codepoint");
			}
			else if (codePoint >= 0x110000) [[unlikely]]
			{
				throw std::out_of_range("utf32_codepoint");
			}
			codePoint = ((codePoint & 0xFF000000) >> 24) | ((codePoint & 0xFF0000) >> 8) | ((codePoint & 0xFF00) << 8) | ((codePoint & 0xFF) << 24);
		}
		else [[unlikely]]
		{
			if ((codePoint & 0xFFFE) == 0xFFFE) [[unlikely]]
			{
				throw std::invalid_argument("utf32_codepoint");
			}
			else if (codePoint >= 0x110000) [[unlikely]]
			{
				throw std::out_of_range("utf32_codepoint");
			}
		}
		return codePoint;
	}

	uint32_t utf32ToCodePoint(uint32_t unicode, Encoding encoding)
	{
		if (hasFlags(encoding, Encoding::LittleEndian)) [[likely]]
		{
			unicode = ((unicode << 24) & 0xFF000000) | ((unicode << 8) & 0xFF0000) | ((unicode >> 8) & 0xFF00) | ((unicode >> 24) & 0xFF);
			if ((unicode & 0xFFFE) == 0xFFFE) [[unlikely]]
			{
				throw std::invalid_argument("utf32_codepoint");
			}
			else if (unicode >= 0x110000) [[unlikely]]
			{
				throw std::out_of_range("utf32_codepoint");
			}
		}
		else [[unlikely]]
		{
			if ((unicode & 0xFFFE) == 0xFFFE) [[unlikely]]
			{
				throw std::invalid_argument("utf32_codepoint");
			}
			else if (unicode >= 0x110000) [[unlikely]]
			{
				throw std::out_of_range("utf32_codepoint");
			}
		}
		return unicode;
	}

	uint32_t codePointToUnicode(uint32_t codePoint, Encoding encoding)
	{
		if (encoding <= Encoding::UTF8) [[likely]]
		{
			return codePointToUtf8(codePoint);
		}
		else if (hasFlags(encoding, Encoding::UTF16)) [[unlikely]]
		{
			return codePointToUtf16(codePoint, encoding);
		}
		else if (hasFlags(encoding, Encoding::UTF32)) [[unlikely]]
		{
			return codePointToUtf32(codePoint, encoding);
		}
		return codePoint;
	}

	uint32_t unicodeToCodePoint(uint32_t unicode, Encoding encoding)
	{
		if (encoding <= Encoding::UTF8) [[likely]]
		{
			return utf8ToCodePoint(unicode);
		}
		else if (hasFlags(encoding, Encoding::UTF16)) [[unlikely]]
		{
			return utf16ToCodePoint(unicode, encoding);
		}
		else if (hasFlags(encoding, Encoding::UTF32)) [[unlikely]]
		{
			return utf32ToCodePoint(unicode, encoding);
		}
		return unicode;
	}

	void serializeCodePointChar(std::ostream& dstStream, uint32_t codePoint, Encoding dstEncoding)
	{
#if defined(JSON_DEBUG) && JSON_DEBUG != false
		std::cout << std::hex << std::uppercase << std::setfill('0');
#endif

		uint32_t unicode = codePointToUnicode(codePoint, dstEncoding);

		if (dstEncoding <= Encoding::UTF8 && unicode < 0x80) [[likely]]
		{
			dstStream << static_cast<uint8_t>(unicode);
			return;
		}

		uint8_t* byteArray = (uint8_t*)&unicode;
		uint8_t byte = 0;
		bool started = hasFlags(dstEncoding, Encoding::UTF32);
#if defined(JSON_PLATFORM_IS_BIG_ENDIAN)
		static const int32_t utf8last = 3;
		static const int32_t utf16last = 2;
		for (int32_t i = 0; i < 4; i++)
#else
		static const int32_t utf8last = 0;
		static const int32_t utf16last = 1;
		for (int32_t i = 3; i >= 0; i--)
#endif
		{
			byte = byteArray[i];
			if (!started) [[likely]]
			{
				if (byte) [[unlikely]]
				{
					started = true;
				}
				else if (dstEncoding <= Encoding::UTF8) [[likely]]
				{
					if (i == utf8last) [[unlikely]]
					{
						started = true;
					}
				}
				else if (hasFlags(dstEncoding, Encoding::UTF16)) [[unlikely]]
				{
					if (i == utf16last) [[unlikely]]
					{
						started = true;
					}
				}
				else [[unlikely]]
				{
					throw std::invalid_argument("serializeunicodechar_badencoding");
				}
			}
				if (started) [[unlikely]]
				{
#if defined(JSON_DEBUG) && JSON_DEBUG != false
					std::cout << std::setw(2) << static_cast<uint16_t>(byte) << ' ';
#endif
					dstStream << byte;
				}
		}
	}

	void serializeCodePointChar(std::string& dstString, uint32_t codePoint, Encoding dstEncoding)
	{
		std::ostringstream dstStream;
		serializeCodePointChar(dstStream, codePoint, dstEncoding);
		dstString += dstStream.str();
	}

	void serializeCodePointChar(std::wostream& dstStream, uint32_t codePoint, Encoding dstEncoding)
	{
#if defined(JSON_DEBUG) && JSON_DEBUG != false
		std::cout << std::hex << std::uppercase << std::setfill('0');
#endif

		if (dstEncoding == Encoding::Unknown) [[unlikely]]
		{
			dstEncoding = JSON_DEFAULT_ENCODING_WSTRING;
		}

		uint32_t unicode = codePointToUnicode(codePoint, dstEncoding);

		if (dstEncoding == Encoding::UTF8) [[unlikely]]
		{
			assert(false); // you should not store an UTF8 string in a wstring/wstream

			uint8_t* byteArray = (uint8_t*)&codePoint;
			bool started = false;
#if defined(JSON_PLATFORM_IS_BIG_ENDIAN)
			const int32_t start = 0, end = 4, increment = 1;
#else
			const int32_t start = 3, end = -1, increment = -1;
#endif
			for (int32_t i = start; i != end; i += increment)
			{
				wchar_t wc = (wchar_t)byteArray[i];
				if (wc || started || i == (end - increment)) [[unlikely]]
				{
#if defined(JSON_DEBUG) && JSON_DEBUG != false
					std::cout << std::setw(2) << static_cast<uint16_t>(wc & 0xFF) << '_';
					std::cout << std::setw(2) << static_cast<uint16_t>((wc & 0xFF00) >> 8) << ' ';
#endif
					dstStream << wc;
					started = true;
				}
			}
		}
		else if (hasFlags(dstEncoding, Encoding::UTF32)) [[unlikely]]
		{
			assert(false); // you should not store an UTF32 string in a wstring/wstream

#if defined(JSON_PLATFORM_IS_BIG_ENDIAN)
			const int32_t start = 0, end = 2, increment = 1;
#else
			const int32_t start = 1, end = -1, increment = -1;
#endif
			for (int32_t i = start; i != end; i += increment)
			{
				wchar_t wc = ((uint16_t*)&unicode)[i];

#if defined(JSON_DEBUG) && JSON_DEBUG != false
				std::cout << std::setw(2) << static_cast<uint16_t>(wc & 0xFF) << '_';
				std::cout << std::setw(2) << static_cast<uint16_t>((wc & 0xFF00) >> 8) << ' ';
#endif
				dstStream << wc;
			}
		}
		// Default UTF16BE
		else [[likely]]
		{
#if defined(JSON_PLATFORM_IS_BIG_ENDIAN)
			const int32_t start = 0, end = 2, increment = 1;
#else
			const int32_t start = 1, end = -1, increment = -1;
#endif
			for (int32_t i = start; i != end; i += increment)
			{
				wchar_t wc = ((uint16_t*)&unicode)[i];

				if (i == (end - increment) || wc) [[unlikely]]
				{
#if defined(JSON_DEBUG) && JSON_DEBUG != false
					std::cout << std::setw(2) << static_cast<uint16_t>(wc & 0xFF) << '_';
					std::cout << std::setw(2) << static_cast<uint16_t>((wc & 0xFF00) >> 8) << ' ';
#endif
					dstStream << wc;
				}
			}
		}
	}

	void serializeCodePointChar(std::wstring& dstString, uint32_t codePoint, Encoding dstEncoding)
	{
		std::wostringstream stream;
		serializeCodePointChar(stream, codePoint, dstEncoding);
		dstString += stream.str();
	}

	void convert(std::ostream& dstStream, std::istream& srcStream, Encoding dstEncoding, Encoding srcEncoding)
	{
		if (srcEncoding == dstEncoding) [[likely]]
		{
#if defined(JSON_COPY_BUFFER_ON_HEAP) && JSON_COPY_BUFFER_ON_HEAP
			char* buffer = new char[JSON_COPY_BUFFER_SIZE];
#else
			char buffer[JSON_COPY_BUFFER_SIZE];
#endif
			std::streamsize bytesReadCount;
			while (true)
			{
				bytesReadCount = srcStream.readsome(static_cast<char*>(buffer), JSON_COPY_BUFFER_SIZE);
				if (bytesReadCount <= 0)
				{
					break;
				}
				dstStream.write(buffer, bytesReadCount);
			}
#if defined(JSON_COPY_BUFFER_ON_HEAP) && JSON_COPY_BUFFER_ON_HEAP
			delete[] buffer;
#endif
		}
		else
		{
			convertStream(dstStream, srcStream, dstEncoding, srcEncoding);
		}
	}

	void convert(std::ostream& dstStream, std::wistream& srcStream, Encoding dstEncoding, Encoding srcEncoding)
	{
		convertStream(dstStream, srcStream, dstEncoding, srcEncoding);
	}

	void convert(std::wostream& dstStream, std::istream& srcStream, Encoding dstEncoding, Encoding srcEncoding)
	{
		convertStream(dstStream, srcStream, dstEncoding, srcEncoding);
	}

	void convert(std::wostream& dstStream, std::wistream& srcStream, Encoding dstEncoding, Encoding srcEncoding)
	{
		convertStream(dstStream, srcStream, dstEncoding, srcEncoding);
	}

	void convert(std::string& dstString, std::istream& srcStream, Encoding dstEncoding, Encoding srcEncoding)
	{
		std::ostringstream dstStream;
		convert(dstStream, srcStream, dstEncoding, srcEncoding);
		dstString = dstStream.str();
	}

	void convert(std::string& dstString, std::wistream& srcStream, Encoding dstEncoding, Encoding srcEncoding)
	{
		std::ostringstream dstStream;
		convert(dstStream, srcStream, dstEncoding, srcEncoding);
		dstString = dstStream.str();
	}

	void convert(std::wstring& dstString, std::istream& srcStream, Encoding dstEncoding, Encoding srcEncoding)
	{
		std::wostringstream dstStream;
		convert(dstStream, srcStream, dstEncoding, srcEncoding);
		dstString = dstStream.str();
	}

	void convert(std::wstring& dstString, std::wistream& srcStream, Encoding dstEncoding, Encoding srcEncoding)
	{
		std::wostringstream dstStream;
		convert(dstStream, srcStream, dstEncoding, srcEncoding);
		dstString = dstStream.str();
	}

	void convert(std::ostream& dstStream, const std::string& srcString, Encoding dstEncoding, Encoding srcEncoding)
	{
		std::istringstream srcStream(srcString);
		convert(dstStream, srcStream, dstEncoding, srcEncoding);
	}

	void convert(std::ostream& dstStream, const std::wstring& srcString, Encoding dstEncoding, Encoding srcEncoding)
	{
		std::wistringstream srcStream(srcString);
		convert(dstStream, srcStream, dstEncoding, srcEncoding);
	}

	void convert(std::wostream& dstStream, const std::string& srcString, Encoding dstEncoding, Encoding srcEncoding)
	{
		std::istringstream srcStream(srcString);
		convert(dstStream, srcStream, dstEncoding, srcEncoding);
	}

	void convert(std::wostream& dstStream, const std::wstring& srcString, Encoding dstEncoding, Encoding srcEncoding)
	{
		std::wistringstream srcStream(srcString);
		convert(dstStream, srcStream, dstEncoding, srcEncoding);
	}

	void convert(std::string& dstString, const std::string& srcString, Encoding dstEncoding, Encoding srcEncoding)
	{
		if (dstEncoding == srcEncoding) [[likely]]
		{
			dstString = srcString;
			return;
		}

		std::ostringstream dstStream;
		std::istringstream srcStream(srcString);
		convert(dstStream, srcStream, dstEncoding, srcEncoding);
		dstString = dstStream.str();
	}

	void convert(std::string& dstString, const std::wstring& srcString, Encoding dstEncoding, Encoding srcEncoding)
	{
		std::ostringstream dstStream;
		std::wistringstream srcStream(srcString);
		convert(dstStream, srcStream, dstEncoding, srcEncoding);
		dstString = dstStream.str();
	}

	void convert(std::wstring& dstString, const std::string& srcString, Encoding dstEncoding, Encoding srcEncoding)
	{
		std::wostringstream dstStream;
		std::istringstream srcStream(srcString);
		convert(dstStream, srcStream, dstEncoding, srcEncoding);
		dstString = dstStream.str();
	}

	void convert(std::wstring& dstString, const std::wstring& srcString, Encoding dstEncoding, Encoding srcEncoding)
	{
		if (dstEncoding == srcEncoding) [[likely]]
		{
			dstString = srcString;
			return;
		}

		std::wostringstream dstStream;
		std::wistringstream srcStream(srcString);
		convert(dstStream, srcStream, dstEncoding, srcEncoding);
		dstString = dstStream.str();
	}

	void readBOM(std::istream& srcStream, Encoding& encoding)
	{
		encoding = JSON_DEFAULT_ENCODING;
		int32_t size = 1;
		switch (srcStream.get())
		{
		case 0x00: [[unlikely]]
			size++;
			if (srcStream.get() == 0x00) [[unlikely]]
			{
				size++;
				if (srcStream.get() == 0xFE) [[unlikely]]
				{
					size++;
					if (srcStream.get() == 0xFF) [[unlikely]]
					{
						encoding = Encoding::UTF32BE;
						size = 0;
						break;
					}
				}
			}
			break;
		case 0xEF: [[unlikely]]
			size++;
			if (srcStream.get() == 0xBB) [[unlikely]]
			{
				size++;
				if (srcStream.get() == 0xBF) [[unlikely]]
				{
					encoding = Encoding::UTF8;
					size = 0;
					break;
				}
			}
			break;
		case 0xFE: [[unlikely]]
			size++;
			if (srcStream.get() == 0xFF) [[likely]]
			{
				encoding = Encoding::UTF16BE;
				size = 0;
				break;
			}
			break;
		case 0xFF: [[unlikely]]
			size++;
			if (srcStream.get() == 0xFE) [[unlikely]]
			{
				size = 1;
				if (srcStream.get() == 0x00) [[unlikely]]
				{
					size++;
					if (srcStream.get() == 0x00) [[unlikely]]
					{
						encoding = Encoding::UTF32LE;
						size = 0;
						break;
					}
				}
				encoding = Encoding::UTF16LE;
				break;
			}
			break;
		default: [[likely]]
			break;
		}

		JSON_UNGET(srcStream, size);
	}

	void writeBOM(std::ostream& dstStream, Encoding encoding)
	{
#if defined(JSON_DEFAULT_WRITE_UTF8_BOM) && JSON_DEFAULT_WRITE_UTF8_BOM
		if (encoding <= Encoding::UTF8) [[likely]]
		{
			stream << uint8_t(0xEF) << uint8_t(0xBB) << uint8_t(0xBF);
		}
#endif
#if defined(JSON_DEFAULT_WRITE_UTF16_BOM) && JSON_DEFAULT_WRITE_UTF16_BOM
			if (hasFlags(encoding, Encoding::UTF16)) [[unlikely]]
			{
				if (hasFlags(encoding, Encoding::LittleEndian)) [[unlikely]]
				{
					dstStream << uint8_t(0xFF) << uint8_t(0xFE);
				}
				else [[likely]]
				{
					dstStream << uint8_t(0xFE) << uint8_t(0xFF);
				}
			}
#endif
	}

	int32_t readChar(std::istream& srcStream, uint32_t& codePoint, Encoding encoding)
	{
		int32_t size = 0;
		uint32_t& unicode = codePoint = 0;
		size = 0;
		if (encoding <= Encoding::UTF8) [[likely]]
		{
			uint32_t b = unicode = static_cast<uint32_t>(srcStream.get());
			if ((b & 0x80) == 0) [[likely]]
			{
				size = 1;
			}
			else if ((b & 0xE0) == 0xC0) [[unlikely]]
			{
				size = 2;
			}
			else if ((b & 0xF0) == 0xE0) [[unlikely]]
			{
				size = 3;
			}
			else if ((b & 0xF8) == 0xF0) [[unlikely]]
			{
				size = 4;
			}
			else [[unlikely]]
			{
				size = 1;
			}
				if (size > 1) [[unlikely]]
				{
					for (int32_t i = 0; i < (size - 1); i++)
					{
						b = static_cast<uint32_t>(srcStream.get());
						unicode = (unicode << 8) | b;
					}
				}
					if (b == JSON_EOF) [[unlikely]]
					{
						unicode = JSON_EOF;
						return 0;
					}
				codePoint = utf8ToCodePoint(unicode);
		}
		else if (hasFlags(encoding, Encoding::UTF16)) [[unlikely]]
		{
			uint32_t b1, b2;
			for (int32_t i = 0; i < 2; i++)
			{
				size += 2;
				b1 = static_cast<uint32_t>(srcStream.get());
				b2 = static_cast<uint32_t>(srcStream.get());
				if (b2 == JSON_EOF) [[unlikely]]
				{
					codePoint = JSON_EOF;
					return 0;
				}
				unicode = (unicode << (i * 16)) | ((b1 << 8) | b2);
				if (hasFlags(encoding, Encoding::LittleEndian)) [[likely]]
				{
					if ((unicode & 0x00F8) != 0x00D8) [[likely]]
					{
						break;
					}
				}
				else if (hasFlags(encoding, Encoding::BigEndian)) [[unlikely]]
				{
					if ((unicode & 0xF800) != 0xD800) [[likely]]
					{
						break;
					}
				}
			}
			codePoint = utf16ToCodePoint(unicode, encoding);
		}
		else if (hasFlags(encoding, Encoding::UTF32)) [[unlikely]]
		{
			size = 4;
			uint32_t b1 = static_cast<uint32_t>(srcStream.get());
			if (b1 == JSON_EOF) [[unlikely]]
			{
				codePoint = JSON_EOF;
				return 0;
			}
			uint32_t b2 = static_cast<uint32_t>(srcStream.get());
			if (b2 == JSON_EOF) [[unlikely]]
			{
				codePoint = JSON_EOF;
				return 1;
			}
			uint32_t b3 = static_cast<uint32_t>(srcStream.get());
			if (b3 == JSON_EOF) [[unlikely]]
			{
				codePoint = JSON_EOF;
				return 2;
			}
			uint32_t b4 = static_cast<uint32_t>(srcStream.get());
			if (b4 == JSON_EOF) [[unlikely]]
			{
				codePoint = JSON_EOF;
				return 3;
			}
			unicode = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
			codePoint = utf32ToCodePoint(unicode, encoding);
		}
		return size;
	}

	int32_t readChar(std::wistream& srcStream, uint32_t& codePoint, Encoding srcEncoding)
	{
		int32_t size = 0;
		uint32_t& unicode = codePoint = 0;
		size = 0;
		if (hasFlags(srcEncoding, Encoding::UTF16)) [[likely]]
		{
			uint16_t b;
			for (int32_t i = 0; i < 1; i++)
			{
				size++;
				b = srcStream.get();
				if (b == WEOF) [[unlikely]]
				{
					codePoint = JSON_EOF;
					return 0;
				}
				unicode = (unicode << (i * 16)) | b;
				if (hasFlags(srcEncoding, Encoding::LittleEndian)) [[likely]]
				{
					if ((unicode & 0x00F8) != 0x00D8) [[likely]]
					{
						break;
					}
				}
				else if (hasFlags(srcEncoding, Encoding::BigEndian)) [[unlikely]]
				{
					if ((unicode & 0xF800) != 0xD800) [[likely]]
					{
						break;
					}
				}
			}
			codePoint = utf16ToCodePoint(unicode, srcEncoding);
		}
		else if (hasFlags(srcEncoding, Encoding::UTF32)) [[unlikely]]
		{
			size = 4;
			uint16_t b1 = srcStream.get();
			if (b1 == JSON_EOF) [[unlikely]]
			{
				codePoint = JSON_EOF;
				return 0;
			}
			uint32_t b2 = srcStream.get();
			if (b2 == JSON_EOF) [[unlikely]]
			{
				codePoint = JSON_EOF;
				return 1;
			}
			uint32_t b3 = srcStream.get();
			if (b3 == JSON_EOF) [[unlikely]]
			{
				codePoint = JSON_EOF;
				return 2;
			}
			uint32_t b4 = srcStream.get();
			if (b4 == JSON_EOF) [[unlikely]]
			{
				codePoint = JSON_EOF;
				return 3;
			}
			unicode = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
			codePoint = utf32ToCodePoint(unicode, srcEncoding);
		}
		else [[unlikely]]
		{
			throw std::runtime_error("readchar_wistream_badencoding");
		}
		return size;
	}
}

#pragma GCC diagnostic pop
