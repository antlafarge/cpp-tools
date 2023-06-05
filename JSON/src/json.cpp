#include "json_definitions.h"
#include "json_implementations.h"

#include <iomanip>
#include <sstream>
#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

namespace JSON
{
	Type operator|(Type lhs, Type rhs)
	{
		return static_cast<Type>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	Type operator&(Type lhs, Type rhs)
	{
		return static_cast<Type>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
	}

	Options::Options::Options(Encoding jsonEncoding)
		: jsonEncoding(jsonEncoding)
	{
	}

	Options::Options(Encoding jsonEncoding, Encoding memberEncoding)
		: jsonEncoding(jsonEncoding)
		, memberEncoding(memberEncoding)
	{
	}

	Options::Options(int32_t precision)
		: precision(precision)
	{
	}

	Options::Options(Encoding jsonEncoding, int32_t precision)
		: jsonEncoding(jsonEncoding)
		, precision(precision)
	{
	}

	Options::Options(Encoding jsonEncoding, Encoding memberEncoding, int32_t precision)
		: jsonEncoding(jsonEncoding)
		, memberEncoding(memberEncoding)
		, precision(precision)
	{
	}

	Options::Options(int32_t precision, Encoding jsonEncoding)
		: jsonEncoding(jsonEncoding)
		, precision(precision)
	{
	}

	Options::Options(int32_t precision, Encoding jsonEncoding, Encoding memberEncoding)
		: jsonEncoding(jsonEncoding)
		, memberEncoding(memberEncoding)
		, precision(precision)
	{
	}

	Options::Options(const Options& other)
		: jsonEncoding(other.jsonEncoding)
		, memberEncoding(other.memberEncoding)
		, precision(other.precision)
	{
	}

	Options::Options(Options&& other) noexcept
		: jsonEncoding(other.jsonEncoding)
		, memberEncoding(other.memberEncoding)
		, precision(other.precision)
	{
	}

	Options& Options::operator=(const Options& other)
	{
		jsonEncoding = other.jsonEncoding;
		memberEncoding = other.memberEncoding;
		precision = other.precision;
		return *this;
	}

	Options& Options::operator=(Options&& other) noexcept
	{
		jsonEncoding = other.jsonEncoding;
		memberEncoding = other.memberEncoding;
		precision = other.precision;
		return *this;
	}

	Field::Field(const char* fieldName, Encoding memberEncoding, int32_t precision)
		: name(fieldName)
		, memberEncoding(memberEncoding)
		, precision(precision)
	{
	}

	Field::Field(const std::string& fieldName, Encoding memberEncoding, int32_t precision)
		: name(fieldName)
		, memberEncoding(memberEncoding)
		, precision(precision)
	{
	}

	Field::Field(std::string&& fieldName, Encoding memberEncoding, int32_t precision)
		: name(fieldName)
		, memberEncoding(memberEncoding)
		, precision(precision)
	{
	}

	Field::Field(const char* fieldName, int32_t precision, Encoding memberEncoding)
		: name(fieldName)
		, memberEncoding(memberEncoding)
		, precision(precision)
	{
	}

	Field::Field(const std::string& fieldName, int32_t precision, Encoding memberEncoding)
		: name(fieldName)
		, memberEncoding(memberEncoding)
		, precision(precision)
	{
	}

	Field::Field(std::string&& fieldName, int32_t precision, Encoding memberEncoding)
		: name(fieldName)
		, memberEncoding(memberEncoding)
		, precision(precision)
	{
	}

	Field::Field(const Field& other)
		: name(other.name)
		, memberEncoding(other.memberEncoding)
		, precision(other.precision)
	{
	}

	Field::Field(Field&& other) noexcept
		: name(std::move(other.name))
		, memberEncoding(other.memberEncoding)
		, precision(other.precision)
	{
	}

	Field& Field::operator=(const Field& other)
	{
		name = other.name;
		memberEncoding = other.memberEncoding;
		precision = other.precision;
		return *this;
	}

	Field& Field::operator=(Field&& other) noexcept
	{
		name = std::move(other.name);
		memberEncoding = std::move(other.memberEncoding);
		precision = std::move(other.precision);
		return *this;
	}

	bool Field::operator==(const Field& other) const
	{
		return name == other.name;
	}

	bool Field::operator!=(const Field& other) const
	{
		return name != other.name;
	}

	Field::operator std::string() const
	{
		return name;
	}

	Value::Value(Type srcType)
	{
		*this = srcType;
	}

	Value::Value(const bool srcBoolean)
		: type(Type::Boolean)
		, ptrBoolean(new bool(srcBoolean))
	{
	}

	Value::Value(const int8_t srcNumber)
		: type(Type::NumberI)
		, ptrNumberI(new int64_t(static_cast<int64_t>(srcNumber)))
	{
	}

	Value::Value(const int16_t srcNumber)
		: type(Type::NumberI)
		, ptrNumberI(new int64_t(static_cast<int64_t>(srcNumber)))
	{
	}

	Value::Value(const int32_t srcNumber)
		: type(Type::NumberI)
		, ptrNumberI(new int64_t(static_cast<int64_t>(srcNumber)))
	{
	}

	Value::Value(const int64_t srcNumber)
		: type(Type::NumberI)
		, ptrNumberI(new int64_t(srcNumber))
	{
	}

	Value::Value(const uint8_t srcNumber)
		: type(Type::NumberU)
		, ptrNumberU(new uint64_t(static_cast<uint64_t>(srcNumber)))
	{
	}

	Value::Value(const uint16_t srcNumber)
		: type(Type::NumberU)
		, ptrNumberU(new uint64_t(static_cast<uint64_t>(srcNumber)))
	{
	}

	Value::Value(const uint32_t srcNumber)
		: type(Type::NumberU)
		, ptrNumberU(new uint64_t(static_cast<uint64_t>(srcNumber)))
	{
	}

	Value::Value(const uint64_t srcNumber)
		: type(Type::NumberU)
		, ptrNumberU(new uint64_t(srcNumber))
	{
	}

	Value::Value(const float srcNumber)
		: type(Type::NumberF)
		, ptrNumberF(new long double(srcNumber))
	{
	}

	Value::Value(const double srcNumber)
		: type(Type::NumberF)
		, ptrNumberF(new long double(srcNumber))
	{
	}

	Value::Value(const long double srcNumber)
		: type(Type::NumberF)
		, ptrNumberF(new long double(srcNumber))
	{
	}

	Value::Value(const char* srcString)
		: type(Type::String)
		, ptrString(new std::string(srcString))
	{
	}

	Value::Value(const std::string& srcString)
		: type(Type::String)
		, ptrString(new std::string(srcString))
	{
	}

	Value::Value(const wchar_t* srcString, Encoding srcEncoding)
		: Value(std::wstring(srcString), srcEncoding)
	{
	}

	Value::Value(const std::wstring& srcString, Encoding srcEncoding)
		: type(Type::String)
		, ptrString(new std::string())
	{
		if (hasFlags(srcEncoding, Encoding::UTF16)) [[likely]]
		{
			convert(*ptrString, srcString, JSON_DEFAULT_ENCODING, srcEncoding);
		}
		else [[unlikely]]
		{
			throw std::runtime_error("json_value_encodingconversionnotsupported");
		}
	}

	Value::Value(const std::vector<Value>& srcContainer)
		: type(Type::Array)
		, ptrArray(new std::vector<Value>(srcContainer))
	{
	}

	Value::Value(const std::unordered_map<std::string, Value>& srcContainer)
		: type(Type::Object)
		, ptrObject(new std::unordered_map<Field, Value>())
	{
		ptrObject->reserve(srcContainer.size());
		for (auto& pair : srcContainer)
		{
			(*ptrObject)[pair.first] = pair.second;
		}
	}

	Value::Value(const std::unordered_map<Field, Value>& srcContainer)
		: type(Type::Object)
		, ptrObject(new std::unordered_map<Field, Value>(srcContainer))
	{
	}

	Value::Value(const Value& other)
	{
		*this = other;
	}

	Value::Value(Value&& other) noexcept
	{
		std::swap(type, other.type);
		std::swap(ptr, other.ptr);
	}

	Value::~Value()
	{
		deleteData();
	}

	Value& Value::operator=(const Value& other)
	{
		switch (other.type)
		{
		case Type::Boolean:
			*this = *other.ptrBoolean;
			break;
		case Type::NumberI:
			*this = *other.ptrNumberI;
			break;
		case Type::NumberU:
			*this = *other.ptrNumberU;
			break;
		case Type::NumberF:
			*this = *other.ptrNumberF;
			break;
		case Type::String:
			*this = *other.ptrString;
			break;
		case Type::Array:
			*this = *other.ptrArray;
			break;
		case Type::Object:
			*this = *other.ptrObject;
			break;
		case Type::Null:
		default:
			break;
		}

		return *this;
	}

	Value& Value::operator=(Value&& other) noexcept
	{
		std::swap(type, other.type);
		std::swap(ptr, other.ptr);
		return *this;
	}

	Value& Value::operator=(nullptr_t)
	{
		return *this = Type::Null;
	}

	Value& Value::operator=(const Type srcType)
	{
		if (type != srcType) [[unlikely]]
		{
			deleteData();
			switch (srcType)
			{
			case Type::Boolean: [[unlikely]]
				ptrBoolean = new bool;
				break;
			case Type::NumberI: [[unlikely]]
				ptrNumberI = new int64_t;
				break;
			case Type::NumberU: [[unlikely]]
				ptrNumberU = new uint64_t;
				break;
			case Type::NumberF: [[unlikely]]
				ptrNumberF = new long double;
				break;
			case Type::String: [[unlikely]]
				ptrString = new std::string;
				break;
			case Type::Array: [[unlikely]]
				ptrArray = new std::vector<Value>;
				break;
			case Type::Object: [[unlikely]]
				ptrObject = new std::unordered_map<Field, Value>;
				break;
			case Type::Null: [[likely]]
			default: [[unlikely]]
				break;
			}
			type = srcType;
		}
		return *this;
	}

	Value& Value::operator=(const bool srcBoolean)
	{
		*this = Type::Boolean;
		*ptrBoolean = srcBoolean;
		return *this;
	}

	Value& Value::operator=(const int8_t srcNumber)
	{
		*this = Type::NumberI;
		*ptrNumberI = srcNumber;
		return *this;
	}

	Value& Value::operator=(const int16_t srcNumber)
	{
		*this = Type::NumberI;
		*ptrNumberI = srcNumber;
		return *this;
	}

	Value& Value::operator=(const int32_t srcNumber)
	{
		*this = Type::NumberI;
		*ptrNumberI = srcNumber;
		return *this;
	}

	Value& Value::operator=(const int64_t srcNumber)
	{
		*this = Type::NumberI;
		*ptrNumberI = srcNumber;
		return *this;
	}

	Value& Value::operator=(const uint8_t srcNumber)
	{
		*this = Type::NumberU;
		*ptrNumberU = srcNumber;
		return *this;
	}

	Value& Value::operator=(const uint16_t srcNumber)
	{
		*this = Type::NumberU;
		*ptrNumberU = srcNumber;
		return *this;
	}

	Value& Value::operator=(const uint32_t srcNumber)
	{
		*this = Type::NumberU;
		*ptrNumberU = srcNumber;
		return *this;
	}

	Value& Value::operator=(const uint64_t srcNumber)
	{
		*this = Type::NumberU;
		*ptrNumberU = srcNumber;
		return *this;
	}

	Value& Value::operator=(const float srcNumber)
	{
		*this = Type::NumberF;
		*ptrNumberF = srcNumber;
		return *this;
	}

	Value& Value::operator=(const double srcNumber)
	{
		*this = Type::NumberF;
		*ptrNumberF = srcNumber;
		return *this;
	}

	Value& Value::operator=(const long double srcNumber)
	{
		*this = Type::NumberF;
		*ptrNumberF = srcNumber;
		return *this;
	}

	Value& Value::operator=(const char* srcString)
	{
		return *this = std::string(srcString);
	}

	Value& Value::operator=(const std::string& srcString)
	{
		*this = Type::String;
		*ptrString = srcString;
		return *this;
	}

	Value& Value::operator=(const wchar_t* srcString)
	{
		return *this = std::wstring(srcString);
	}

	Value& Value::operator=(const std::wstring& srcString)
	{
		*this = Type::String;
		convert(*ptrString, srcString, Encoding::UTF8, Encoding::UTF16BE);
		return *this;
	}

	Value& Value::operator=(const std::vector<Value>& srcContainer)
	{
		*this = Type::Array;
		*ptrArray = srcContainer;
		return *this;
	}

	Value& Value::operator=(const std::unordered_map<std::string, Value>& srcContainer)
	{
		*this = Type::Object;
		ptrObject->reserve(srcContainer.size());
		for (auto& pair : srcContainer)
		{
			(*ptrObject)[pair.first] = pair.second;
		}
		return *this;
	}

	Value& Value::operator=(const std::unordered_map<Field, Value>& srcContainer)
	{
		*this = Type::Object;
		*ptrObject = srcContainer;
		return *this;
	}

	bool Value::operator==(const Type otherType) const
	{
		return type == otherType;
	}

	bool Value::operator==(const Value& other) const
	{
		switch (type)
		{
		case Type::Null:
			return type == other.type;
		case Type::Boolean:
			return type == other.type && *ptrBoolean == *other.ptrBoolean;
		case Type::NumberI:
			switch (other.type)
			{
			case Type::NumberI:
				return *ptrNumberI == *other.ptrNumberI;
			case Type::NumberU:
				if (*ptrNumberI < 0) return false;
				return *ptrNumberI == *other.ptrNumberI;
			case Type::NumberF:
				return static_cast<long double>(*ptrNumberI) == *other.ptrNumberF;
			default:
				return false;
			}
		case Type::NumberU:
			switch (other.type)
			{
			case Type::NumberI:
				if (*other.ptrNumberI < 0) return false;
				return *ptrNumberU == *other.ptrNumberU;
			case Type::NumberU:
				return *ptrNumberU == *other.ptrNumberU;
			case Type::NumberF:
				return *ptrNumberU == *other.ptrNumberU;
			default:
				return false;
			}
		case Type::NumberF:
			switch (other.type)
			{
			case Type::NumberI:
				return *ptrNumberF == static_cast<long double>(*other.ptrNumberI);
			case Type::NumberU:
				return *ptrNumberF == static_cast<long double>(*other.ptrNumberU);
			case Type::NumberF:
				return *ptrNumberF == *other.ptrNumberF;
			default:
				return false;
			}
		case Type::String:
			return type == other.type && *ptrString == *other.ptrString;
		case Type::Array:
			return type == other.type && *ptrArray == *other.ptrArray;
		case Type::Object:
			return type == other.type && *ptrObject == *other.ptrObject;
		default:
			return false;
		}
	}

	bool Value::operator!=(const Value& other) const
	{
		return !(*this == other);
	}

	Value Value::createNull()
	{
		return Value(Type::Null);
	}

	Value Value::createBoolean()
	{
		return Value(Type::Boolean);
	}

	Value Value::createNumberI()
	{
		return Value(Type::NumberI);
	}

	Value Value::createNumberU()
	{
		return Value(Type::NumberU);
	}

	Value Value::createNumberF()
	{
		return Value(Type::NumberF);
	}

	Value Value::createString()
	{
		return Value(Type::String);
	}

	Value Value::createArray()
	{
		return Value(Type::Array);
	}

	Value Value::createObject()
	{
		return Value(Type::Object);
	}

	bool& Value::getBoolean() const
	{
		return *ptrBoolean;
	}

	int64_t& Value::getNumberI() const
	{
		return *ptrNumberI;
	}

	uint64_t& Value::getNumberU() const
	{
		return *ptrNumberU;
	}

	long double& Value::getNumberF() const
	{
		return *ptrNumberF;
	}

	std::string& Value::getString() const
	{
		return *ptrString;
	}

	std::vector<Value>& Value::getArray() const
	{
		return *ptrArray;
	}

	std::unordered_map<Field, Value>& Value::getObject() const
	{
		return *ptrObject;
	}

	bool Value::isNull() const
	{
		return type == Type::Null;
	}

	bool Value::isBoolean() const
	{
		return type == Type::Boolean;
	}

	bool Value::isNumber() const
	{
		return type == Type::NumberI || type == Type::NumberU || type == Type::NumberF;
	}

	bool Value::isString() const
	{
		return type == Type::String;
	}

	bool Value::isArray() const
	{
		return type == Type::Array;
	}

	bool Value::isObject() const
	{
		return type == Type::Object;
	}

	void Value::deleteData()
	{
		switch (type)
		{
		case Type::Boolean:
			delete ptrBoolean;
			break;
		case Type::NumberI:
			delete ptrNumberI;
			break;
		case Type::NumberU:
			delete ptrNumberU;
			break;
		case Type::NumberF:
			delete ptrNumberF;
			break;
		case Type::String:
			delete ptrString;
			break;
		case Type::Array:
			delete ptrArray;
			break;
		case Type::Object:
			delete ptrObject;
			break;
		case Type::Null:
		default:
			break;
		}
		ptr = nullptr;
		type = Type::Null;
	}

#pragma region Serialization

	void serializeValue(std::ostream& dstStream, const Value* srcValue, const Options& options)
	{
		switch (srcValue->type)
		{
		case Type::Null:
		{
			std::optional<bool> null;
			serializeValue(dstStream, &null, options);
			break;
		}
		case Type::Boolean:
			serializeValue(dstStream, srcValue->ptrBoolean, options);
			break;
		case Type::NumberI:
			serializeValue(dstStream, srcValue->ptrNumberI, options);
			break;
		case Type::NumberU:
			serializeValue(dstStream, srcValue->ptrNumberU, options);
			break;
		case Type::NumberF:
			serializeValue(dstStream, srcValue->ptrNumberF, options);
			break;
		case Type::String:
			serializeValue(dstStream, srcValue->ptrString, options);
			break;
		case Type::Array:
			serializeValue(dstStream, srcValue->ptrArray, options);
			break;
		case Type::Object:
			serializeValue(dstStream, srcValue->ptrObject, options);
			break;
		default: [[unlikely]]
			throw std::runtime_error("json_serialize_value_badtype");
		}
	}

	void serializeValue(std::ostream& dstStream, const bool* srcBoolean, const Options& options)
	{
		if (*srcBoolean)
		{
			serializeCodePointChar(dstStream, 't', options.jsonEncoding);
			serializeCodePointChar(dstStream, 'r', options.jsonEncoding);
			serializeCodePointChar(dstStream, 'u', options.jsonEncoding);
			serializeCodePointChar(dstStream, 'e', options.jsonEncoding);
		}
		else
		{
			serializeCodePointChar(dstStream, 'f', options.jsonEncoding);
			serializeCodePointChar(dstStream, 'a', options.jsonEncoding);
			serializeCodePointChar(dstStream, 'l', options.jsonEncoding);
			serializeCodePointChar(dstStream, 's', options.jsonEncoding);
			serializeCodePointChar(dstStream, 'e', options.jsonEncoding);
		}
	}

	void serializeValue(std::ostream& dstStream, const int8_t* srcNumber, const Options& options)
	{
		int64_t valueNumber = static_cast<int64_t>(*srcNumber);
		serializeValueInteger(dstStream, &valueNumber, options);
	}

	void serializeValue(std::ostream& dstStream, const uint8_t* srcNumber, const Options& options)
	{
		uint64_t valueNumber = static_cast<uint64_t>(*srcNumber);
		serializeValueInteger(dstStream, &valueNumber, options);
	}

	void serializeValue(std::ostream& dstStream, const int16_t* srcNumber, const Options& options)
	{
		int64_t valueNumber = static_cast<int64_t>(*srcNumber);
		serializeValueInteger(dstStream, &valueNumber, options);
	}

	void serializeValue(std::ostream& dstStream, const uint16_t* srcNumber, const Options& options)
	{
		uint64_t valueNumber = static_cast<uint64_t>(*srcNumber);
		serializeValueInteger(dstStream, &valueNumber, options);
	}

	void serializeValue(std::ostream& dstStream, const int32_t* srcNumber, const Options& options)
	{
		int64_t valueNumber = static_cast<int64_t>(*srcNumber);
		serializeValueInteger(dstStream, &valueNumber, options);
	}

	void serializeValue(std::ostream& dstStream, const uint32_t* srcNumber, const Options& options)
	{
		uint64_t valueNumber = static_cast<uint64_t>(*srcNumber);
		serializeValueInteger(dstStream, &valueNumber, options);
	}

	void serializeValue(std::ostream& dstStream, const int64_t* srcNumber, const Options& options)
	{
		serializeValueInteger(dstStream, srcNumber, options);
	}

	void serializeValue(std::ostream& dstStream, const uint64_t* srcNumber, const Options& options)
	{
		serializeValueInteger(dstStream, srcNumber, options);
	}

	void serializeValue(std::ostream& dstStream, const float* srcNumber, const Options& options)
	{
		serializeValueFloat(dstStream, srcNumber, options);
	}

	void serializeValue(std::ostream& dstStream, const double* srcNumber, const Options& options)
	{
		serializeValueFloat(dstStream, srcNumber, options);
	}

	void serializeValue(std::ostream& dstStream, const long double* srcNumber, const Options& options)
	{
		serializeValueFloat(dstStream, srcNumber, options);
	}

	void serializeValue(std::ostream& dstStream, const std::string* srcString, const Options& options)
	{
		serializeCodePointChar(dstStream, '"', options.jsonEncoding);
		if (options.memberEncoding <= Encoding::UTF8 && options.jsonEncoding <= Encoding::UTF8) [[likely]]
		{
			dstStream << *srcString;
		}
		else [[unlikely]]
		{
			convert(dstStream, *srcString, options.jsonEncoding, options.memberEncoding);
		}
		serializeCodePointChar(dstStream, '"', options.jsonEncoding);
	}

	void serializeValue(std::ostream& dstStream, const std::wstring* srcString, const Options& options)
	{
		serializeCodePointChar(dstStream, '"', options.jsonEncoding);
		convert(dstStream, *srcString, options.jsonEncoding, (options.memberEncoding != Encoding::Unknown ? options.memberEncoding : JSON_DEFAULT_ENCODING_WSTRING));
		serializeCodePointChar(dstStream, '"', options.jsonEncoding);
	}

#pragma endregion

#pragma region Deserialization

	bool isValid(const std::string& srcJson, const Options& options) noexcept
	{
		std::istringstream srcStream(srcJson);
		return isValid(srcStream, options);
	}

	bool isValid(std::ifstream&& srcStream, const Options& options) noexcept
	{
		std::istream srcStream2(srcStream.rdbuf());
		Options options2(options);
		if (srcStream.gcount() == 0) [[likely]]
		{
			readBOM(srcStream2, options2.jsonEncoding);
			if (options.jsonEncoding != JSON_DEFAULT_ENCODING) [[unlikely]]
			{
				options2.jsonEncoding = options.jsonEncoding;
			}
		}
		return isValid(srcStream2, options2);
	}

	bool isValid(std::ifstream& srcStream, const Options& options) noexcept
	{
		std::istream srcStream2(srcStream.rdbuf());
		Options options2(options);
		if (srcStream.gcount() == 0) [[likely]]
		{
			readBOM(srcStream2, options2.jsonEncoding);
			if (options.jsonEncoding != JSON_DEFAULT_ENCODING) [[unlikely]]
			{
				options2.jsonEncoding = options.jsonEncoding;
			}
		}
		return isValid(srcStream2, options2);
	}

	bool isValid(std::istream&& srcStream, const Options& options) noexcept
	{
		std::istream srcStream2(srcStream.rdbuf());
		return isValid(srcStream2, options);
	}

	bool isValid(std::istream& srcStream, const Options& options) noexcept
	{
		try
		{
			deserializeValue(srcStream, static_cast<Value*>(nullptr), options);
			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	void deserializeFieldValue(const Value&, const Options&)
	{
	}

	void deserializeValue(std::istream& srcStream, Value* dstValue, const Options& options)
	{
		uint32_t codePoint = 0;
		int32_t bytesCount = readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
		if (codePoint == JSON_EOF) [[unlikely]]
		{
			throw std::runtime_error("json_deserialize_value_truncated");
		}
		JSON_UNGET(srcStream, bytesCount);
		switch (codePoint)
		{
		case 'n':
		{
			deserializeValue(srcStream, static_cast<std::optional<bool>*>(nullptr), options);
			if (dstValue) [[likely]]
			{
				*dstValue = Type::Null;
			}
			break;
		}
		case 't':
		case 'f':
			if (dstValue) [[likely]]
			{
				*dstValue = Type::Boolean;
				deserializeValue(srcStream, dstValue->ptrBoolean, options);
			}
			else [[unlikely]]
			{
				deserializeValue(srcStream, static_cast<bool*>(nullptr), options);
			}
			break;
		case '"':
			if (dstValue) [[likely]]
			{
				*dstValue = Type::String;
				deserializeValue(srcStream, dstValue->ptrString, options);
			}
			else [[unlikely]]
			{
				deserializeValue(srcStream, static_cast<std::string*>(nullptr), options);
			}
			break;
		case '[':
			if (dstValue) [[likely]]
			{
				*dstValue = Type::Array;
				deserializeValue(srcStream, dstValue->ptrArray, options);
			}
			else [[unlikely]]
			{
				deserializeValue(srcStream, static_cast<std::vector<Value>*>(nullptr), options);
			}
			break;
		case '{':
			if (dstValue) [[likely]]
			{
				*dstValue = Type::Object;
				deserializeValue(srcStream, dstValue->ptrObject, options);
			}
			else [[unlikely]]
			{
				deserializeValue(srcStream, static_cast<std::unordered_map<std::string, Value>*>(nullptr), options);
			}
			break;
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '-':
			if (dstValue) [[likely]]
			{
				std::string numberStr;
				deserializeValueNumberAsString(srcStream, &numberStr, options);
				if (numberStr.find('.') != std::string::npos) [[unlikely]]
				{
					*dstValue = Type::NumberF;
					*dstValue->ptrNumberF = std::stold(numberStr);
				}
				else [[likely]]
				{
					try
					{
						if (numberStr[0] == '-') [[unlikely]]
						{
							*dstValue = Type::NumberI;
							*dstValue->ptrNumberI = std::stoll(numberStr);
						}
						else [[likely]]
						{
							*dstValue = Type::NumberU;
							*dstValue->ptrNumberU = std::stoull(numberStr);
						}
					}
					catch (const std::out_of_range&)
					{
						*dstValue = Type::NumberF;
						*dstValue->ptrNumberF = std::stold(numberStr);
					}
				}
			}
			else [[unlikely]]
			{
				deserializeValueNumberAsString(srcStream, static_cast<std::string*>(nullptr), options);
			}
			break;
		default: [[unlikely]]
			throw std::runtime_error("json_deserialize_value_invalid");
		}
	}

	void deserializeValue(std::istream& srcStream, bool* dstValue, const Options& options)
	{
		uint32_t codePoint = 0;
		readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
		if (codePoint == 't') [[likely]]
		{
			readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
			if (codePoint == 'r') [[likely]]
			{
				readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
				if (codePoint == 'u') [[likely]]
				{
					readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
					if (codePoint == 'e') [[likely]]
					{
						if (dstValue) [[likely]]
						{
							*dstValue = true;
						}
						return;
					}
				}
			}
		}
		else if (codePoint == 'f') [[likely]]
		{
			readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
			if (codePoint == 'a') [[likely]]
			{
				readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
				if (codePoint == 'l') [[likely]]
				{
					readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
					if (codePoint == 's') [[likely]]
					{
						readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
						if (codePoint == 'e') [[likely]]
						{
							if (dstValue) [[likely]]
							{
								*dstValue = false;
							}
							return;
						}
					}
				}
			}
		}

			if (codePoint == JSON_EOF) [[likely]]
			{
				throw std::runtime_error("json_deserialize_boolean_truncated");
			}
			else [[unlikely]]
			{
				throw std::runtime_error("json_deserialize_boolean_invalid");
			}
	}

	void deserializeValue(std::istream& srcStream, int8_t* dstValue, const Options& options)
	{
		if (dstValue) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(srcStream, &number, options);
			try
			{
				*dstValue = static_cast<int8_t>(std::stol(number));
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(srcStream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& srcStream, uint8_t* dstValue, const Options& options)
	{
		if (dstValue) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(srcStream, &number, options);
			try
			{
				*dstValue = static_cast<uint8_t>(std::stoul(number));
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(srcStream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& srcStream, int16_t* dstValue, const Options& options)
	{
		if (dstValue) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(srcStream, &number, options);
			try
			{
				*dstValue = static_cast<int16_t>(std::stol(number));
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(srcStream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& srcStream, uint16_t* dstValue, const Options& options)
	{
		if (dstValue) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(srcStream, &number, options);
			try
			{
				*dstValue = static_cast<uint16_t>(std::stoul(number));
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(srcStream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& srcStream, int32_t* dstValue, const Options& options)
	{
		if (dstValue) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(srcStream, &number, options);
			try
			{
				*dstValue = std::stol(number);
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(srcStream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& srcStream, uint32_t* dstValue, const Options& options)
	{
		if (dstValue) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(srcStream, &number, options);
			try
			{
				*dstValue = std::stoul(number);
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(srcStream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& srcStream, int64_t* dstValue, const Options& options)
	{
		if (dstValue) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(srcStream, &number, options);
			try
			{
				*dstValue = std::stoll(number);
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(srcStream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& srcStream, uint64_t* dstValue, const Options& options)
	{
		if (dstValue) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(srcStream, &number, options);
			try
			{
				*dstValue = std::stoull(number);
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(srcStream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& srcStream, float* dstValue, const Options& options)
	{
		if (dstValue) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(srcStream, &number, options);
			try
			{
				*dstValue = std::stof(number);
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(srcStream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& srcStream, double* dstValue, const Options& options)
	{
		if (dstValue) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(srcStream, &number, options);
			try
			{
				*dstValue = std::stod(number);
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(srcStream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& srcStream, long double* dstValue, const Options& options)
	{
		if (dstValue) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(srcStream, &number, options);
			try
			{
				*dstValue = std::stold(number);
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(srcStream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& srcStream, std::string* dstValue, const Options& options)
	{
		uint32_t codePoint = 0;
		bool lastCharWasAntislash = false;
		readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
		if (codePoint == JSON_EOF) [[unlikely]]
		{
			throw std::runtime_error("json_deserialize_string_truncated");
		}
			if (codePoint != '"') [[unlikely]]
			{
				throw std::runtime_error("json_deserialize_string_notastring");
			}
				if (dstValue && !dstValue->empty()) [[likely]]
				{
					dstValue->clear();
				}
					while (true)
					{
						readChar(srcStream, codePoint, options.jsonEncoding);
						if (codePoint == JSON_EOF) [[unlikely]]
						{
							throw std::runtime_error("json_deserialize_string_truncated");
						}
							if (codePoint == '"' && !lastCharWasAntislash) [[unlikely]]
							{
								return;
							}
								if (codePoint == 'u' && lastCharWasAntislash) [[unlikely]]
								{
									if (dstValue) [[likely]]
									{
										serializeCodePointChar(*dstValue, codePoint, options.memberEncoding);
									}
									for (int32_t i = 0; i < 4; i++)
									{
										readChar(srcStream, codePoint, options.jsonEncoding);
										if (codePoint == JSON_EOF) [[unlikely]]
										{
											throw std::runtime_error("json_deserialize_string_truncated");
										}
											if (!(codePoint >= '0' && codePoint <= '9') || (codePoint >= 'A' && codePoint <= 'F') || (codePoint >= 'a' && codePoint <= 'f')) [[unlikely]]
											{
												throw std::runtime_error("json_deserialize_string_badunicodechar");
											}
												if (dstValue) [[likely]]
												{
													serializeCodePointChar(*dstValue, codePoint, options.memberEncoding);
												}
											continue;
									}
								}
									if (codePoint == '\\' && !lastCharWasAntislash) [[unlikely]]
									{
										lastCharWasAntislash = true;
									}
									else [[likely]]
									{
										lastCharWasAntislash = false;
									}
										if (dstValue) [[likely]]
										{
											serializeCodePointChar(*dstValue, codePoint, options.memberEncoding);
										}
					}
	}

	void deserializeValue(std::istream& srcStream, std::wstring* dstValue, const Options& options)
	{
		uint32_t codePoint = 0;
		bool lastCharWasAntislash = false;
		readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
		if (codePoint == JSON_EOF) [[unlikely]]
		{
			throw std::runtime_error("json_deserialize_wstring_truncated");
		}
			if (codePoint != '"') [[unlikely]]
			{
				throw std::runtime_error("json_deserialize_wstring_notastring");
			}
				if (dstValue && !dstValue->empty()) [[unlikely]]
				{
					dstValue->clear();
				}
					while (true)
					{
						readChar(srcStream, codePoint, options.jsonEncoding);
						if (codePoint == JSON_EOF) [[unlikely]]
						{
							throw std::runtime_error("json_deserialize_wstring_truncated");
						}
							if (codePoint == '"' && !lastCharWasAntislash) [[unlikely]]
							{
								return;
							}
								if (codePoint == 'u' && lastCharWasAntislash) [[unlikely]]
								{
									if (dstValue) [[likely]]
									{
										serializeCodePointChar(*dstValue, codePoint, options.memberEncoding);
									}
									for (int32_t i = 0; i < 4; i++)
									{
										readChar(srcStream, codePoint, options.jsonEncoding);
										if (codePoint == JSON_EOF) [[unlikely]]
										{
											throw std::runtime_error("json_deserialize_wstring_truncated");
										}
											if (!(codePoint >= '0' && codePoint <= '9') || (codePoint >= 'A' && codePoint <= 'F') || (codePoint >= 'a' && codePoint <= 'f')) [[unlikely]]
											{
												throw std::runtime_error("json_deserialize_wstring_badunicodechar");
											}
												if (dstValue) [[likely]]
												{
													serializeCodePointChar(*dstValue, codePoint, options.memberEncoding);
												}
											continue;
									}
								}
									if (codePoint == '\\' && !lastCharWasAntislash) [[unlikely]]
									{
										lastCharWasAntislash = true;
									}
									else [[likely]]
									{
										lastCharWasAntislash = false;
									}
										if (dstValue) [[unlikely]]
										{
											serializeCodePointChar(*dstValue, codePoint, options.memberEncoding);
										}
					}
	}

	void deserializeValueNumberAsString(std::istream& srcStream, std::string* dstValue, const Options& options)
	{
		uint32_t codePoint = 0;
		bool first = true;
		bool comma = false;
		bool e = false;
		bool plus = false;
		bool lastCharWasNumber = false;
		int32_t bytesCount = 0;
		while (true)
		{
			if (!first) [[likely]]
			{
				bytesCount = readChar(srcStream, codePoint, options.jsonEncoding);
			}
			else [[unlikely]]
			{
				bytesCount = readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
			}
				if (codePoint == JSON_EOF) [[unlikely]]
				{
					if (!lastCharWasNumber) [[unlikely]]
					{
						throw std::runtime_error("json_deserialize_number_truncated");
					}
					break;
				}
					if (codePoint >= '0' && codePoint <= '9') [[likely]]
					{
						lastCharWasNumber = true;
					}
					else if (codePoint == '-') [[unlikely]]
					{
						if (!e && !first) [[unlikely]]
						{
							throw std::runtime_error("json_deserialize_number_invalid(-)");
						}
					}
					else if (codePoint == '.') [[unlikely]]
					{
						if (comma) [[unlikely]]
						{
							throw std::runtime_error("json_deserialize_number_invalid(.)");
						}
						else [[likely]]
						{
							comma = true;
							lastCharWasNumber = false;
						}
					}
					else if ((codePoint == 'e' || codePoint == 'E') && !first && !e && lastCharWasNumber) [[unlikely]]
					{
						e = true;
					}
					else if (codePoint == '+' && !first && !plus) [[unlikely]]
					{
						plus = true;
					}
					else [[unlikely]]
					{
						JSON_UNGET(srcStream, bytesCount);
						break;
					}
						if (dstValue) [[likely]]
						{
							*dstValue += static_cast<char>(codePoint);
						}
					first = false;
		}
	}

	void deserializeValue(const Value& srcValue, Value* srcValue2, const Options&)
	{
		*srcValue2 = srcValue;
	}

	void deserializeValue(const Value& srcValue, bool* dstBoolean, const Options&)
	{
		if (srcValue == Type::Boolean) [[likely]]
		{
			*dstBoolean = *srcValue.ptrBoolean;
			return;
		}
		throw std::runtime_error("json_deserialize_boolean_badtype");
	}

	void deserializeValue(const Value& srcValue, int8_t* dstNumber, const Options&)
	{
		if (srcValue == Type::NumberI) [[likely]]
		{
			*dstNumber = static_cast<int8_t>(*srcValue.ptrNumberI);
			return;
		}
		else if (srcValue == Type::NumberU) [[unlikely]]
		{
			*dstNumber = static_cast<int8_t>(*srcValue.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_int8_badtype");
	}

	void deserializeValue(const Value& srcValue, int16_t* dstNumber, const Options&)
	{
		if (srcValue == Type::NumberI) [[likely]]
		{
			*dstNumber = static_cast<int16_t>(*srcValue.ptrNumberI);
			return;
		}
		else if (srcValue == Type::NumberU) [[unlikely]]
		{
			*dstNumber = static_cast<int16_t>(*srcValue.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_int16_badtype");
	}

	void deserializeValue(const Value& srcValue, int32_t* dstNumber, const Options&)
	{
		if (srcValue == Type::NumberI) [[likely]]
		{
			*dstNumber = static_cast<int32_t>(*srcValue.ptrNumberI);
			return;
		}
		else if (srcValue == Type::NumberU) [[unlikely]]
		{
			*dstNumber = static_cast<int32_t>(*srcValue.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_int32_badtype");
	}

	void deserializeValue(const Value& srcValue, int64_t* dstNumber, const Options&)
	{
		if (srcValue == Type::NumberI) [[likely]]
		{
			*dstNumber = static_cast<int64_t>(*srcValue.ptrNumberI);
			return;
		}
		else if (srcValue == Type::NumberU) [[unlikely]]
		{
			*dstNumber = static_cast<int64_t>(*srcValue.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_int64_badtype");
	}

	void deserializeValue(const Value& srcValue, uint8_t* dstNumber, const Options&)
	{
		if (srcValue == Type::NumberU) [[likely]]
		{
			*dstNumber = static_cast<uint8_t>(*srcValue.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_uint8_badtype");
	}

	void deserializeValue(const Value& srcValue, uint16_t* dstNumber, const Options&)
	{
		if (srcValue == Type::NumberU) [[likely]]
		{
			*dstNumber = static_cast<uint16_t>(*srcValue.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_uint16_badtype");
	}

	void deserializeValue(const Value& srcValue, uint32_t* dstNumber, const Options&)
	{
		if (srcValue == Type::NumberU) [[likely]]
		{
			*dstNumber = static_cast<uint32_t>(*srcValue.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_uint32_badtype");
	}

	void deserializeValue(const Value& srcValue, uint64_t* dstNumber, const Options&)
	{
		if (srcValue == Type::NumberU) [[likely]]
		{
			*dstNumber = static_cast<uint64_t>(*srcValue.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_uint64_badtype");
	}

	void deserializeValue(const Value& srcValue, float* dstNumber, const Options&)
	{
		if (srcValue == Type::NumberF) [[likely]]
		{
			*dstNumber = static_cast<float>(*srcValue.ptrNumberF);
			return;
		}
		throw std::runtime_error("json_deserialize_float_badtype");
	}

	void deserializeValue(const Value& srcValue, double* dstNumber, const Options&)
	{
		if (srcValue == Type::NumberF) [[likely]]
		{
			*dstNumber = static_cast<double>(*srcValue.ptrNumberF);
			return;
		}
		throw std::runtime_error("json_deserialize_double_badtype");
	}

	void deserializeValue(const Value& srcValue, long double* dstNumber, const Options&)
	{
		if (srcValue == Type::NumberF) [[likely]]
		{
			*dstNumber = *srcValue.ptrNumberF;
			return;
		}
		throw std::runtime_error("json_deserialize_longdouble_badtype");
	}

	void deserializeValue(const Value& srcValue, std::string* dstString, const Options& options)
	{
		if (srcValue == Type::String) [[likely]]
		{
			convert(*dstString, *srcValue.ptrString, options.memberEncoding, options.jsonEncoding);
			return;
		}
		throw std::runtime_error("json_deserialize_string_badtype");
	}

	void deserializeValue(const Value& srcValue, std::wstring* dstString, const Options& options)
	{
		if (srcValue == Type::String) [[likely]]
		{
			convert(*dstString, *srcValue.ptrString, options.memberEncoding, options.memberEncoding);
			return;
		}
		throw std::runtime_error("json_deserialize_string_badtype");
	}

#pragma endregion
}

std::istream& operator>>(std::istream& srcStream, JSON::Field& dstField)
{
	srcStream >> dstField.name;
	return srcStream;
}

std::ostream& operator<<(std::ostream& dstStream, const JSON::Field& srcField)
{
	dstStream << srcField.name;
	return dstStream;
}

#pragma GCC diagnostic pop
