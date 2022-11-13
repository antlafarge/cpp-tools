#include "json_definitions.h"
#include "json_implementations.h"

#include <iomanip>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

namespace JSON
{
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
		encoding = std::move(other.encoding);
		precision = std::move(other.precision);
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
		: name(other.name)
		, options(other.options)
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

	Value::Value(const bool value)
		: type(Type::Boolean)
		, boolean(new bool(value))
	{
	}

	Value::Value(const int8_t value)
		: type(Type::NumberInteger)
		, number_i(new int64_t(static_cast<int64_t>(value)))
	{
	}

	Value::Value(const int16_t value)
		: type(Type::NumberInteger)
		, number_i(new int64_t(static_cast<int64_t>(value)))
	{
	}

	Value::Value(const int32_t value)
		: type(Type::NumberInteger)
		, number_i(new int64_t(static_cast<int64_t>(value)))
	{
	}

	Value::Value(const int64_t value)
		: type(Type::NumberInteger)
		, number_i(new int64_t(value))
	{
	}

	Value::Value(const uint8_t value)
		: type(Type::NumberUInteger)
		, number_u(new uint64_t(static_cast<uint64_t>(value)))
	{
	}

	Value::Value(const uint16_t value)
		: type(Type::NumberUInteger)
		, number_u(new uint64_t(static_cast<uint64_t>(value)))
	{
	}

	Value::Value(const uint32_t value)
		: type(Type::NumberUInteger)
		, number_u(new uint64_t(static_cast<uint64_t>(value)))
	{
	}

	Value::Value(const uint64_t value)
		: type(Type::NumberUInteger)
		, number_u(new uint64_t(value))
	{
	}

	Value::Value(const float value)
		: type(Type::NumberFloat)
		, number_f(new long double(value))
	{
	}

	Value::Value(const double value)
		: type(Type::NumberFloat)
		, number_f(new long double(value))
	{
	}

	Value::Value(const long double value)
		: type(Type::NumberFloat)
		, number_f(new long double(value))
	{
	}

	Value::Value(const char* value)
		: type(Type::String)
		, string(new std::string(value))
	{
	}

	Value::Value(const std::string& value)
		: type(Type::String)
		, string(new std::string(value))
	{
	}

	Value::Value(const wchar_t* value, const Options& options)
		: type(Type::String)
		, string(new std::string())
	{
		utf16ToUtf8(*string, std::wstring(value), options.encoding);
	}

	Value::Value(const std::wstring& value, const Options& options)
		: type(Type::String)
		, string(new std::string())
	{
		utf16ToUtf8(*string, value, options.encoding);
	}

	Value::Value(const std::vector<Value>& value)
		: type(Type::Array)
		, array(new std::vector<Value>(value))
	{
	}

	Value::Value(const std::unordered_map<std::string, Value>& value)
		: type(Type::Object)
		, object(new std::unordered_map<std::string, Value>(value))
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
		type = std::move(other.type);
		ptr = std::move(other.ptr);
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
				boolean = new bool;
				break;
			case Type::NumberInteger: [[unlikely]]
				number_i = new int64_t;
				break;
			case Type::NumberUInteger: [[unlikely]]
				number_u = new uint64_t;
				break;
			case Type::NumberFloat: [[unlikely]]
				number_f = new long double;
				break;
			case Type::String: [[unlikely]]
				string = new std::string;
				break;
			case Type::Array: [[unlikely]]
				array = new std::vector<Value>;
				break;
			case Type::Object: [[unlikely]]
				object = new std::unordered_map<std::string, Value>;
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
		*boolean = value;
		return *this;
	}

	Value& Value::operator=(const int8_t value)
	{
		*this = Type::NumberInteger;
		*number_i = value;
		return *this;
	}

	Value& Value::operator=(const int16_t value)
	{
		*this = Type::NumberInteger;
		*number_i = value;
		return *this;
	}

	Value& Value::operator=(const int32_t value)
	{
		*this = Type::NumberInteger;
		*number_i = value;
		return *this;
	}

	Value& Value::operator=(const int64_t value)
	{
		*this = Type::NumberInteger;
		*number_i = value;
		return *this;
	}

	Value& Value::operator=(const uint8_t value)
	{
		*this = Type::NumberUInteger;
		*number_u = value;
		return *this;
	}

	Value& Value::operator=(const uint16_t value)
	{
		*this = Type::NumberUInteger;
		*number_u = value;
		return *this;
	}

	Value& Value::operator=(const uint32_t value)
	{
		*this = Type::NumberUInteger;
		*number_u = value;
		return *this;
	}

	Value& Value::operator=(const uint64_t value)
	{
		*this = Type::NumberUInteger;
		*number_u = value;
		return *this;
	}

	Value& Value::operator=(const float value)
	{
		*this = Type::NumberFloat;
		*number_f = value;
		return *this;
	}

	Value& Value::operator=(const double value)
	{
		*this = Type::NumberFloat;
		*number_f = value;
		return *this;
	}

	Value& Value::operator=(const long double value)
	{
		*this = Type::NumberFloat;
		*number_f = value;
		return *this;
	}

	Value& Value::operator=(const char* value)
	{
		*this = Type::String;
		*string = value;
		return *this;
	}

	Value& Value::operator=(const std::string& value)
	{
		*this = Type::String;
		*string = value;
		return *this;
	}

	Value& Value::operator=(const wchar_t* value)
	{
		*this = Type::String;
		utf16ToUtf8(*string, std::wstring(value));
		return *this;
	}

	Value& Value::operator=(const std::wstring& value)
	{
		*this = Type::String;
		utf16ToUtf8(*string, value);
		return *this;
	}

	Value& Value::operator=(const std::vector<Value>& value)
	{
		*this = Type::Array;
		*array = value;
		return *this;
	}

	Value& Value::operator=(const std::unordered_map<std::string, Value>& value)
	{
		*this = Type::Object;
		*object = value;
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
			return type == other.type && *boolean == *other.boolean;
		case Type::NumberInteger:
			switch (other.type)
			{
			case Type::NumberInteger:
				return *number_i == *other.number_i;
			case Type::NumberUInteger:
				if (*number_i < 0) return false;
				return *number_i == *other.number_i;
			case Type::NumberFloat:
				return *number_i == *other.number_f;
			default:
				return false;
			}
		case Type::NumberUInteger:
			switch (other.type)
			{
			case Type::NumberInteger:
				if (*other.number_i < 0) return false;
				return *number_u == *other.number_u;
			case Type::NumberUInteger:
				return *number_u == *other.number_u;
			case Type::NumberFloat:
				return *number_u == *other.number_f;
			default:
				return false;
			}
		case Type::NumberFloat:
			switch (other.type)
			{
			case Type::NumberInteger:
				return *number_f == *other.number_i;
			case Type::NumberUInteger:
				return *number_f == *other.number_u;
			case Type::NumberFloat:
				return *number_f == *other.number_f;
			default:
				return false;
			}
		case Type::String:
			return type == other.type && *string == *other.string;
		case Type::Array:
			return type == other.type && *array == *other.array;
		case Type::Object:
			return type == other.type && *object == *other.object;
		default:
			return false;
		}
	}

	bool Value::operator!=(const Value& right) const
	{
		return !(*this == right);
	}

	void Value::copyData(const Value& other)
	{
		switch (other.type)
		{
		case Type::Boolean:
			boolean = { new bool(*other.boolean) };
			break;
		case Type::NumberInteger:
			number_i = { new int64_t(*other.number_i) };
			break;
		case Type::NumberUInteger:
			number_u = { new uint64_t(*other.number_u) };
			break;
		case Type::NumberFloat:
			number_f = { new long double(*other.number_f) };
			break;
		case Type::String:
			string = { new std::string(*other.string) };
			break;
		case Type::Array:
			array = { new std::vector<Value>(*other.array) };
			break;
		case Type::Object:
			object = { new std::unordered_map<std::string, Value>(*other.object) };
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
			delete boolean;
			break;
		case Type::NumberInteger:
			delete number_i;
			break;
		case Type::NumberUInteger:
			delete number_u;
			break;
		case Type::NumberFloat:
			delete number_f;
			break;
		case Type::String:
			delete string;
			break;
		case Type::Array:
			delete array;
			break;
		case Type::Object:
			delete object;
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
			serializeValue(stream, value->boolean, options);
			break;
		case Type::NumberInteger:
			serializeValue(stream, value->number_i, options);
			break;
		case Type::NumberUInteger:
			serializeValue(stream, value->number_u, options);
			break;
		case Type::NumberFloat:
			serializeValue(stream, value->number_f, options);
			break;
		case Type::String:
			serializeValue(stream, value->string, options);
			break;
		case Type::Array:
			serializeValue(stream, value->array, options);
			break;
		case Type::Object:
			serializeValue(stream, value->object, options);
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

			if (options & Encoding::UTF8) [[likely]]
			{
				stream << '"' << *value << '"';
			}
			else [[unlikely]]
			{
				serializeUnicodeChar(stream, codePointToUnicode('"', options), options);
				for (char c : *value)
				{
					serializeUnicodeChar(stream, codePointToUnicode(c, options), options);
				}
				serializeUnicodeChar(stream, codePointToUnicode('"', options), options);
			}
	}

	void serializeValue(std::ostream& stream, const std::wstring* value, Options& options)
	{
		if (options == Encoding::None) [[unlikely]]
		{
			options.encoding = JSON_DEFAULT_ENCODING;
		}

			if (options & Encoding::UTF8) [[likely]]
			{
				stream << '"';
				utf16ToUtf8(stream, *value);
				stream << '"';
			}
			else [[unlikely]]
			{
				serializeUnicodeChar(stream, codePointToUnicode('"', options), options);
				std::ostringstream stream2;
				utf16ToUtf8(stream2, *value);
				std::istringstream stream3(stream2.str());
				Encoding encoding = Encoding::UTF8;
				while (true)
				{
					int32_t codePoint = 0;
					JSON_READ_CHAR(stream3, codePoint, encoding);
					if (codePoint == EOF)
					{
						break;
					}
					serializeUnicodeChar(stream, codePointToUnicode(codePoint, options), options);
				}
				serializeUnicodeChar(stream, codePointToUnicode('"', options), options);
			}
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
				deserializeValue(stream, value->boolean, options);
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
				deserializeValue(stream, value->string, options);
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
				deserializeValue(stream, value->array, options);
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
				deserializeValue(stream, value->object, options);
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
					*value = Type::NumberFloat;
					*value->number_f = std::stold(numberStr);
				}
				else [[likely]]
				{
					try
					{
						if (numberStr[0] == '-') [[unlikely]]
						{
							*value = Type::NumberInteger;
							*value->number_i = std::stoll(numberStr);
						}
						else [[likely]]
						{
							*value = Type::NumberUInteger;
							*value->number_u = std::stoull(numberStr);
						}
					}
					catch (const std::out_of_range&)
					{
						*value = Type::NumberFloat;
						*value->number_f = std::stold(numberStr);
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
			*boolean = (*value.boolean);
			return;
		}
		throw std::runtime_error("json_deserialize_boolean_badtype");
	}

	void deserializeValue(const Value& value, int8_t* number)
	{
		if (value == Type::NumberInteger) [[likely]]
		{
			*number = static_cast<int8_t>(*value.number_i);
			return;
		}
		else if (value == Type::NumberUInteger) [[unlikely]]
		{
			*number = static_cast<int8_t>(*value.number_u);
			return;
		}
		throw std::runtime_error("json_deserialize_int8_badtype");
	}

	void deserializeValue(const Value& value, int16_t* number)
	{
		if (value == Type::NumberInteger) [[likely]]
		{
			*number = static_cast<int16_t>(*value.number_i);
			return;
		}
		else if (value == Type::NumberUInteger) [[unlikely]]
		{
			*number = static_cast<int16_t>(*value.number_u);
			return;
		}
		throw std::runtime_error("json_deserialize_int16_badtype");
	}

	void deserializeValue(const Value& value, int32_t* number)
	{
		if (value == Type::NumberInteger) [[likely]]
		{
			*number = static_cast<int32_t>(*value.number_i);
			return;
		}
		else if (value == Type::NumberUInteger) [[unlikely]]
		{
			*number = static_cast<int32_t>(*value.number_u);
			return;
		}
		throw std::runtime_error("json_deserialize_int32_badtype");
	}

	void deserializeValue(const Value& value, int64_t* number)
	{
		if (value == Type::NumberInteger) [[likely]]
		{
			*number = static_cast<int64_t>(*value.number_i);
			return;
		}
		else if (value == Type::NumberUInteger) [[unlikely]]
		{
			*number = static_cast<int64_t>(*value.number_u);
			return;
		}
		throw std::runtime_error("json_deserialize_int64_badtype");
	}

	void deserializeValue(const Value& value, uint8_t* number)
	{
		if (value == Type::NumberUInteger) [[likely]]
		{
			*number = static_cast<uint8_t>(*value.number_u);
			return;
		}
		throw std::runtime_error("json_deserialize_uint8_badtype");
	}

	void deserializeValue(const Value& value, uint16_t* number)
	{
		if (value == Type::NumberUInteger) [[likely]]
		{
			*number = static_cast<uint16_t>(*value.number_u);
			return;
		}
		throw std::runtime_error("json_deserialize_uint16_badtype");
	}

	void deserializeValue(const Value& value, uint32_t* number)
	{
		if (value == Type::NumberUInteger) [[likely]]
		{
			*number = static_cast<uint32_t>(*value.number_u);
			return;
		}
		throw std::runtime_error("json_deserialize_uint32_badtype");
	}

	void deserializeValue(const Value& value, uint64_t* number)
	{
		if (value == Type::NumberUInteger) [[likely]]
		{
			*number = static_cast<uint64_t>(*value.number_u);
			return;
		}
		throw std::runtime_error("json_deserialize_uint64_badtype");
	}

	void deserializeValue(const Value& value, float* number)
	{
		if (value == Type::NumberFloat) [[likely]]
		{
			*number = (float)(*value.number_f);
			return;
		}
		throw std::runtime_error("json_deserialize_float_badtype");
	}

	void deserializeValue(const Value& value, double* number)
	{
		if (value == Type::NumberFloat) [[likely]]
		{
			*number = (double)(*value.number_f);
			return;
		}
		throw std::runtime_error("json_deserialize_double_badtype");
	}

	void deserializeValue(const Value& value, long double* number)
	{
		if (value == Type::NumberFloat) [[likely]]
		{
			*number = (*value.number_f);
			return;
		}
		throw std::runtime_error("json_deserialize_longdouble_badtype");
	}

	void deserializeValue(const Value& value, std::string* string)
	{
		if (value == Type::String) [[likely]]
		{
			*string = *value.string;
			return;
		}
		throw std::runtime_error("json_deserialize_string_badtype");
	}

	void deserializeValue(const Value& value, std::wstring* string)
	{
		if (value == Type::String) [[likely]]
		{
			utf8ToUtf16(*string, *value.string);
			return;
		}
		throw std::runtime_error("json_deserialize_string_badtype");
	}

#pragma endregion
}

#pragma GCC diagnostic pop
