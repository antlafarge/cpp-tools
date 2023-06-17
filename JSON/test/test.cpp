#include "../src/optional.polyfill.h"
#include "../src/json_definitions.h"

#include <bitset>
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

#if defined(_MSC_VER)
#	define NOMINMAX
#	include <Windows.h>
#endif

#define PI 3.14159265358979323846264338327950288419716939937510582L

// A custom class can be used as map key if "operator>>" and "opeartor<<" to/from stream are implemented, and "json_implementations.h" is included after this Class definition.
struct Key
{
public:

	Key() = default;

	Key(int32_t value)
		: _value(value)
	{
	}

	Key(const char* value)
		: _value(std::stoi(value))
	{
	}

	Key(const std::string& value)
		: _value(std::stoi(value))
	{
	}

	Key(const Key& other)
		: _value(other._value)
	{
	}

	Key(Key&& other) noexcept
		: _value(std::move(other._value))
	{
	}

	~Key() = default;

	Key& operator=(const Key& other)
	{
		_value = other._value;
		return *this;
	}

	Key& operator=(const int32_t otherValue)
	{
		_value = otherValue;
		return *this;
	}

	int32_t value() const
	{
		return _value;
	}

	bool operator<(const Key& other) const
	{
		return _value < other._value;
	}

	bool operator>(const Key& other) const
	{
		return _value > other._value;
	}

	bool operator<=(const Key& other) const
	{
		return _value <= other._value;
	}

	bool operator>=(const Key& other) const
	{
		return _value >= other._value;
	}

	bool operator==(const Key& other) const
	{
		return _value == other._value;
	}

	bool operator!=(const Key& other) const
	{
		return _value != other._value;
	}

private:

	int32_t _value = 0;
};

std::istream& operator>>(std::istream& stream, Key& key)
{
	int32_t value;
	stream >> value;
	key = value;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Key& key)
{
	return stream << key.value();
}

namespace JSON
{
	template<std::size_t TBitsetSize>
	void serializeValue(std::ostream& stream, const std::bitset<TBitsetSize>* value, const Options& options)
	{
		std::string value2 = value->to_string();
		serializeValue(stream, &value2, options);
	}

	template<std::size_t TBitsetSize>
	void deserializeValue(std::istream& stream, std::bitset<TBitsetSize>* value, const Options& options)
	{
		if (value != nullptr)
		{
			std::string data;
			JSON::deserializeValue(stream, &data, options);
			*value = std::bitset<TBitsetSize>(data);
		}
		else
		{
			JSON::deserializeValue(stream, (std::string*)nullptr, options);
		}
	}
}

#include "../src/json_implementations.h"

template<class TValue>
bool operator==(const std::priority_queue<TValue>& left, const std::priority_queue<TValue>& right)
{
	auto copy1 = left, copy2 = right;
	if (copy1.size() != copy2.size())
	{
		return false;
	}
	while (!copy1.empty())
	{
		if (copy1.top() != copy2.top())
		{
			return false;
		}
		copy1.pop();
		copy2.pop();
	}
	return true;
}

template<class TValue>
void testJson(const std::string& title, const TValue& value1)
{
	std::cout << title << std::endl;
	std::string json = JSON::serialize(value1);
	std::cout << "json=" << json << std::endl;
	TValue value2;
	JSON::deserialize(json, value2);
	bool passed = (value1 == value2);
	std::cout << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
	assert(passed);
}

struct SampleObject
{
	uint8_t sampleField = 0;

	SampleObject() = default;

	SampleObject(int32_t content)
		: sampleField(static_cast<uint8_t>(content))
	{
	}

	bool operator==(const SampleObject& other) const
	{
		return (sampleField == other.sampleField);
	}

	bool operator!=(const SampleObject& other) const
	{
		return (sampleField != other.sampleField);
	}

	bool operator<(const SampleObject& other) const
	{
		return (sampleField < other.sampleField);
	}

	JSON("namedField", sampleField);
};

struct BasicData
{
	bool boolean = false;
	int8_t int8 = 0;
	int16_t int16 = 0;
	int32_t int32 = 0;
	int64_t int64 = 0;
	uint8_t uint8 = 0;
	uint16_t uint16 = 0;
	uint32_t uint32 = 0;
	uint64_t uint64 = 0;
	float myFloat = 0.0f;
	double myDouble = 0.0;
	double myLongDouble = 0.0L;
	std::string string;
	std::wstring wstring;

	bool operator==(const BasicData& other) const
	{
		bool b1 = boolean == other.boolean;
		bool b2 = int8 == other.int8;
		bool b3 = int16 == other.int16;
		bool b4 = int32 == other.int32;
		bool b5 = int64 == other.int64;
		bool b6 = uint8 == other.uint8;
		bool b7 = uint16 == other.uint16;
		bool b8 = uint32 == other.uint32;
		bool b9 = uint64 == other.uint64;
		bool b10 = fabs(myFloat - other.myFloat) < 0.01f;
		bool b11 = abs(myDouble - other.myDouble) < 0.0001;
		bool b12 = fabsl(myLongDouble - other.myLongDouble) < 0.000001L;
		bool b13 = string == other.string;
		bool b14 = wstring == other.wstring;
		return (b1 && b2 && b3 && b4 && b5 && b6 && b7 && b8 && b9 && b10 && b11 && b12 && b13 && b14);
	}

	JSON
	(
		"boolean", boolean,
		"int8", int8,
		"int16", int16,
		"int32", int32,
		"int64", int64,
		"uint8", uint8,
		"uint16", uint16,
		"uint32", uint32,
		"uint64", uint64,
		JSON::Field("myFloat", 3), myFloat,
		JSON::Field("myDouble", 5), myDouble,
		JSON::Field("myLongDouble", 7), myLongDouble,
		"string", string,
		"wstring", wstring
	);
};

struct ContainerData
{
	std::optional<std::string> null;
	std::optional<std::string> notNull;
	std::array<SampleObject, 3> array = {};
	std::vector<SampleObject> vector;
	std::deque<SampleObject> deque;
	std::forward_list<SampleObject> forwardList;
	std::list<SampleObject> list;
	std::set<uint8_t> set;
	std::map<std::string, SampleObject> map;
	std::multiset<uint8_t> multiset;
	std::multimap<std::string, SampleObject> multimap;
	std::unordered_set<uint8_t> uset;
	std::unordered_map<std::string, SampleObject> umap;
	std::unordered_multiset<uint8_t> umultiset;
	std::unordered_multimap<std::string, SampleObject> umultimap;
	std::stack<SampleObject> stack;
	std::queue<SampleObject> queue;
	std::priority_queue<SampleObject> priorityQueue;

	bool operator==(const ContainerData& other) const
	{
		bool b1 = null == other.null;
		bool b2 = notNull == other.notNull;
		bool b3 = array == other.array;
		bool b4 = vector == other.vector;
		bool b5 = deque == other.deque;
		bool b6 = forwardList == other.forwardList;
		bool b7 = list == other.list;
		bool b8 = set == other.set;
		bool b9 = map == other.map;
		bool b10 = multiset == other.multiset;
		bool b11 = multimap == other.multimap;
		bool b12 = uset == other.uset;
		bool b13 = umap == other.umap;
		bool b14 = umultiset == other.umultiset;
		bool b15 = umultimap == other.umultimap;
		bool b16 = stack == other.stack;
		bool b17 = queue == other.queue;
		bool b18 = priorityQueue == other.priorityQueue;
		return (b1 && b2 && b3 && b4 && b5 && b6 && b7 && b8 && b9 && b10 && b11 && b12 && b13 && b14 && b15 && b16 && b17 && b18);
	}

	JSON
	(
		"null", null,
		"notNull", notNull,
		"array", array,
		"vector", vector,
		"deque", deque,
		"forwardList", forwardList,
		"list", list,
		"set", set,
		"map", map,
		"multiset", multiset,
		"multimap", multimap,
		"uset", uset,
		"umap", umap,
		"umultiset", umultiset,
		"umultimap", umultimap,
		"stack", stack,
		"queue", queue,
		"priorityQueue", priorityQueue
	);
};

struct GlobalData
{
	BasicData basicData;
	ContainerData containerData;

	bool operator==(const GlobalData& other) const
	{
		return (basicData == other.basicData && containerData == other.containerData);
	}

	JSON
	(
		"basicData", basicData,
		"containerData", containerData
	);
};

struct FloatParts
{
#if defined(JSON_PLATFORM_IS_BIG_ENDIAN)
	uint32_t sign : 1;
	uint32_t exponent : 8;
	uint32_t mantissa : 23;
#else // DEFAULT IS LITTLE ENDIAN
	uint32_t mantissa : 23;
	uint32_t exponent : 8;
	uint32_t sign : 1;
#endif
};

struct DoubleParts
{
#if defined(JSON_PLATFORM_IS_BIG_ENDIAN)
	uint64_t sign : 1;
	uint64_t exponent : 11;
	uint64_t mantissa : 52;
#else // DEFAULT IS LITTLE ENDIAN
	uint64_t mantissa : 52;
	uint64_t exponent : 11;
	uint64_t sign : 1;
#endif
};

void testUnicodeChar(uint32_t codePoint)
{
	{
		std::cout << "CodePoint :\t0x" << std::uppercase << std::setfill('0') << std::hex << std::setw(8) << codePoint << std::endl;
	}

	{
		uint32_t unicode = JSON::codePointToUtf8(codePoint);
		std::string bytesStr;
		JSON::serializeCodePointChar(bytesStr, codePoint, JSON::Encoding::UTF8);
		std::cout << "UTF-8 :\t\t0x" << unicode  << std::endl;
		uint32_t codePoint2 = JSON::utf8ToCodePoint(unicode);
		assert(codePoint == codePoint2);
	}

	{
		uint32_t unicode = JSON::codePointToUtf16(codePoint, JSON::Encoding::BigEndian);
		std::string bytesStr;
		JSON::serializeCodePointChar(bytesStr, codePoint, JSON::Encoding::UTF16BE);
		std::cout << "UTF-16BE :\t0x" << unicode << std::endl;
		uint32_t codePoint2 = JSON::utf16ToCodePoint(unicode, JSON::Encoding::BigEndian);
		assert(codePoint == codePoint2);
	}

	{
		uint32_t unicode = JSON::codePointToUtf16(codePoint, JSON::Encoding::LittleEndian);
		std::string bytesStr;
		JSON::serializeCodePointChar(bytesStr, codePoint, JSON::Encoding::UTF16LE);
		std::cout << "UTF-16LE :\t0x" << unicode << std::endl;
		uint32_t codePoint2 = JSON::utf16ToCodePoint(unicode, JSON::Encoding::LittleEndian);
		assert(codePoint == codePoint2);
	}

	{
		uint32_t unicode = JSON::codePointToUtf32(codePoint, JSON::Encoding::BigEndian);
		std::string bytesStr;
		JSON::serializeCodePointChar(bytesStr, codePoint, JSON::Encoding::UTF32BE);
		std::cout << "UTF-32BE :\t0x" << unicode << std::endl;
		uint32_t codePoint2 = JSON::utf32ToCodePoint(unicode, JSON::Encoding::BigEndian);
		assert(codePoint == codePoint2);
	}

	{
		uint32_t unicode = JSON::codePointToUtf32(codePoint, JSON::Encoding::LittleEndian);
		std::string bytesStr;
		JSON::serializeCodePointChar(bytesStr, codePoint, JSON::Encoding::UTF32LE);
		std::cout << "UTF-32LE :\t0x" << unicode << std::endl;
		uint32_t codePoint2 = JSON::utf32ToCodePoint(unicode, JSON::Encoding::LittleEndian);
		assert(codePoint == codePoint2);
	}

	std::cout << std::endl;
}

struct TestJsonUtf8
{
	std::string myStr;

	JSON(JSON::Field("UTF8", JSON::Encoding::UTF8), myStr);
};

struct TestJsonUtf8bom
{
	std::string myStr;

	JSON(JSON::Field("UTF8BOM", JSON::Encoding::UTF8), myStr);
};

struct TestJsonUtf16BE
{
	std::string myStr;

	JSON(JSON::Field("UTF16BE", JSON::Encoding::UTF16BE), myStr);
};

struct TestJsonUtf16LE
{
	std::string myStr;

	JSON(JSON::Field("UTF16LE", JSON::Encoding::UTF16LE), myStr);
};

template<class T>
void testJson(const std::string& sourceFileName, const std::string& targetFileName, JSON::Encoding writeEncoding = JSON::Encoding::Unknown)
{
	std::cout << sourceFileName << " => " << targetFileName << std::endl;
	T object;
	JSON::deserialize(std::ifstream(sourceFileName), object);
	JSON::serialize(std::ofstream(targetFileName), object, writeEncoding);
	bool valid = JSON::isValid(std::ifstream(targetFileName));
	std::cout << (valid ? "PASSED" : "FAILED") << std::endl << std::endl;
	assert(valid);
}

void testOptional()
{
	std::cout << "optional" << std::endl;

	std::optional<int> opt;
	assert(!opt.has_value());

	opt = 1337;
	assert(opt.has_value());

	assert(*opt == 1337);

	assert(!(std::optional<int>() == std::optional<int>(1337)));
	assert(std::optional<int>(1337) == std::optional<int>(1337));
	assert(!(std::optional<int>(1337) == std::optional<int>(1338)));

	assert(std::optional<int>() != std::optional<int>(1337));
	assert(!(std::optional<int>(1337) != std::optional<int>(1337)));
	assert(std::optional<int>(1337) != std::optional<int>(1338));

	assert(std::optional<int>() < std::optional<int>(1337));
	assert(!(std::optional<int>(1337) < std::optional<int>(1337)));
	assert(std::optional<int>(1337) < std::optional<int>(1338));

	assert(std::optional<int>() <= std::optional<int>(1337));
	assert(std::optional<int>(1337) <= std::optional<int>(1337));
	assert(std::optional<int>(1337) <= std::optional<int>(1338));

	assert(!(std::optional<int>() > std::optional<int>(1337)));
	assert(!(std::optional<int>(1337) > std::optional<int>(1337)));
	assert(!(std::optional<int>(1337) > std::optional<int>(1338)));

	assert(!(std::optional<int>() >= std::optional<int>(1337)));
	assert(std::optional<int>(1337) >= std::optional<int>(1337));
	assert(!(std::optional<int>(1337) >= std::optional<int>(1338)));

	std::cout << "PASSED" << std::endl
		<< std::endl;
}

void testUnicode()
{
	testUnicodeChar(0x00000041);
	testUnicodeChar(0x000000D8);
	testUnicodeChar(0x00000AFF);
	testUnicodeChar(0x0001F408);

	try
	{
		testUnicodeChar(0xD800);
	}
	catch (const std::exception& ex)
	{
		std::cout << "Exception as expected : " << ex.what() << std::endl
			<< std::endl;
	}

	try
	{
		testUnicodeChar(0xFFFE);
	}
	catch (const std::exception& ex)
	{
		std::cout << "Exception as expected : " << ex.what() << std::endl
			<< std::endl;
	}
}

void testJsonBasic()
{
	testJson("false", false);
	testJson("true", true);
	testJson("INT8_MIN", INT8_MIN);
	testJson("INT16_MIN", INT16_MIN);
	testJson("INT32_MIN", INT32_MIN);
	testJson("INT64_MIN", INT64_MIN);
	testJson("UINT8_MAX", UINT8_MAX);
	testJson("UINT16_MAX", UINT16_MAX);
	testJson("UINT32_MAX", UINT32_MAX);
	testJson("UINT64_MAX", UINT64_MAX);
	testJson("float", (float)PI);
	testJson("double", (double)PI);
	testJson("long double", PI);
	testJson("-0.1e-200", -1.0e-200);
	testJson("optional<bool>()", std::optional<bool>());
	testJson("std::optional<bool>(false)", std::optional<bool>(false));
	testJson("std::optional<bool>(true)", std::optional<bool>(true));
	testJson("std::optional<float>((float)PI)", std::optional<float>((float)PI));
	testJson("std::optional<std::string>(\"STRING\")", std::optional<std::string>("STRING"));
	testJson("std::optional<std::wstring>(L\"WSTRING\")", std::optional<std::wstring>(L"WSTRING"));
	testJson("std::array", std::array<uint32_t, 3>{ 2, 1, 0 });
	testJson("std::vector", std::vector<uint32_t>{ 2, 1, 0 });
	testJson("std::deque", std::deque<uint32_t>{ 2, 1, 0 });
	testJson("std::forward_list", std::forward_list<uint32_t>{ 2, 1, 0 });
	testJson("std::list", std::list<uint32_t>{ 2, 1, 0 });
	testJson("std::set", std::set<uint32_t>{ 2, 1, 0 });
	testJson("std::map<string, uint>", std::map<std::string, uint32_t>{ { "a", 0}, { "b", 1 } });
	testJson("std::map<uint32_t, uint32_t>", std::map<uint32_t, uint32_t>{ { 0, 1}, { 2, 3 } });
	testJson("std::map<Key, uint32_t>", std::map<Key, uint32_t>{ { Key(13), 1}, { Key(37), 3 } });
	testJson("std::multiset", std::multiset<uint32_t>{ 2, 1, 0 });
	testJson("std::multimap<string, uint>", std::multimap<std::string, uint32_t>{ { "a", 0}, { "b", 1 } });
	testJson("std::unordered_set", std::unordered_set<uint32_t>{ 2, 1, 0 });
	testJson("std::unordered_map<std::string, uint32_t>", std::unordered_map<std::string, uint32_t>{ { "a", 0}, { "b", 1 } });
	testJson("std::unordered_multiset", std::unordered_multiset<uint32_t>{ 2, 1, 0 });
	testJson("std::unordered_multimap<std::string, uint32_t>", std::unordered_multimap<std::string, uint32_t>{ { "a", 0}, { "b", 1 } });
	testJson("std::stack", std::stack<uint32_t>({ 2, 1, 0 }));
	testJson("std::queue", std::queue<uint32_t>({ 2, 1, 0 }));
	testJson("std::priority_queue", std::priority_queue<uint32_t>(std::less<uint32_t>(), { 2, 1, 0 }));
}

void testJsonFloat()
{
	std::cout << "sizeof(float)=" << sizeof(float) << std::endl;
	std::cout << "sizeof(double)=" << sizeof(double) << std::endl;
	std::cout << "sizeof(long double)=" << sizeof(long double) << std::endl;
	std::cout << std::endl;

	{
		// FLOATs
		FloatParts fl;
		fl.sign = 0b1;
		fl.mantissa = 0b11111111111111111111111;

		{
			fl.exponent = 0b00000001;
			std::ostringstream stream;
			float number = *(float*)&fl;
			stream << "JSON::Value(" << number << ") exponent=0b" << std::bitset<8>(fl.exponent);
			testJson(stream.str(), JSON::Value(number));
		}

		{
			fl.exponent = 0b11111110;
			std::ostringstream stream;
			float number = *(float*)&fl;
			stream << "JSON::Value(" << number << ") exponent=0b" << std::bitset<8>(fl.exponent);
			testJson(stream.str(), JSON::Value(number));
		}
	}

	{
		// BOUBLEs
		DoubleParts db;
		db.sign = 0b1;
		db.mantissa = 0b1111111111111111111111111111111111111111111111111111;

		{
#if defined(__ppc__) || defined(__PPC__) || defined(__ppc64__) || defined(__PPC64__)
			db.exponent = 0b00000110110; // TODO: why stold throws an out_of_range exception when exponent less than this value?
#else
			db.exponent = 0b00000000001;
#endif
			std::ostringstream stream;
			double number = *(double*)&db;
			stream << "JSON::Value(" << number << "L) exponent=0b" << std::bitset<11>(db.exponent);
			testJson(stream.str(), JSON::Value(number));
		}

		{
			db.exponent = 0b11111111110;
			std::ostringstream stream;
			double number = *(double*)&db;
			stream << "JSON::Value(" << number << "L) exponent=0b" << std::bitset<11>(db.exponent);
			testJson(stream.str(), JSON::Value(number));
		}
	}
}

void testJsonCustomDeSerialization()
{
	testJson("std::bitset", std::bitset<8>{ 0xA });
}

void testJsonCustomClass()
{
	GlobalData data{
		{
			true,
			INT8_MIN,
			INT16_MIN,
			INT32_MIN,
			INT64_MIN,
			UINT8_MAX,
			UINT16_MAX,
			UINT32_MAX,
			UINT64_MAX,
			(float)PI,
			(double)PI,
			PI,
			"myString",
			L"myWString"
		},
		{
			{},
			{ "Not null" },
			{3,2,1},
			{3,2,1},
			{3,2,1},
			{3,2,1},
			{3,2,1},
			{3,2,1},
			{{"B",2},{"A",1}},
			{3,2,1},
			{{"D",4},{"C",3}},
			{3,2,1},
			{{"B",2},{"A",1}},
			{3,2,1},
			{{"D",4},{"C",3}},
			std::stack<SampleObject>({3,2,1}),
			std::queue<SampleObject>({3,2,1}),
			std::priority_queue<SampleObject>(std::less<SampleObject>(), {3,2,1})
		}
	};

	testJson("GlobalData", data);
}

void testJsonValue()
{
	JSON::Value jsValue = JSON::Value::createObject();
	jsValue.getObject()["A"] = JSON::Value();
	jsValue.getObject()["B"] = JSON::Value(false);
	jsValue.getObject()["C"] = JSON::Value(true);
	jsValue.getObject()["D"] = JSON::Value(INT8_MIN);
	jsValue.getObject()["E"] = JSON::Value(UINT8_MAX);
	jsValue.getObject()["F"] = JSON::Value(INT16_MIN);
	jsValue.getObject()["G"] = JSON::Value(UINT16_MAX);
	jsValue.getObject()["H"] = JSON::Value(INT32_MIN);
	jsValue.getObject()["I"] = JSON::Value(INT32_MAX);
	jsValue.getObject()["J"] = JSON::Value(INT64_MIN);
	jsValue.getObject()["K"] = JSON::Value(UINT64_MAX);
	jsValue.getObject()["L"] = JSON::Value((float)PI);
	jsValue.getObject()["M"] = JSON::Value((double)PI);
	jsValue.getObject()["N"] = JSON::Value(PI);
	jsValue.getObject()["O"] = JSON::Value("STRING_2");
	jsValue.getObject()["O"] = JSON::Value(L"WSTRING_2", JSON::Encoding::UTF16BE);
	jsValue.getObject()["P"] = JSON::Value::createArray();
	jsValue.getObject()["P"].getArray().push_back(JSON::Value("value1"));
	jsValue.getObject()["P"].getArray().push_back(JSON::Value("value2"));

	testJson("JSON::Value", jsValue);
}

void testJsonFile()
{
	std::map<std::string, std::string> data2{ { "Field", "Value" } };

	// Write to file
	JSON::serialize(std::ofstream("data.json"), data2);

	// Read from file
	JSON::deserialize(std::ifstream("data.json"), data2);
}

void testJsonUnicode()
{
	{
		std::string strA = "ABC", strC;
		std::wstring wstrB;
		JSON::convert(wstrB, strA, JSON::Encoding::UTF16BE, JSON::Encoding::UTF8);
		JSON::convert(strC, wstrB, JSON::Encoding::UTF8, JSON::Encoding::UTF16BE);
		assert(strA == strC);
	}

	{
		std::string strA = "ABC", strC;
		std::wstring wstrB;
		JSON::convert(wstrB, strA, JSON::Encoding::UTF16LE, JSON::Encoding::UTF8);
		JSON::convert(strC, wstrB, JSON::Encoding::UTF8, JSON::Encoding::UTF16LE);
		assert(strA == strC);
	}

	std::string catUtf8;
	catUtf8.resize(6, 0);
	catUtf8[0] = '"';
	catUtf8[1] = uint8_t(0xF0);
	catUtf8[2] = uint8_t(0x9F);
	catUtf8[3] = uint8_t(0x90);
	catUtf8[4] = uint8_t(0x88);
	catUtf8[5] = '"';

	{
		// UTF8
		std::string str;
		str.resize(6, 0);
		str[0] = '"';
		str[1] = uint8_t(0xF0);
		str[2] = uint8_t(0x9F);
		str[3] = uint8_t(0x90);
		str[4] = uint8_t(0x88);
		str[5] = '"';
		JSON::Value value;
		JSON::deserialize(str, value);
		auto json = JSON::serialize(value);
		assert(json == catUtf8);
	}

	{
		// UTF16BE
		std::string str;
		str.resize(8, 0);
		str[0] = uint8_t(0x00);
		str[1] = uint8_t(0x22);
		str[2] = uint8_t(0xD8);
		str[3] = uint8_t(0x3D);
		str[4] = uint8_t(0xDC);
		str[5] = uint8_t(0x08);
		str[6] = uint8_t(0x00);
		str[7] = uint8_t(0x22);
		JSON::Value value;
		JSON::deserialize(str, value, JSON::Options(JSON::Encoding::UTF16BE));
		auto json = JSON::serialize(value);
		assert(json == catUtf8);
	}

	{
		// UTF16LE
		std::string str;
		str.resize(8, 0);
		str[0] = uint8_t(0x22);
		str[1] = uint8_t(0x00);
		str[2] = uint8_t(0x3D);
		str[3] = uint8_t(0xD8);
		str[4] = uint8_t(0x08);
		str[5] = uint8_t(0xDC);
		str[6] = uint8_t(0x22);
		str[7] = uint8_t(0x00);
		JSON::Value value;
		JSON::deserialize(str, value, (JSON::Encoding::UTF16LE));
		auto json = JSON::serialize(value);
		assert(json == catUtf8);
	}

	{
		// UTF32BE
		std::string str;
		str.resize(12, 0);
		str[0] = uint8_t(0x00);
		str[1] = uint8_t(0x00);
		str[2] = uint8_t(0x00);
		str[3] = uint8_t(0x22);
		str[4] = uint8_t(0x00);
		str[5] = uint8_t(0x01);
		str[6] = uint8_t(0xF4);
		str[7] = uint8_t(0x08);
		str[8] = uint8_t(0x00);
		str[9] = uint8_t(0x00);
		str[10] = uint8_t(0x00);
		str[11] = uint8_t(0x22);
		JSON::Value value;
		JSON::deserialize(str, value, (JSON::Encoding::UTF32BE));
		auto json = JSON::serialize(value);
		assert(json == catUtf8);
	}

	{
		// UTF32LE
		std::string str;
		str.resize(12, 0);
		str[0] = uint8_t(0x22);
		str[1] = uint8_t(0x00);
		str[2] = uint8_t(0x00);
		str[3] = uint8_t(0x00);
		str[4] = uint8_t(0x08);
		str[5] = uint8_t(0xF4);
		str[6] = uint8_t(0x01);
		str[7] = uint8_t(0x00);
		str[8] = uint8_t(0x22);
		str[9] = uint8_t(0x00);
		str[10] = uint8_t(0x00);
		str[11] = uint8_t(0x00);
		JSON::Value value;
		JSON::deserialize(str, value, (JSON::Encoding::UTF32LE));
		auto json = JSON::serialize(value);
		assert(json == catUtf8);
	}

	// JSON::Value testJsonUtf8(JSON::Encoding::UTF8);
	testJson<TestJsonUtf8>("UTF8.json", "UTF8_2.json", JSON::Encoding::UTF8);

	// JSON::Value testJsonUtf8Bom(JSON::Encoding::UTF8);
	testJson<TestJsonUtf8bom>("UTF8BOM.json", "UTF8BOM_2.json", JSON::Encoding::UTF8);

	// JSON::Value testJsonUtf16Be(JSON::Encoding::UTF16BE);
	testJson<TestJsonUtf16BE>("UTF16BE.json", "UTF16BE_2.json", JSON::Encoding::UTF16BE);

	// JSON::Value testJsonUtf16Le(JSON::Encoding::UTF16LE);
	testJson<TestJsonUtf16LE>("UTF16LE.json", "UTF16LE_2.json", JSON::Encoding::UTF16LE);
}

int main()
{
	testOptional();
	testUnicode();
	testJsonBasic();
	testJsonFloat();
	testJsonCustomDeSerialization();
	testJsonCustomClass();
	testJsonValue();
	testJsonFile();
	testJsonUnicode();

	std::cout << "ALL TESTS PASSED" << std::endl;

	return 0;
}
