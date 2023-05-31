#include "json_definitions.h"
#include "json_implementations.h"

#include <iomanip>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

namespace JSON
{
	Options Options::merge(const Options& optionsOverride, const Options& optionsDefault)
	{
		Options options;
		options.precision = optionsOverride.precision != -1 ? optionsOverride.precision : optionsDefault.precision;
		options.encoding = optionsOverride.encoding != Encoding::None ? optionsOverride.encoding : optionsDefault.encoding;
		return options;
	}

	Options::Options(Encoding encoding)
		: encoding(encoding)
	{
	}

	Options::Options(std::streamsize precision)
		: precision(precision)
	{
	}

	Options::Options(Encoding encoding, std::streamsize precision)
		: encoding(encoding)
		, precision(precision)
	{
	}

	Options::Options(std::streamsize precision, Encoding encoding)
		: encoding(encoding)
		, precision(precision)
	{
	}

	Options::Options(const Options& other)
		: encoding(other.encoding)
		, precision(other.precision)
	{
	}

	Options::Options(Options&& other) noexcept
		: encoding(other.encoding)
		, precision(other.precision)
	{
	}

	Options& Options::operator=(const Options& other)
	{
		encoding = other.encoding;
		precision = other.precision;
		return *this;
	}

	Options& Options::operator=(Options&& other) noexcept
	{
		encoding = other.encoding;
		precision = other.precision;
		return *this;
	}

	Options::operator Encoding() const
	{
		return encoding;
	}

	void Options::merge(const Options& options)
	{
		encoding = (encoding == Encoding::None && options.encoding != Encoding::None ? options.encoding : encoding);
		precision = (precision == -1 && options.precision != -1 ? options.precision : precision);
	}

	bool Options::hasFlags(Encoding flags) const
	{
		return JSON::hasFlags(encoding, flags);
	}

	Field::Field(const char* fieldName, const Options& options)
		: name(fieldName)
		, options(options)
	{
	}

	Field::Field(const std::string& fieldName, const Options& options)
		: name(fieldName)
		, options(options)
	{
	}

	Field::Field(std::string&& fieldName, const Options& options)
		: name(fieldName)
		, options(options)
	{
	}

	Field::Field(const Options& options)
		: options(options)
	{
	}

	Field::Field(const Field& other)
		: name(other.name)
		, options(other.options)
	{
	}

	Field::Field(Field&& other) noexcept
		: name(std::move(other.name))
		, options(std::move(other.options))
	{
	}

	Field& Field::operator=(const Field& right)
	{
		name = right.name;
		options = right.options;
		return *this;
	}

	Field& Field::operator=(Field&& right) noexcept
	{
		name = std::move(right.name);
		options = std::move(right.options);
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

	Value::Value(Type type)
		: type(Type::Null)
		, ptr(nullptr)
	{
		*this = type;
	}

	Value::Value(const bool value)
		: type(Type::Boolean)
		, ptrBoolean(new bool(value))
	{
	}

	Value::Value(const int8_t value)
		: type(Type::NumberI)
		, ptrNumberI(new int64_t(static_cast<int64_t>(value)))
	{
	}

	Value::Value(const int16_t value)
		: type(Type::NumberI)
		, ptrNumberI(new int64_t(static_cast<int64_t>(value)))
	{
	}

	Value::Value(const int32_t value)
		: type(Type::NumberI)
		, ptrNumberI(new int64_t(static_cast<int64_t>(value)))
	{
	}

	Value::Value(const int64_t value)
		: type(Type::NumberI)
		, ptrNumberI(new int64_t(value))
	{
	}

	Value::Value(const uint8_t value)
		: type(Type::NumberU)
		, ptrNumberU(new uint64_t(static_cast<uint64_t>(value)))
	{
	}

	Value::Value(const uint16_t value)
		: type(Type::NumberU)
		, ptrNumberU(new uint64_t(static_cast<uint64_t>(value)))
	{
	}

	Value::Value(const uint32_t value)
		: type(Type::NumberU)
		, ptrNumberU(new uint64_t(static_cast<uint64_t>(value)))
	{
	}

	Value::Value(const uint64_t value)
		: type(Type::NumberU)
		, ptrNumberU(new uint64_t(value))
	{
	}

	Value::Value(const float value)
		: type(Type::NumberF)
		, ptrNumberF(new long double(value))
	{
	}

	Value::Value(const double value)
		: type(Type::NumberF)
		, ptrNumberF(new long double(value))
	{
	}

	Value::Value(const long double value)
		: type(Type::NumberF)
		, ptrNumberF(new long double(value))
	{
	}

	Value::Value(const char* value)
		: type(Type::String)
		, ptrString(new std::string(value))
	{
	}

	Value::Value(const std::string& value)
		: type(Type::String)
		, ptrString(new std::string(value))
	{
	}

	Value::Value(const wchar_t* value, const Options& options)
		: type(Type::String)
		, ptrString(new std::string())
	{
		utf16ToUtf8(*ptrString, std::wstring(value), options.encoding);
	}

	Value::Value(const std::wstring& value, const Options& options)
		: type(Type::String)
		, ptrString(new std::string())
	{
		utf16ToUtf8(*ptrString, value, options.encoding);
	}

	Value::Value(const std::vector<Value>& value)
		: type(Type::Array)
		, ptrArray(new std::vector<Value>(value))
	{
	}

	Value::Value(const std::unordered_map<std::string, Value>& value)
		: type(Type::Object)
		, ptrObject(new std::unordered_map<Field, Value>())
	{
		ptrObject->reserve(value.size());
		for (auto& pair : value)
		{
			(*ptrObject)[pair.first] = pair.second;
		}
	}

	Value::Value(const std::unordered_map<Field, Value>& value)
		: type(Type::Object)
		, ptrObject(new std::unordered_map<Field, Value>(value))
	{
	}

	Value::Value(const Value& other)
	{
		copyData(other);
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
		deleteData();
		copyData(other);
		return *this;
	}

	Value& Value::operator=(Value&& other) noexcept
	{
		std::swap(type, other.type);
		std::swap(ptr, other.ptr);
		return *this;
	}

	Value& Value::operator=(const Type type2)
	{
		if (type != type2) [[unlikely]]
		{
			deleteData();
			switch (type2)
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
			type = type2;
		}
		return *this;
	}

	Value& Value::operator=(const bool value)
	{
		*this = Type::Boolean;
		*ptrBoolean = value;
		return *this;
	}

	Value& Value::operator=(const int8_t value)
	{
		*this = Type::NumberI;
		*ptrNumberI = value;
		return *this;
	}

	Value& Value::operator=(const int16_t value)
	{
		*this = Type::NumberI;
		*ptrNumberI = value;
		return *this;
	}

	Value& Value::operator=(const int32_t value)
	{
		*this = Type::NumberI;
		*ptrNumberI = value;
		return *this;
	}

	Value& Value::operator=(const int64_t value)
	{
		*this = Type::NumberI;
		*ptrNumberI = value;
		return *this;
	}

	Value& Value::operator=(const uint8_t value)
	{
		*this = Type::NumberU;
		*ptrNumberU = value;
		return *this;
	}

	Value& Value::operator=(const uint16_t value)
	{
		*this = Type::NumberU;
		*ptrNumberU = value;
		return *this;
	}

	Value& Value::operator=(const uint32_t value)
	{
		*this = Type::NumberU;
		*ptrNumberU = value;
		return *this;
	}

	Value& Value::operator=(const uint64_t value)
	{
		*this = Type::NumberU;
		*ptrNumberU = value;
		return *this;
	}

	Value& Value::operator=(const float value)
	{
		*this = Type::NumberF;
		*ptrNumberF = value;
		return *this;
	}

	Value& Value::operator=(const double value)
	{
		*this = Type::NumberF;
		*ptrNumberF = value;
		return *this;
	}

	Value& Value::operator=(const long double value)
	{
		*this = Type::NumberF;
		*ptrNumberF = value;
		return *this;
	}

	Value& Value::operator=(const char* value)
	{
		*this = Type::String;
		*ptrString = value;
		return *this;
	}

	Value& Value::operator=(const std::string& value)
	{
		*this = Type::String;
		*ptrString = value;
		return *this;
	}

	Value& Value::operator=(const wchar_t* value)
	{
		*this = Type::String;
		utf16ToUtf8(*ptrString, std::wstring(value));
		return *this;
	}

	Value& Value::operator=(const std::wstring& value)
	{
		*this = Type::String;
		utf16ToUtf8(*ptrString, value);
		return *this;
	}

	Value& Value::operator=(const std::vector<Value>& value)
	{
		*this = Type::Array;
		*ptrArray = value;
		return *this;
	}

	Value& Value::operator=(const std::unordered_map<std::string, Value>& value)
	{
		*this = Type::Object;
		ptrObject->reserve(value.size());
		for (auto& pair : value)
		{
			(*ptrObject)[pair.first] = pair.second;
		}
		return *this;
	}

	Value& Value::operator=(const std::unordered_map<Field, Value>& value)
	{
		*this = Type::Object;
		*ptrObject = value;
		return *this;
	}

	bool Value::operator==(const Type type2) const
	{
		return type == type2;
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
				return *ptrNumberI == *other.ptrNumberF;
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
				return *ptrNumberF == *other.ptrNumberI;
			case Type::NumberU:
				return *ptrNumberF == *other.ptrNumberU;
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

	bool Value::operator!=(const Value& right) const
	{
		return !(*this == right);
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

	void Value::copyData(const Value& other)
	{
		switch (other.type)
		{
		case Type::Boolean:
			ptrBoolean = { new bool(*other.ptrBoolean) };
			break;
		case Type::NumberI:
			ptrNumberI = { new int64_t(*other.ptrNumberI) };
			break;
		case Type::NumberU:
			ptrNumberU = { new uint64_t(*other.ptrNumberU) };
			break;
		case Type::NumberF:
			ptrNumberF = { new long double(*other.ptrNumberF) };
			break;
		case Type::String:
			ptrString = { new std::string(*other.ptrString) };
			break;
		case Type::Array:
			ptrArray = { new std::vector<Value>(*other.ptrArray) };
			break;
		case Type::Object:
			ptrObject = { new std::unordered_map<Field, Value>(*other.ptrObject) };
			break;
		case Type::Null:
		default:
			break;
		}
		type = other.type;
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

	void serializeValue(std::ostream& stream, const Value* value, Options& options)
	{
		switch (value->type)
		{
		case Type::Null:
		{
			std::optional<bool> null;
			serializeValue(stream, &null, options);
			break;
		}
		case Type::Boolean:
			serializeValue(stream, value->ptrBoolean, options);
			break;
		case Type::NumberI:
			serializeValue(stream, value->ptrNumberI, options);
			break;
		case Type::NumberU:
			serializeValue(stream, value->ptrNumberU, options);
			break;
		case Type::NumberF:
			serializeValue(stream, value->ptrNumberF, options);
			break;
		case Type::String:
			serializeValue(stream, value->ptrString, options);
			break;
		case Type::Array:
			serializeValue(stream, value->ptrArray, options);
			break;
		case Type::Object:
			serializeValue(stream, value->ptrObject, options);
			break;
		default: [[unlikely]]
			throw std::runtime_error("json_serialize_value_badtype");
		}
	}

	void serializeValue(std::ostream& stream, const bool* value, Options& options)
	{
		if (*value)
		{
			serializeUnicodeChar(stream, codePointToUnicode('t', options), options);
			serializeUnicodeChar(stream, codePointToUnicode('r', options), options);
			serializeUnicodeChar(stream, codePointToUnicode('u', options), options);
			serializeUnicodeChar(stream, codePointToUnicode('e', options), options);
		}
		else
		{
			serializeUnicodeChar(stream, codePointToUnicode('f', options), options);
			serializeUnicodeChar(stream, codePointToUnicode('a', options), options);
			serializeUnicodeChar(stream, codePointToUnicode('l', options), options);
			serializeUnicodeChar(stream, codePointToUnicode('s', options), options);
			serializeUnicodeChar(stream, codePointToUnicode('e', options), options);
		}
	}

	void serializeValue(std::ostream& stream, const int8_t* value, Options& options)
	{
		int64_t valueNumber = static_cast<int64_t>(*value);
		serializeValueInteger(stream, &valueNumber, options);
	}

	void serializeValue(std::ostream& stream, const uint8_t* value, Options& options)
	{
		int64_t valueNumber = static_cast<uint64_t>(*value);
		serializeValueInteger(stream, &valueNumber, options);
	}

	void serializeValue(std::ostream& stream, const int16_t* value, Options& options)
	{
		int64_t valueNumber = static_cast<int64_t>(*value);
		serializeValueInteger(stream, &valueNumber, options);
	}

	void serializeValue(std::ostream& stream, const uint16_t* value, Options& options)
	{
		int64_t valueNumber = static_cast<uint64_t>(*value);
		serializeValueInteger(stream, &valueNumber, options);
	}

	void serializeValue(std::ostream& stream, const int32_t* value, Options& options)
	{
		int64_t valueNumber = static_cast<int64_t>(*value);
		serializeValueInteger(stream, &valueNumber, options);
	}

	void serializeValue(std::ostream& stream, const uint32_t* value, Options& options)
	{
		int64_t valueNumber = static_cast<uint64_t>(*value);
		serializeValueInteger(stream, &valueNumber, options);
	}

	void serializeValue(std::ostream& stream, const int64_t* value, Options& options)
	{
		serializeValueInteger(stream, value, options);
	}

	void serializeValue(std::ostream& stream, const uint64_t* value, Options& options)
	{
		serializeValueInteger(stream, value, options);
	}

	void serializeValue(std::ostream& stream, const float* value, Options& options)
	{
		serializeValueFloat(stream, value, options);
	}

	void serializeValue(std::ostream& stream, const double* value, Options& options)
	{
		serializeValueFloat(stream, value, options);
	}

	void serializeValue(std::ostream& stream, const long double* value, Options& options)
	{
		serializeValueFloat(stream, value, options);
	}

	void serializeValue(std::ostream& stream, const std::string* value, Options& options)
	{
		if (options == Encoding::None) [[unlikely]]
		{
			options.encoding = JSON_DEFAULT_ENCODING;
		}

		serializeUnicodeChar(stream, codePointToUnicode('"', options), options);
		if (options.hasFlags(Encoding::UTF8)) [[likely]]
		{
			stream << *value;
		}
		else [[unlikely]]
		{
			std::istringstream stream2(*value);
			int32_t codePoint = 0;
			Encoding encoding = Encoding::UTF8;
			while (true)
			{
				JSON_READ_CHAR(stream2, codePoint, encoding);
				if (codePoint == EOF)
				{
					break;
				}
				serializeUnicodeChar(stream, codePointToUnicode(codePoint, options), options);
			}
		}
		serializeUnicodeChar(stream, codePointToUnicode('"', options), options);
	}

	void serializeValue(std::ostream& stream, const std::wstring* value, Options& options)
	{
		if (options == Encoding::None) [[unlikely]]
		{
			options.encoding = JSON_DEFAULT_ENCODING;
		}

		serializeUnicodeChar(stream, codePointToUnicode('"', options), options);
		if (options.hasFlags(Encoding::UTF8)) [[likely]]
		{
			utf16ToUtf8(stream, *value);
		}
		else [[unlikely]]
		{
			std::stringstream stream2;
			utf16ToUtf8(stream2, *value);
			stream2.seekg(0, stream2.beg);
			int32_t codePoint = 0;
			Encoding encoding = Encoding::UTF8;
			while (true)
			{
				JSON_READ_CHAR(stream2, codePoint, encoding);
				if (codePoint == EOF)
				{
					break;
				}
				serializeUnicodeChar(stream, codePointToUnicode(codePoint, options), options);
			}
		}
		serializeUnicodeChar(stream, codePointToUnicode('"', options), options);
	}

#pragma endregion

#pragma region Deserialization

	bool isValid(const std::string& json, const Options& options) noexcept
	{
		std::istringstream stream(json);
		return isValid(stream, options);
	}

	bool isValid(std::istream&& stream, const Options& options) noexcept
	{
		std::istream stream2(stream.rdbuf());
		return isValid(stream2, options);
	}

	bool isValid(std::istream& stream, const Options& options) noexcept
	{
		try
		{
			Options options2(options);
			deserializeValue(stream, static_cast<Value*>(nullptr), options2);
			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	void deserializeFieldValue(const Value&)
	{
	}

	void deserializeValue(std::istream& stream, Value* value, Options& options)
	{
		int codePoint = 0;
		int bytesCount = JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
		if (codePoint == EOF) [[unlikely]]
		{
			throw std::runtime_error("json_deserialize_value_truncated");
		}
		JSON_UNGET(stream, bytesCount);
		switch (codePoint)
		{
		case 'n':
		{
			deserializeValue(stream, static_cast<std::optional<bool>*>(nullptr), options);
			if (value) [[likely]]
			{
				*value = Type::Null;
			}
			break;
		}
		case 't':
		case 'f':
			if (value) [[likely]]
			{
				*value = Type::Boolean;
				deserializeValue(stream, value->ptrBoolean, options);
			}
			else [[unlikely]]
			{
				deserializeValue(stream, static_cast<bool*>(nullptr), options);
			}
			break;
		case '"':
			if (value) [[likely]]
			{
				*value = Type::String;
				deserializeValue(stream, value->ptrString, options);
			}
			else [[unlikely]]
			{
				deserializeValue(stream, static_cast<std::string*>(nullptr), options);
			}
			break;
		case '[':
			if (value) [[likely]]
			{
				*value = Type::Array;
				deserializeValue(stream, value->ptrArray, options);
			}
			else [[unlikely]]
			{
				deserializeValue(stream, static_cast<std::vector<Value>*>(nullptr), options);
			}
			break;
		case '{':
			if (value) [[likely]]
			{
				*value = Type::Object;
				deserializeValue(stream, value->ptrObject, options);
			}
			else [[unlikely]]
			{
				deserializeValue(stream, static_cast<std::unordered_map<std::string, Value>*>(nullptr), options);
			}
			break;
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '-':
			if (value) [[likely]]
			{
				std::string numberStr;
				deserializeValueNumberAsString(stream, &numberStr, options);
				if (numberStr.find('.') != std::string::npos) [[unlikely]]
				{
					*value = Type::NumberF;
					*value->ptrNumberF = std::stold(numberStr);
				}
				else [[likely]]
				{
					try
					{
						if (numberStr[0] == '-') [[unlikely]]
						{
							*value = Type::NumberI;
							*value->ptrNumberI = std::stoll(numberStr);
						}
						else [[likely]]
						{
							*value = Type::NumberU;
							*value->ptrNumberU = std::stoull(numberStr);
						}
					}
					catch (const std::out_of_range&)
					{
						*value = Type::NumberF;
						*value->ptrNumberF = std::stold(numberStr);
					}
				}
			}
			else [[unlikely]]
			{
				deserializeValueNumberAsString(stream, static_cast<std::string*>(nullptr), options);
			}
			break;
		default: [[unlikely]]
			throw std::runtime_error("json_deserialize_value_invalid");
		}
	}

	void deserializeValue(std::istream& stream, bool* value, Options& options)
	{
		int32_t codePoint = 0;
		JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
		if (codePoint == 't') [[likely]]
		{
			JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
			if (codePoint == 'r') [[likely]]
			{
				JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
				if (codePoint == 'u') [[likely]]
				{
					JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
					if (codePoint == 'e') [[likely]]
					{
						if (value) [[likely]]
						{
							*value = true;
						}
						return;
					}
				}
			}
		}
		else if (codePoint == 'f') [[likely]]
		{
			JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
			if (codePoint == 'a') [[likely]]
			{
				JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
				if (codePoint == 'l') [[likely]]
				{
					JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
					if (codePoint == 's') [[likely]]
					{
						JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
						if (codePoint == 'e') [[likely]]
						{
							if (value) [[likely]]
							{
								*value = false;
							}
							return;
						}
					}
				}
			}
		}

			if (codePoint == EOF) [[likely]]
			{
				throw std::runtime_error("json_deserialize_boolean_truncated");
			}
			else [[unlikely]]
			{
				throw std::runtime_error("json_deserialize_boolean_invalid");
			}
	}

	void deserializeValue(std::istream& stream, int8_t* value, Options& options)
	{
		if (value) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(stream, &number, options);
			try
			{
				*value = static_cast<int8_t>(std::stol(number));
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(stream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& stream, uint8_t* value, Options& options)
	{
		if (value) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(stream, &number, options);
			try
			{
				*value = static_cast<uint8_t>(std::stoul(number));
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(stream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& stream, int16_t* value, Options& options)
	{
		if (value) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(stream, &number, options);
			try
			{
				*value = static_cast<int16_t>(std::stol(number));
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(stream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& stream, uint16_t* value, Options& options)
	{
		if (value) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(stream, &number, options);
			try
			{
				*value = static_cast<uint16_t>(std::stoul(number));
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(stream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& stream, int32_t* value, Options& options)
	{
		if (value) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(stream, &number, options);
			try
			{
				*value = std::stol(number);
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(stream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& stream, uint32_t* value, Options& options)
	{
		if (value) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(stream, &number, options);
			try
			{
				*value = std::stoul(number);
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(stream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& stream, int64_t* value, Options& options)
	{
		if (value) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(stream, &number, options);
			try
			{
				*value = std::stoll(number);
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(stream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& stream, uint64_t* value, Options& options)
	{
		if (value) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(stream, &number, options);
			try
			{
				*value = std::stoull(number);
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(stream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& stream, float* value, Options& options)
	{
		if (value) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(stream, &number, options);
			try
			{
				*value = std::stof(number);
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(stream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& stream, double* value, Options& options)
	{
		if (value) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(stream, &number, options);
			try
			{
				*value = std::stod(number);
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(stream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& stream, long double* value, Options& options)
	{
		if (value) [[likely]]
		{
			std::string number;
			deserializeValueNumberAsString(stream, &number, options);
			try
			{
				*value = std::stold(number);
			}
			catch (const std::exception&)
			{
				throw std::runtime_error("json_deserialize_number_invalid");
			}
		}
		else [[unlikely]]
		{
			deserializeValueNumberAsString(stream, nullptr, options);
		}
	}

	void deserializeValue(std::istream& stream, std::string* value, Options& options)
	{
		int32_t codePoint = 0;
		bool lastCharWasAntislash = false;
		JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
		if (codePoint == EOF) [[unlikely]]
		{
			throw std::runtime_error("json_deserialize_string_truncated");
		}
			if (codePoint != '"') [[unlikely]]
			{
				throw std::runtime_error("json_deserialize_string_notastring");
			}
				if (value) [[likely]]
				{
					value->clear();
				}
					while (true)
					{
						JSON_READ_CHAR(stream, codePoint, options.encoding);
						if (codePoint == EOF) [[unlikely]]
						{
							throw std::runtime_error("json_deserialize_string_truncated");
						}
							if (codePoint == '"' && !lastCharWasAntislash) [[unlikely]]
							{
								return;
							}
								if (codePoint == 'u' && lastCharWasAntislash) [[unlikely]]
								{
									if (value) [[likely]]
									{
										serializeUnicodeChar(*value, codePointToUnicode(codePoint, JSON_DEFAULT_ENCODING), JSON_DEFAULT_ENCODING);
									}
									for (int i = 0; i < 4; i++)
									{
										JSON_READ_CHAR(stream, codePoint, options.encoding);
										if (codePoint == EOF) [[unlikely]]
										{
											throw std::runtime_error("json_deserialize_string_truncated");
										}
											if (!(codePoint >= '0' && codePoint <= '9') || (codePoint >= 'A' && codePoint <= 'F') || (codePoint >= 'a' && codePoint <= 'f')) [[unlikely]]
											{
												throw std::runtime_error("json_deserialize_string_badunicodechar");
											}
												if (value) [[likely]]
												{
													serializeUnicodeChar(*value, codePointToUnicode(codePoint, JSON_DEFAULT_ENCODING), JSON_DEFAULT_ENCODING);
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
										if (value) [[likely]]
										{
											serializeUnicodeChar(*value, codePointToUnicode(codePoint, JSON_DEFAULT_ENCODING), JSON_DEFAULT_ENCODING);
										}
					}
	}

	void deserializeValue(std::istream& stream, std::wstring* value, Options& options)
	{
		int32_t codePoint = 0;
		bool lastCharWasAntislash = false;
		JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
		if (codePoint == EOF) [[unlikely]]
		{
			throw std::runtime_error("json_deserialize_wstring_truncated");
		}
			if (codePoint != '"') [[unlikely]]
			{
				throw std::runtime_error("json_deserialize_wstring_notastring");
			}
				if (value) [[likely]]
				{
					value->clear();
				}
					while (true)
					{
						JSON_READ_CHAR(stream, codePoint, options.encoding);
						if (codePoint == EOF) [[unlikely]]
						{
							throw std::runtime_error("json_deserialize_wstring_truncated");
						}
							if (codePoint == '"' && !lastCharWasAntislash) [[unlikely]]
							{
								return;
							}
								if (codePoint == 'u' && lastCharWasAntislash) [[unlikely]]
								{
									if (value) [[likely]]
									{
										serializeUnicodeChar(*value, codePointToUnicode(codePoint, JSON_DEFAULT_ENCODING_WSTRING), JSON_DEFAULT_ENCODING_WSTRING);
									}
									for (int i = 0; i < 4; i++)
									{
										JSON_READ_CHAR(stream, codePoint, options.encoding);
										if (codePoint == EOF) [[unlikely]]
										{
											throw std::runtime_error("json_deserialize_wstring_truncated");
										}
											if (!(codePoint >= '0' && codePoint <= '9') || (codePoint >= 'A' && codePoint <= 'F') || (codePoint >= 'a' && codePoint <= 'f')) [[unlikely]]
											{
												throw std::runtime_error("json_deserialize_wstring_badunicodechar");
											}
												if (value) [[likely]]
												{
													serializeUnicodeChar(*value, codePointToUnicode(codePoint, JSON_DEFAULT_ENCODING_WSTRING), JSON_DEFAULT_ENCODING_WSTRING);
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
										if (value) [[unlikely]]
										{
											serializeUnicodeChar(*value, codePointToUnicode(codePoint, JSON_DEFAULT_ENCODING_WSTRING), JSON_DEFAULT_ENCODING_WSTRING);
										}
					}
	}

	void deserializeValueNumberAsString(std::istream& stream, std::string* value, Options& options)
	{
		int32_t codePoint = 0;
		bool first = true;
		bool comma = false;
		bool e = false;
		bool plus = false;
		bool lastCharWasNumber = false;
		int bytesCount = 0;
		while (true)
		{
			if (!first) [[likely]]
			{
				bytesCount = JSON_READ_CHAR(stream, codePoint, options.encoding);
			}
			else [[unlikely]]
			{
				bytesCount = JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
			}
				if (codePoint == EOF) [[unlikely]]
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
						JSON_UNGET(stream, bytesCount);
						break;
					}
						if (value) [[likely]]
						{
							*value += static_cast<char>(codePoint);
						}
					first = false;
		}
	}

	void deserializeValue(const Value& value, Value* value2)
	{
		*value2 = value;
	}

	void deserializeValue(const Value& value, bool* boolean)
	{
		if (value == Type::Boolean) [[likely]]
		{
			*boolean = (*value.ptrBoolean);
			return;
		}
		throw std::runtime_error("json_deserialize_boolean_badtype");
	}

	void deserializeValue(const Value& value, int8_t* number)
	{
		if (value == Type::NumberI) [[likely]]
		{
			*number = static_cast<int8_t>(*value.ptrNumberI);
			return;
		}
		else if (value == Type::NumberU) [[unlikely]]
		{
			*number = static_cast<int8_t>(*value.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_int8_badtype");
	}

	void deserializeValue(const Value& value, int16_t* number)
	{
		if (value == Type::NumberI) [[likely]]
		{
			*number = static_cast<int16_t>(*value.ptrNumberI);
			return;
		}
		else if (value == Type::NumberU) [[unlikely]]
		{
			*number = static_cast<int16_t>(*value.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_int16_badtype");
	}

	void deserializeValue(const Value& value, int32_t* number)
	{
		if (value == Type::NumberI) [[likely]]
		{
			*number = static_cast<int32_t>(*value.ptrNumberI);
			return;
		}
		else if (value == Type::NumberU) [[unlikely]]
		{
			*number = static_cast<int32_t>(*value.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_int32_badtype");
	}

	void deserializeValue(const Value& value, int64_t* number)
	{
		if (value == Type::NumberI) [[likely]]
		{
			*number = static_cast<int64_t>(*value.ptrNumberI);
			return;
		}
		else if (value == Type::NumberU) [[unlikely]]
		{
			*number = static_cast<int64_t>(*value.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_int64_badtype");
	}

	void deserializeValue(const Value& value, uint8_t* number)
	{
		if (value == Type::NumberU) [[likely]]
		{
			*number = static_cast<uint8_t>(*value.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_uint8_badtype");
	}

	void deserializeValue(const Value& value, uint16_t* number)
	{
		if (value == Type::NumberU) [[likely]]
		{
			*number = static_cast<uint16_t>(*value.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_uint16_badtype");
	}

	void deserializeValue(const Value& value, uint32_t* number)
	{
		if (value == Type::NumberU) [[likely]]
		{
			*number = static_cast<uint32_t>(*value.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_uint32_badtype");
	}

	void deserializeValue(const Value& value, uint64_t* number)
	{
		if (value == Type::NumberU) [[likely]]
		{
			*number = static_cast<uint64_t>(*value.ptrNumberU);
			return;
		}
		throw std::runtime_error("json_deserialize_uint64_badtype");
	}

	void deserializeValue(const Value& value, float* number)
	{
		if (value == Type::NumberF) [[likely]]
		{
			*number = (float)(*value.ptrNumberF);
			return;
		}
		throw std::runtime_error("json_deserialize_float_badtype");
	}

	void deserializeValue(const Value& value, double* number)
	{
		if (value == Type::NumberF) [[likely]]
		{
			*number = (double)(*value.ptrNumberF);
			return;
		}
		throw std::runtime_error("json_deserialize_double_badtype");
	}

	void deserializeValue(const Value& value, long double* number)
	{
		if (value == Type::NumberF) [[likely]]
		{
			*number = (*value.ptrNumberF);
			return;
		}
		throw std::runtime_error("json_deserialize_longdouble_badtype");
	}

	void deserializeValue(const Value& value, std::string* string)
	{
		if (value == Type::String) [[likely]]
		{
			*string = *value.ptrString;
			return;
		}
		throw std::runtime_error("json_deserialize_string_badtype");
	}

	void deserializeValue(const Value& value, std::wstring* string)
	{
		if (value == Type::String) [[likely]]
		{
			utf8ToUtf16(*string, *value.ptrString);
			return;
		}
		throw std::runtime_error("json_deserialize_string_badtype");
	}

#pragma endregion
}

std::istream& operator>>(std::istream& stream, JSON::Field& field)
{
	stream >> field.name;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const JSON::Field& field)
{
	stream << field.name;
	return stream;
}

#pragma GCC diagnostic pop
