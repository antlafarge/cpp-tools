#pragma once

#include "json_unicode.h"

#include <array>
#include <deque>
#include <forward_list>
#include <fstream>
#include <functional>
#include <list>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

#define JSON_EXPAND(x) x
#define JSON_FOR_EACH_0(what, ...)
#define JSON_FOR_EACH_2(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_0(what,  __VA_ARGS__))
#define JSON_FOR_EACH_4(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_2(what,  __VA_ARGS__))
#define JSON_FOR_EACH_6(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_4(what,  __VA_ARGS__))
#define JSON_FOR_EACH_8(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_6(what,  __VA_ARGS__))
#define JSON_FOR_EACH_10(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_8(what,  __VA_ARGS__))
#define JSON_FOR_EACH_12(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_10(what,  __VA_ARGS__))
#define JSON_FOR_EACH_14(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_12(what,  __VA_ARGS__))
#define JSON_FOR_EACH_16(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_14(what,  __VA_ARGS__))
#define JSON_FOR_EACH_18(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_16(what,  __VA_ARGS__))
#define JSON_FOR_EACH_20(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_18(what,  __VA_ARGS__))
#define JSON_FOR_EACH_22(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_20(what,  __VA_ARGS__))
#define JSON_FOR_EACH_24(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_22(what,  __VA_ARGS__))
#define JSON_FOR_EACH_26(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_24(what,  __VA_ARGS__))
#define JSON_FOR_EACH_28(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_26(what,  __VA_ARGS__))
#define JSON_FOR_EACH_30(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_28(what,  __VA_ARGS__))
#define JSON_FOR_EACH_32(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_30(what,  __VA_ARGS__))
#define JSON_FOR_EACH_34(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_32(what,  __VA_ARGS__))
#define JSON_FOR_EACH_36(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_34(what,  __VA_ARGS__))
#define JSON_FOR_EACH_38(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_36(what,  __VA_ARGS__))
#define JSON_FOR_EACH_40(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_38(what,  __VA_ARGS__))
#define JSON_FOR_EACH_42(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_40(what,  __VA_ARGS__))
#define JSON_FOR_EACH_44(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_42(what,  __VA_ARGS__))
#define JSON_FOR_EACH_46(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_44(what,  __VA_ARGS__))
#define JSON_FOR_EACH_48(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_46(what,  __VA_ARGS__))
#define JSON_FOR_EACH_50(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_48(what,  __VA_ARGS__))
#define JSON_FOR_EACH_52(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_50(what,  __VA_ARGS__))
#define JSON_FOR_EACH_54(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_52(what,  __VA_ARGS__))
#define JSON_FOR_EACH_56(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_54(what,  __VA_ARGS__))
#define JSON_FOR_EACH_58(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_56(what,  __VA_ARGS__))
#define JSON_FOR_EACH_60(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_58(what,  __VA_ARGS__))
#define JSON_FOR_EACH_62(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_60(what,  __VA_ARGS__))
#define JSON_FOR_EACH_64(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_62(what,  __VA_ARGS__))
#define JSON_FOR_EACH_66(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_64(what,  __VA_ARGS__))
#define JSON_FOR_EACH_68(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_66(what,  __VA_ARGS__))
#define JSON_FOR_EACH_70(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_68(what,  __VA_ARGS__))
#define JSON_FOR_EACH_72(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_70(what,  __VA_ARGS__))
#define JSON_FOR_EACH_74(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_72(what,  __VA_ARGS__))
#define JSON_FOR_EACH_76(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_74(what,  __VA_ARGS__))
#define JSON_FOR_EACH_78(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_76(what,  __VA_ARGS__))
#define JSON_FOR_EACH_80(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_78(what,  __VA_ARGS__))
#define JSON_FOR_EACH_82(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_80(what,  __VA_ARGS__))
#define JSON_FOR_EACH_84(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_82(what,  __VA_ARGS__))
#define JSON_FOR_EACH_86(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_84(what,  __VA_ARGS__))
#define JSON_FOR_EACH_88(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_86(what,  __VA_ARGS__))
#define JSON_FOR_EACH_90(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_88(what,  __VA_ARGS__))
#define JSON_FOR_EACH_92(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_90(what,  __VA_ARGS__))
#define JSON_FOR_EACH_94(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_92(what,  __VA_ARGS__))
#define JSON_FOR_EACH_96(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_94(what,  __VA_ARGS__))
#define JSON_FOR_EACH_98(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_96(what,  __VA_ARGS__))
#define JSON_FOR_EACH_100(what, x, y, ...) what(x, y) JSON_EXPAND(JSON_FOR_EACH_98(what,  __VA_ARGS__))

#ifdef _MSC_VER // Microsoft compilers
#   define JSON_ARGS_COUNT(...) JSON_EXPAND_ARGS_PRIVATE(JSON_ARGS_AUGMENTER(__VA_ARGS__))
#   define JSON_ARGS_AUGMENTER(...) unused, __VA_ARGS__
#   define JSON_EXPAND_ARGS_PRIVATE(...) JSON_EXPAND(JSON_ARGS_COUNT_PRIVATE(__VA_ARGS__, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#   define JSON_ARGS_COUNT_PRIVATE(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, unused, count, ...) count
#else // Non-Microsoft compilers
#   define JSON_ARGS_COUNT(...) JSON_ARGS_COUNT_PRIVATE(0, ## __VA_ARGS__, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#   define JSON_ARGS_COUNT_PRIVATE(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, count, ...) count
#endif

#define JSON_CONCATENATE(x,y) x##y
#define JSON_FOR_EACH_(N, what, ...) JSON_EXPAND(JSON_CONCATENATE(JSON_FOR_EACH_, N)(what, __VA_ARGS__))
#define JSON_FOR_EACH(what, ...) JSON_FOR_EACH_(JSON_ARGS_COUNT(__VA_ARGS__), what, __VA_ARGS__)

#define JSON_FIELD_VALUE(field, value) , JSON::FieldValue<decltype(value)>(field, value)
#define JSON_FIELD_VALUE_CONST(field, value) , JSON::FieldValue<const decltype(value)>(field, value)

#define JSON(...) \
void toJSON(std::ostream& dstStream, const JSON::Options& options = JSON::Options()) const \
{ \
	JSON::serializeCodePointChar(dstStream, '{', options.jsonEncoding); \
	JSON::serializeFieldValue(dstStream, options JSON_FOR_EACH(JSON_FIELD_VALUE_CONST, __VA_ARGS__)); \
	JSON::serializeCodePointChar(dstStream, '}', options.jsonEncoding); \
} \
\
void fromJSON(std::istream& srcStream, const JSON::Options& options = JSON::Options()) \
{ \
	JSON::Value dstValue; \
	JSON::deserialize(srcStream, dstValue, options); \
	fromJSON(dstValue, options); \
} \
\
void fromJSON(const JSON::Value& srcValue, const JSON::Options& options = JSON::Options()) \
{ \
	JSON::deserializeFieldValue(srcValue, options JSON_FOR_EACH(JSON_FIELD_VALUE, __VA_ARGS__)); \
}

namespace JSON
{
	enum class Type : uint8_t
	{
		Unknown = 0X00, // Unknown / undefined
		Null	= 0x01, // Null
		Boolean	= 0x02, // Bool
		NumberI	= 0x04, // int64_t
		NumberU	= 0x08, // uint64_t
		NumberF	= 0x10, // long double
		String	= 0x20, // std::string
		Array	= 0x40, // std::vector<Value>
		Object	= 0x80 // std::unordered_map<Field, Value>
	};

	Type operator|(Type lhs, Type rhs);

	Type operator&(Type lhs, Type rhs);

	struct Options
	{
		Encoding jsonEncoding = JSON_DEFAULT_ENCODING;

		Encoding memberEncoding = Encoding::Unknown;

		int32_t precision = -1;

		Options() = default;

		Options(Encoding jsonEncoding);

		Options(Encoding jsonEncoding, Encoding memberEncoding);

		Options(int32_t precision);

		Options(Encoding jsonEncoding, int32_t precision);

		Options(Encoding jsonEncoding, Encoding memberEncoding, int32_t precision);

		Options(int32_t precision, Encoding jsonEncoding);

		Options(int32_t precision, Encoding jsonEncoding, Encoding memberEncoding);

		Options(const Options& other);

		Options(Options&& other) noexcept;

		Options& operator=(const Options& other);

		Options& operator=(Options&& other) noexcept;
	};

	struct Field
	{
		std::string name;

		Encoding memberEncoding = Encoding::Unknown;

		int32_t precision = -1;

		Field() = default;

		Field(const char* fieldName, Encoding memberEncoding = Encoding::Unknown, int32_t precision = -1);

		Field(const std::string& fieldName, Encoding memberEncoding = Encoding::Unknown, int32_t precision = -1);

		Field(std::string&& fieldName, Encoding memberEncoding = Encoding::Unknown, int32_t precision = -1);

		template<class TString>
		Field(const TString& fieldName, Encoding memberEncoding = Encoding::Unknown, int32_t precision = -1);

		Field(const char* fieldName, int32_t precision, Encoding memberEncoding = Encoding::Unknown);

		Field(const std::string& fieldName, int32_t precision, Encoding memberEncoding = Encoding::Unknown);

		Field(std::string&& fieldName, int32_t precision, Encoding memberEncoding = Encoding::Unknown);

		Field(const Field& other);

		Field(Field&& other) noexcept;

		Field& operator=(const Field& other);

		Field& operator=(Field&& other) noexcept;

		bool operator==(const Field& other) const;

		bool operator!=(const Field& other) const;

		operator std::string() const;
	};
};

namespace std
{
	template<>
	struct hash<JSON::Field>
	{
		std::size_t operator()(const JSON::Field& field) const
		{
			return std::hash<std::string>()(field.name);
		}
	};
};

namespace JSON
{
	struct Value
	{
		Type type = Type::Null;

		union
		{
			void* ptr = nullptr;
			bool* ptrBoolean;
			int64_t* ptrNumberI;
			uint64_t* ptrNumberU;
			long double* ptrNumberF;
			std::string* ptrString;
			std::vector<Value>* ptrArray;
			std::unordered_map<Field, Value>* ptrObject;
		};

		static Value createNull();

		static Value createBoolean();

		static Value createNumberI();

		static Value createNumberU();

		static Value createNumberF();

		static Value createString();

		static Value createArray();

		static Value createObject();

		Value() = default;

		Value(Type srcType);

		Value(const bool srcBoolean);

		Value(const int8_t srcNumber);

		Value(const int16_t srcNumber);

		Value(const int32_t srcNumber);

		Value(const int64_t srcNumber);

		Value(const uint8_t srcNumber);

		Value(const uint16_t srcNumber);

		Value(const uint32_t srcNumber);

		Value(const uint64_t srcNumber);

		Value(const float srcNumber);

		Value(const double srcNumber);

		Value(const long double srcNumber);

		Value(const char* srcString);

		Value(const std::string& srcString);

		Value(const wchar_t* srcString, Encoding srcEncoding = JSON_DEFAULT_ENCODING_WSTRING);

		Value(const std::wstring& srcString, Encoding srcEncoding = JSON_DEFAULT_ENCODING_WSTRING);

		Value(const std::vector<Value>& srcContainer);

		Value(const std::unordered_map<std::string, Value>& srcContainer);

		Value(const std::unordered_map<Field, Value>& srcContainer);

		Value(const Value& other);

		Value(Value&& other) noexcept;

		~Value();

		Value& operator=(const Value& other);

		Value& operator=(Value&& other) noexcept;

		Value& operator=(const std::nullptr_t);

		Value& operator=(const Type srcType);

		Value& operator=(const bool srcBoolean);

		Value& operator=(const int8_t srcNumber);

		Value& operator=(const int16_t srcNumber);

		Value& operator=(const int32_t srcNumber);

		Value& operator=(const int64_t srcNumber);

		Value& operator=(const uint8_t srcNumber);

		Value& operator=(const uint16_t srcNumber);

		Value& operator=(const uint32_t srcNumber);

		Value& operator=(const uint64_t srcNumber);

		Value& operator=(const float srcNumber);

		Value& operator=(const double srcNumber);

		Value& operator=(const long double srcNumber);

		Value& operator=(const char* srcString);

		Value& operator=(const std::string& srcString);

		Value& operator=(const wchar_t* srcString);

		Value& operator=(const std::wstring& value);

		Value& operator=(const std::vector<Value>& srcContainer);

		Value& operator=(const std::unordered_map<std::string, Value>& srcContainer);

		Value& operator=(const std::unordered_map<Field, Value>& srcContainer);

		bool operator==(const Type otherType) const;

		bool operator==(const Value& other) const;

		bool operator!=(const Value& other) const;

		bool& getBoolean() const;

		int64_t& getNumberI() const;

		uint64_t& getNumberU() const;

		long double& getNumberF() const;

		std::string& getString() const;

		std::vector<Value>& getArray() const;

		std::unordered_map<Field, Value>& getObject() const;

		bool isNull() const;

		bool isBoolean() const;

		bool isNumberI() const;

		bool isNumberU() const;

		bool isNumberF() const;

		bool isNumber() const;

		bool isString() const;

		bool isArray() const;

		bool isObject() const;

	protected:

		void deleteData();
	};

	template<class TValue>
	struct FieldValue
	{
		const Field& field;

		TValue& value;

		FieldValue(const Field& field, TValue& value)
			: field(field)
			, value(value)
		{
		}

		FieldValue(const FieldValue& other)
			: field(other.field)
			, value(other.value)
		{
		}

		FieldValue(FieldValue&& other)
			: field(std::move(other.field))
			, value(std::move(other.value))
		{
		}

		FieldValue& operator=(const FieldValue& other)
		{
			field = other.field;
			value = other.value;
			return *this;
		}

		FieldValue& operator=(FieldValue&& other)
		{
			field = std::move(other.field);
			value = std::move(other.value);
			return *this;
		}
	};

#pragma region Serialization

	template<class TValue>
	std::string serialize(const TValue& srcValue, const Options& options = Options());

	template<class TValue>
	void serialize(std::ofstream&& dstStream, const TValue& srcValue, const Options& options);

	template<class TValue>
	void serialize(std::ofstream& dstStream, const TValue& srcValue, const Options& options);

	template<class TValue>
	void serialize(std::ostream&& dstStream, const TValue& srcValue, const Options& options = Options());

	template<class TValue>
	void serialize(std::ostream& dstStream, const TValue& srcValue, const Options& options = Options());

	template<class TValue, class... TValues>
	void serializeFieldValue(std::ostream& dstStream, const Options& options, const FieldValue<TValue>& srcFieldValue, const FieldValue<TValues>&... srcFieldValues);

	template<class TValue, class... TValues>
	void serializeFieldValue(std::ostream& dstStream, const FieldValue<TValue>& srcFieldValue, const FieldValue<TValues>&... srcFieldValues);

	template<class TValue>
	void serializeFieldValue(std::ostream& dstStream, const Options& options, const FieldValue<TValue>& srcFieldValue);

	template<class TValue>
	void serializeFieldValue(std::ostream& dstStream, const FieldValue<TValue>& srcFieldValue);

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const TValue* srcValue, const Options& options = Options());

	void serializeValue(std::ostream& dstStream, const Value* srcValue, const Options& options = Options());

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::optional<TValue>* srcOptional, const Options& options = Options());

	void serializeValue(std::ostream& dstStream, const bool* srcBoolean, const Options& options = Options());

	template<class TInteger>
	void serializeValueInteger(std::ostream& dstStream, const TInteger* srcNumber, const Options& options = Options());

	void serializeValue(std::ostream& dstStream, const int8_t* srcNumber, const Options& options = Options());

	void serializeValue(std::ostream& dstStream, const uint8_t* srcNumber, const Options& options = Options());

	void serializeValue(std::ostream& dstStream, const int16_t* srcNumber, const Options& options = Options());

	void serializeValue(std::ostream& dstStream, const uint16_t* srcNumber, const Options& options = Options());

	void serializeValue(std::ostream& dstStream, const int32_t* srcNumber, const Options& options = Options());

	void serializeValue(std::ostream& dstStream, const uint32_t* srcNumber, const Options& options = Options());

	void serializeValue(std::ostream& dstStream, const int64_t* srcNumber, const Options& options = Options());

	void serializeValue(std::ostream& dstStream, const uint64_t* srcNumber, const Options& options = Options());

	template<class TFloat>
	void serializeValueFloat(std::ostream& dstStream, const TFloat* srcNumber, const Options& options = Options());

	void serializeValue(std::ostream& dstStream, const float* srcNumber, const Options& options = Options());

	void serializeValue(std::ostream& dstStream, const double* srcNumber, const Options& options = Options());

	void serializeValue(std::ostream& dstStream, const long double* srcNumber, const Options& options = Options());

	void serializeValue(std::ostream& dstStream, const std::string* srcString, const Options& options = Options());

	void serializeValue(std::ostream& dstStream, const std::wstring* srcString, const Options& options = Options(JSON_DEFAULT_ENCODING, Encoding::UTF16BE));

	template<class TArray>
	void serializeValueArray(std::ostream& dstStream, const TArray* srcContainer, const Options& options = Options());

	template<class TValue, std::size_t TArraySize>
	void serializeValue(std::ostream& dstStream, const class std::array<TValue, TArraySize>* srcContainer, const Options& options = Options());

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::vector<TValue>* srcContainer, const Options& options = Options());

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::deque<TValue>* srcContainer, const Options& options = Options());

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::forward_list<TValue>* srcContainer, const Options& options = Options());

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::list<TValue>* srcContainer, const Options& options = Options());

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::set<TValue>* srcContainer, const Options& options = Options());

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::multiset<TValue>* srcContainer, const Options& options = Options());

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::unordered_set<TValue>* srcContainer, const Options& options = Options());

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::unordered_multiset<TValue>* srcContainer, const Options& options = Options());

	template<class TObject>
	void serializeValueObject(std::ostream& dstStream, const TObject* srcContainer, const Options& options = Options());

	template<class TField, class TValue>
	void serializeValue(std::ostream& dstStream, const std::map<TField, TValue>* srcContainer, const Options& options = Options());

	template<class TField, class TValue>
	void serializeValue(std::ostream& dstStream, const std::multimap<TField, TValue>* srcContainer, const Options& options = Options());

	template<class TField, class TValue>
	void serializeValue(std::ostream& dstStream, const std::unordered_map<TField, TValue>* srcContainer, const Options& options = Options());

	template<class TField, class TValue>
	void serializeValue(std::ostream& dstStream, const std::unordered_multimap<TField, TValue>* srcContainer, const Options& options = Options());

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::stack<TValue>* srcContainer, const Options& options = Options());

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::queue<TValue>* srcContainer, const Options& options = Options());

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::priority_queue<TValue>* srcContainer, const Options& options = Options());

#pragma endregion

#pragma region Deserialization

	bool isValid(const std::string& srcJson, const Options& options = Options()) noexcept;

	bool isValid(std::ifstream&& srcStream, const Options& options = Options()) noexcept;

	bool isValid(std::ifstream& srcStream, const Options& options = Options()) noexcept;

	bool isValid(std::istream&& srcStream, const Options& options = Options()) noexcept;

	bool isValid(std::istream& srcStream, const Options& options = Options()) noexcept;

	template<class TValue>
	void deserialize(const std::string& srcJson, TValue& dstValue, const Options& options = Options());

	template<class TValue>
	void deserialize(std::ifstream&& srcStream, TValue& dstValue, const Options& options = Options());

	template<class TValue>
	void deserialize(std::ifstream& srcStream, TValue& dstValue, const Options& options = Options());

	template<class TValue>
	void deserialize(std::istream&& srcStream, TValue& dstValue, const Options& options = Options());

	template<class TValue>
	void deserialize(std::istream& srcStream, TValue& dstValue, const Options& options = Options());

	template<class TValue, class... TValues>
	void deserializeFieldValue(const Value& srcValue, const Options& options, const FieldValue<TValue>& dstFieldValue, const FieldValue<TValues>&... dstFieldValues);

	template<class TValue, class... TValues>
	void deserializeFieldValue(const Value& srcValue, const FieldValue<TValue>& dstFieldValue, const FieldValue<TValues>&... dstFieldValues);

	void deserializeFieldValue(const Value&, const Options& = Options());

	template<class TValue>
	void deserializeValue(std::istream& srcStream, TValue* dstValue, const Options& options = Options());

	void deserializeValue(std::istream& srcStream, Value* dstValue, const Options& options = Options());

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::optional<TValue>* dstOptional, const Options& options = Options());

	void deserializeValue(std::istream& srcStream, bool* dstBoolean, const Options& options = Options());

	void deserializeValue(std::istream& srcStream, int8_t* dstNumber, const Options& options = Options());

	void deserializeValue(std::istream& srcStream, uint8_t* dstNumber, const Options& options = Options());

	void deserializeValue(std::istream& srcStream, int16_t* dstNumber, const Options& options = Options());

	void deserializeValue(std::istream& srcStream, uint16_t* dstNumber, const Options& options = Options());

	void deserializeValue(std::istream& srcStream, int32_t* dstNumber, const Options& options = Options());

	void deserializeValue(std::istream& srcStream, uint32_t* dstNumber, const Options& options = Options());

	void deserializeValue(std::istream& srcStream, int64_t* dstNumber, const Options& options = Options());

	void deserializeValue(std::istream& srcStream, uint64_t* dstNumber, const Options& options = Options());

	void deserializeValue(std::istream& srcStream, float* dstNumber, const Options& options = Options());

	void deserializeValue(std::istream& srcStream, double* dstNumber, const Options& options = Options());

	void deserializeValue(std::istream& srcStream, long double* dstNumber, const Options& options = Options());

	void deserializeValue(std::istream& srcStream, std::string* dstString, const Options& options = Options());

	void deserializeValue(std::istream& srcStream, std::wstring* dstString, const Options& options = Options());

	template<class TValue, std::size_t TSize>
	void deserializeValue(std::istream& srcStream, std::array<TValue, TSize>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::vector<TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::deque<TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::forward_list<TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::list<TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::set<TValue>* dstContainer, const Options& options = Options());

	template<class TKey, class TValue>
	void deserializeValue(std::istream& srcStream, std::map<TKey, TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::multiset<TValue>* dstContainer, const Options& options = Options());

	template<class TKey, class TValue>
	void deserializeValue(std::istream& srcStream, std::multimap<TKey, TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::unordered_set<TValue>* dstContainer, const Options& options = Options());

	template<class TKey, class TValue>
	void deserializeValue(std::istream& srcStream, std::unordered_map<TKey, TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::unordered_multiset<TValue>* dstContainer, const Options& options = Options());

	template<class TKey, class TValue>
	void deserializeValue(std::istream& srcStream, std::unordered_multimap<TKey, TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::stack<TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::queue<TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::priority_queue<TValue>* dstContainer, const Options& options = Options());

	void deserializeValueNumberAsString(std::istream& srcStream, std::string* dstValue, const Options& options = Options());

	template<class TContainer, class TValue>
	void deserializeValueContainer(std::istream& srcStream, TContainer* dstContainer, std::function<void(TContainer*, TValue*, std::size_t index)> append, const Options& options = Options());

	template<class TContainer, class TValue>
	void deserializeValueContainerIt(std::istream& srcStream, TContainer* dstContainer, std::function<typename TContainer::iterator(TContainer*)> initIt, std::function<void(TContainer*, TValue*, typename TContainer::iterator&)> append, const Options& options = Options());

	template<class TContainer, class TKey, class TValue>
	void deserializeValueMap(std::istream& srcStream, TContainer* dstContainer, const Options& options = Options());

	void deserializeValue(const Value& srcValue, Value* dstValue, const Options& options = Options());

	template<class TValue>
	void deserializeValue(const Value& srcValue, TValue* dstValue, const Options& options = Options());

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::optional<TValue>* dstOptional, const Options& options = Options());

	void deserializeValue(const Value& srcValue, bool* dstBoolean, const Options& options = Options());

	void deserializeValue(const Value& srcValue, int8_t* dstNumber, const Options& options = Options());

	void deserializeValue(const Value& srcValue, int16_t* dstNumber, const Options& options = Options());

	void deserializeValue(const Value& srcValue, int32_t* dstNumber, const Options& options = Options());

	void deserializeValue(const Value& srcValue, int64_t* dstNumber, const Options& options = Options());

	void deserializeValue(const Value& srcValue, uint8_t* dstNumber, const Options& options = Options());

	void deserializeValue(const Value& srcValue, uint16_t* dstNumber, const Options& options = Options());

	void deserializeValue(const Value& srcValue, uint32_t* dstNumber, const Options& options = Options());

	void deserializeValue(const Value& srcValue, uint64_t* dstNumber, const Options& options = Options());

	void deserializeValue(const Value& srcValue, float* dstNumber, const Options& options = Options());

	void deserializeValue(const Value& srcValue, double* dstNumber, const Options& options = Options());

	void deserializeValue(const Value& srcValue, long double* dstNumber, const Options& options = Options());

	void deserializeValue(const Value& srcValue, std::string* dstString, const Options& options = Options());

	void deserializeValue(const Value& srcValue, std::wstring* dstString, const Options& options = Options());

	template<class TValue, std::size_t TSize>
	void deserializeValue(const Value& srcValue, std::array<TValue, TSize>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::vector<TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::deque<TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::forward_list<TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::list<TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::set<TValue>* dstContainer, const Options& options = Options());

	template<class TKey, class TValue>
	void deserializeValue(const Value& srcValue, std::map<TKey, TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::multiset<TValue>* dstContainer, const Options& options = Options());

	template<class TKey, class TValue>
	void deserializeValue(const Value& srcValue, std::multimap<TKey, TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::unordered_set<TValue>* dstContainer, const Options& options = Options());

	template<class TKey, class TValue>
	void deserializeValue(const Value& srcValue, std::unordered_map<TKey, TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::unordered_multiset<TValue>* dstContainer, const Options& options = Options());

	template<class TKey, class TValue>
	void deserializeValue(const Value& srcValue, std::unordered_multimap<TKey, TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::stack<TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::queue<TValue>* dstContainer, const Options& options = Options());

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::priority_queue<TValue>* dstContainer, const Options& options = Options());

#pragma endregion
}

std::istream& operator>>(std::istream& srcStream, JSON::Field& dstField);

std::ostream& operator<<(std::ostream& dstStream, const JSON::Field& dstField);

#pragma GCC diagnostic pop
