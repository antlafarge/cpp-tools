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
			else if ((encoding & Encoding::LittleEndian) == Encoding::LittleEndian) [[unlikely]]
			{
				codePoint = ((codePoint & 0xFF) << 8) | ((codePoint & 0xFF00) >> 8);
			}
				// else the code point is equal to the unicode character
		}
		else if (codePoint < 0x110000) [[unlikely]]
		{
			codePoint -= 0x10000;
			if ((encoding & Encoding::LittleEndian) == Encoding::LittleEndian) [[unlikely]]
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
		if ((encoding & Encoding::LittleEndian) == Encoding::LittleEndian) [[unlikely]]
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
		if ((encoding & Encoding::LittleEndian) == Encoding::LittleEndian) [[likely]]
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
		if ((encoding & Encoding::LittleEndian) == Encoding::LittleEndian) [[likely]]
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
		if ((encoding & Encoding::LittleEndian) == Encoding::LittleEndian) [[likely]]
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
		if (encoding == Encoding::None) [[unlikely]]
		{
			encoding = JSON_DEFAULT_ENCODING;
		}

			if ((encoding & Encoding::UTF8) == Encoding::UTF8) [[likely]]
			{
				return codePointToUtf8(codePoint);
			}
			else if ((encoding & Encoding::UTF16) == Encoding::UTF16) [[unlikely]]
			{
				return codePointToUtf16(codePoint, encoding);
			}
			else if ((encoding & Encoding::UTF32) == Encoding::UTF32) [[unlikely]]
			{
				return codePointToUtf32(codePoint, encoding);
			}
		return codePoint;
	}

	uint32_t unicodeToCodePoint(uint32_t unicode, Encoding encoding)
	{
		if (encoding == Encoding::None) [[unlikely]]
		{
			encoding = JSON_DEFAULT_ENCODING;
		}

			if ((encoding & Encoding::UTF8) == Encoding::UTF8) [[likely]]
			{
				return utf8ToCodePoint(unicode);
			}
			else if ((encoding & Encoding::UTF16) == Encoding::UTF16) [[unlikely]]
			{
				return utf16ToCodePoint(unicode, encoding);
			}
			else if ((encoding & Encoding::UTF32) == Encoding::UTF32) [[unlikely]]
			{
				return utf32ToCodePoint(unicode, encoding);
			}
		return unicode;
	}

	int readChar(std::istream& stream, int32_t& codePoint, Encoding& encoding)
	{
		int size = 0;

		if (encoding == Encoding::None) [[unlikely]]
		{
			size++;
			switch (stream.get())
			{
			case 0x00: [[unlikely]]
				size++;
				if (stream.get() == 0x00) [[unlikely]]
				{
					size++;
					if (stream.get() == 0xFE) [[unlikely]]
					{
						size++;
						if (stream.get() == 0xFF) [[unlikely]]
						{
							encoding = Encoding::UTF32 | Encoding::BigEndian;
							break;
						}
					}
				}
				encoding = JSON_DEFAULT_ENCODING;
				JSON_UNGET(stream, size);
				break;
			case 0xFE: [[unlikely]]
				size++;
				if (stream.get() == 0xFF) [[likely]]
				{
					encoding = Encoding::UTF16 | Encoding::BigEndian;
					break;
				}
				encoding = JSON_DEFAULT_ENCODING;
				JSON_UNGET(stream, size);
				break;
			case 0xFF: [[unlikely]]
				size++;
				if (stream.get() == 0xFE) [[unlikely]]
				{
					size = 1;
					if (stream.get() == 0x00) [[unlikely]]
					{
						size++;
						if (stream.get() == 0x00) [[unlikely]]
						{
							encoding = Encoding::UTF32 | Encoding::LittleEndian;
							break;
						}
					}
					encoding = Encoding::UTF16 | Encoding::LittleEndian;
					JSON_UNGET(stream, size);
					break;
				}
				encoding = JSON_DEFAULT_ENCODING;
				JSON_UNGET(stream, size);
				break;
			case 0xEF: [[unlikely]]
				size++;
				if (stream.get() == 0xBB) [[unlikely]]
				{
					size++;
					if (stream.get() == 0xBF) [[unlikely]]
					{
						encoding = Encoding::UTF8;
						break;
					}
				}
				encoding = JSON_DEFAULT_ENCODING;
				JSON_UNGET(stream, size);
				break;
			default: [[likely]]
				JSON_UNGET(stream, size);
				encoding = JSON_DEFAULT_ENCODING;
				break;
			}
		}

		int32_t& unicode = codePoint = 0;
		size = 0;
		if ((encoding & Encoding::UTF8) == Encoding::UTF8) [[likely]]
		{
			int32_t b = 0;
			unicode = b = stream.get();
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
					for (int i = 0; i < (size - 1); i++)
					{
						b = stream.get();
						unicode = (unicode << 8) | b;
					}
				}
					if (b == EOF) [[unlikely]]
					{
						unicode = EOF;
						return 0;
					}
				codePoint = utf8ToCodePoint(unicode);
		}
		else if ((encoding & Encoding::UTF16) == Encoding::UTF16) [[unlikely]]
		{
			int32_t b1, b2;
			for (int i = 0; i < 2; i++)
			{
				size += 2;
				b1 = stream.get();
				b2 = stream.get();
				if (b2 == EOF) [[unlikely]]
				{
					codePoint = EOF;
					return 0;
				}
				unicode = (unicode << (i * 16)) | ((b1 << 8) | b2);
				if ((encoding & Encoding::LittleEndian) == Encoding::LittleEndian) [[likely]]
				{
					if ((unicode & 0x00F8) != 0x00D8) [[likely]]
					{
						break;
					}
				}
				else if ((encoding & Encoding::BigEndian) == Encoding::BigEndian) [[unlikely]]
				{
					if ((unicode & 0xF800) != 0xD800) [[likely]]
					{
						break;
					}
				}
			}
			codePoint = utf16ToCodePoint(unicode, encoding);
		}
		else if ((encoding & Encoding::UTF32) == Encoding::UTF32) [[unlikely]]
		{
			size = 4;
			int32_t b1 = stream.get();
			if (b1 == EOF) [[unlikely]]
			{
				codePoint = EOF;
				return 0;
			}
			int32_t b2 = stream.get();
			if (b2 == EOF) [[unlikely]]
			{
				codePoint = EOF;
				return 1;
			}
			int32_t b3 = stream.get();
			if (b3 == EOF) [[unlikely]]
			{
				codePoint = EOF;
				return 2;
			}
			int32_t b4 = stream.get();
			if (b4 == EOF) [[unlikely]]
			{
				codePoint = EOF;
				return 3;
			}
			unicode = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
			codePoint = utf32ToCodePoint(unicode, encoding);
		}
		return size;
	}

	int readCharNoSpaces(std::istream& stream, int32_t& codePoint, Encoding& encoding)
	{
		int size = 0;
		do
		{
			size = readChar(stream, codePoint, encoding);
		} while (codePoint == 0x20 || codePoint == 0x9 || codePoint == 0xA || codePoint == 0xD);
		return size;
	}

	void serializeUnicodeChar(std::ostream& stream, int32_t unicode, Encoding encoding, bool debug)
	{
		if (debug)
		{
			stream << std::hex << std::uppercase << std::setfill('0');
		}
		uint8_t* byteArray = (uint8_t*)&unicode;
		uint8_t byte = 0;
		bool started = ((encoding & Encoding::UTF32) == Encoding::UTF32);
#if defined(JSON_PLATFORM_IS_BIG_ENDIAN)
		static const int utf8last = 3;
		static const int utf16last = 2;
		for (int i = 0; i < 4; i++)
#else
		static const int utf8last = 0;
		static const int utf16last = 1;
		for (int i = 3; i >= 0; i--)
#endif
		{
			byte = byteArray[i];
			if (!started) [[likely]]
			{
				if (byte) [[unlikely]]
				{
					started = true;
				}
				else if ((encoding & Encoding::UTF8) == Encoding::UTF8) [[unlikely]]
				{
					if (i == utf8last) [[unlikely]]
					{
						started = true;
					}
				}
				else if ((encoding & Encoding::UTF16) == Encoding::UTF16) [[unlikely]]
				{
					if (i == utf16last) [[unlikely]]
					{
						started = true;
					}
				}
			}
				if (started) [[unlikely]]
				{
					if (debug) [[unlikely]]
					{
						stream << std::setw(2) << static_cast<uint16_t>(byte) << ' ';
					}
					else [[likely]]
					{
						stream << byte;
					}
				}
		}
	}

	void serializeUnicodeChar(std::string& dst, int32_t codePoint, Encoding encoding, bool debug)
	{
		std::ostringstream stream;
		serializeUnicodeChar(stream, codePoint, encoding, debug);
		dst += stream.str();
	}

	void serializeUnicodeChar(std::wostream& stream, int32_t unicode, Encoding encoding, bool debug)
	{
		if (debug) [[unlikely]]
		{
			stream << std::hex << std::uppercase << std::setfill(L'0');
		}

			if ((encoding & Encoding::UTF8) == Encoding::UTF8) [[unlikely]]
			{
				assert(false); // you should not store an UTF8 string in a wstring/wstream

				uint8_t* byteArray = (uint8_t*)&unicode;
				bool started = false;
#if defined(JSON_PLATFORM_IS_BIG_ENDIAN)
				const int start = 0, end = 4, increment = 1;
#else
				const int start = 3, end = -1, increment = -1;
#endif
				for (int i = start; i != end; i += increment)
				{
					wchar_t wc = (wchar_t)byteArray[i];
					if (wc || started || i == (end - increment)) [[unlikely]]
					{
						if (debug) [[unlikely]]
						{
							stream << std::setw(2) << static_cast<uint16_t>(wc & 0xFF) << '_';
							stream << std::setw(2) << static_cast<uint16_t>((wc & 0xFF00) >> 8) << ' ';
						}
						else [[likely]]
						{
							stream << wc;
						}
						started = true;
					}
				}
			}
			else if ((encoding & Encoding::UTF16) == Encoding::UTF16) [[likely]]
			{
#if defined(JSON_PLATFORM_IS_BIG_ENDIAN)
				const int start = 0, end = 2, increment = 1;
#else
				const int start = 1, end = -1, increment = -1;
#endif
				for (int i = start; i != end; i += increment)
				{
					wchar_t wc = ((uint16_t*)&unicode)[i];

					if (i == (end - increment) || wc) [[unlikely]]
					{
						if (debug) [[unlikely]]
						{
							stream << std::setw(2) << static_cast<uint16_t>(wc & 0xFF) << '_';
							stream << std::setw(2) << static_cast<uint16_t>((wc & 0xFF00) >> 8) << ' ';
						}
						else [[likely]]
						{
							stream << wc;
						}
					}
				}
			}
			else if ((encoding & Encoding::UTF32) == Encoding::UTF32) [[unlikely]]
			{
				assert(false); // you should not store an UTF32 string in a wstring/wstream

#if defined(JSON_PLATFORM_IS_BIG_ENDIAN)
				const int start = 0, end = 2, increment = 1;
#else
				const int start = 1, end = -1, increment = -1;
#endif
				for (int i = start; i != end; i += increment)
				{
					wchar_t wc = ((uint16_t*)&unicode)[i];

					if (debug) [[unlikely]]
					{
						stream << std::setw(2) << static_cast<uint16_t>(wc & 0xFF) << '_';
						stream << std::setw(2) << static_cast<uint16_t>((wc & 0xFF00) >> 8) << ' ';
					}
					else [[likely]]
					{
						stream << wc;
					}
				}
			}
	}

	void serializeUnicodeChar(std::wstring& dst, int32_t codePoint, Encoding encoding, bool debug)
	{
		std::wostringstream stream;
		serializeUnicodeChar(stream, codePoint, encoding, debug);
		dst += stream.str();
	}

	void utf16ToUtf8(std::ostream& dst, const std::wstring& src, Encoding srcEncoding)
	{
		wchar_t wcFirstPart = 0;
		for (wchar_t wc : src)
		{
			if (wcFirstPart != 0) [[unlikely]] // 2x UTF-16 (part 2)
			{
				serializeUnicodeChar(dst, codePointToUtf8(utf16ToCodePoint(wcFirstPart, wc, srcEncoding)), Encoding::UTF8);
				wcFirstPart = 0;
			}
			else if (wc > 0xD800) [[unlikely]] // 2x UTF-16 (part 1)
			{
				wcFirstPart = wc;
			}
			else [[likely]] // 1x UTF-16
			{
				serializeUnicodeChar(dst, codePointToUtf8(utf16ToCodePoint(wc, srcEncoding)), Encoding::UTF8);
			}
		}
	}

	void utf16ToUtf8(std::string& dst, const std::wstring& src, Encoding srcEncoding)
	{
		std::ostringstream stream;
		utf16ToUtf8(stream, src, srcEncoding);
		dst = stream.str();
	}

	void utf8ToUtf16(std::wostream& dst, const std::string& src, Encoding dstEncoding)
	{
		std::istringstream srcStream(src);
		int32_t codePoint = 0;
		Encoding encoding = Encoding::UTF8;
		while (true)
		{
			JSON_READ_CHAR_NO_SPACES(srcStream, codePoint, encoding);
			if (codePoint == EOF)
			{
				return;
			}
			serializeUnicodeChar(dst, codePointToUtf16(utf8ToCodePoint(codePoint), dstEncoding), dstEncoding);
		}
	}

	void utf8ToUtf16(std::wstring& dst, const std::string& src, Encoding dstEncoding)
	{
		std::wostringstream stream;
		utf8ToUtf16(stream, src, dstEncoding);
		dst = stream.str();
	}

	void utf8ToUtf16(std::ostream& dst, const std::string& src, Encoding dstEncoding)
	{
		std::istringstream srcStream(src);
		int32_t codePoint = 0;
		Encoding encoding = Encoding::UTF8;
		while (true)
		{
			JSON_READ_CHAR_NO_SPACES(srcStream, codePoint, encoding);
			if (codePoint == EOF)
			{
				return;
			}
			serializeUnicodeChar(dst, codePoint, dstEncoding);
		}
	}

	void utf8ToUtf16(std::string& dst, const std::string& src, Encoding dstEncoding)
	{
		std::ostringstream stream;
		utf8ToUtf16(stream, src, dstEncoding);
		dst = stream.str();
	}
}

#pragma GCC diagnostic pop
