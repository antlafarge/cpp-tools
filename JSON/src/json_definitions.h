#pragma once

#include "optional.polyfill.h"
#include "json_unicode.h"

#include <array>
#include <deque>
#include <forward_list>
#include <functional>
#include <list>
#include <map>
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
void toJSON(std::ostream& stream, JSON::Options& options) const \
{ \
	JSON::serializeUnicodeChar(stream, codePointToUnicode('{', options), options); \
	JSON::serializeFieldValue(stream, options JSON_FOR_EACH(JSON_FIELD_VALUE_CONST, __VA_ARGS__)); \
	JSON::serializeUnicodeChar(stream, codePointToUnicode('}', options), options); \
} \
\
void fromJSON(std::istream& stream, const JSON::Options& options) \
{ \
	JSON::Value value; \
	JSON::deserialize(stream, value, options); \
	fromJSON(value); \
} \
\
void fromJSON(const JSON::Value& value) \
{ \
	JSON::Options options; \
	JSON::deserializeFieldValue(value JSON_FOR_EACH(JSON_FIELD_VALUE, __VA_ARGS__)); \
}

namespace JSON
{
	enum class Type
	{
		Null, // Null
		Boolean, // Bool
		NumberI, // int64_t
		NumberU, // uint64_t
		NumberF, // long double
		String, // std::string
		Array, // std::vector<Value>
		Object // std::unordered_map<Field, Value>
	};

	struct Options
	{
		Encoding encoding = Encoding::None;

		std::streamsize precision = -1;

		Options() = default;

		Options(Encoding encoding);

		Options(std::streamsize precision);

		Options(Encoding encoding, std::streamsize precision);

		Options(std::streamsize precision, Encoding encoding);

		Options(const Options& other);

		Options(Options&& other) noexcept;

		Options& operator=(const Options& other);

		Options& operator=(Options&& other) noexcept;

		operator Encoding() const;

		void merge(const Options& options);
	};

	struct Field
	{
		std::string name;

		Options options;

		Field() = default;

		Field(const char* fieldName, const Options& options = Options());

		Field(const std::string& fieldName, const Options& options = Options());

		Field(std::string&& fieldName, const Options& options = Options());

		template<class TString>
		Field(const TString& fieldName, const Options& options = Options());

		Field(const Options& options);

		Field(const Field& other);

		Field(Field&& other) noexcept;

		Field& operator=(const Field& right);

		Field& operator=(Field&& right) noexcept;

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

		inline bool& getBoolean()
		{
			return *ptrBoolean;
		}

		inline int64_t& getNumberI()
		{
			return *ptrNumberI;
		}

		inline uint64_t& getNumberU()
		{
			return *ptrNumberU;
		}

		inline long double& getNumberF()
		{
			return *ptrNumberF;
		}

		inline std::string& getString()
		{
			return *ptrString;
		}

		inline std::vector<Value>& getArray()
		{
			return *ptrArray;
		}

		inline std::unordered_map<Field, Value>& getObject()
		{
			return *ptrObject;
		}

		Value() = default;

		Value(Type type);

		Value(const bool value);

		Value(const int8_t value);

		Value(const int16_t value);

		Value(const int32_t value);

		Value(const int64_t value);

		Value(const uint8_t value);

		Value(const uint16_t value);

		Value(const uint32_t value);

		Value(const uint64_t value);

		Value(const float value);

		Value(const double value);

		Value(const long double value);

		Value(const char* value);

		Value(const std::string& value);

		Value(const wchar_t* value, const Options& options = Options());

		Value(const std::wstring& value, const Options& options = Options());

		Value(const std::vector<Value>& value);

		Value(const std::unordered_map<std::string, Value>& value);

		Value(const std::unordered_map<Field, Value>& value);

		Value(const Value& other);

		Value(Value&& other) noexcept;

		~Value();

		Value& operator=(const Value& other);

		Value& operator=(Value&& other) noexcept;

		Value& operator=(const Type type2);

		Value& operator=(const bool value);

		Value& operator=(const int8_t value);

		Value& operator=(const int16_t value);

		Value& operator=(const int32_t value);

		Value& operator=(const int64_t value);

		Value& operator=(const uint8_t value);

		Value& operator=(const uint16_t value);

		Value& operator=(const uint32_t value);

		Value& operator=(const uint64_t value);

		Value& operator=(const float value);

		Value& operator=(const double value);

		Value& operator=(const long double value);

		Value& operator=(const char* value);

		Value& operator=(const std::string& value);

		Value& operator=(const wchar_t* value);

		Value& operator=(const std::wstring& value);

		Value& operator=(const std::vector<Value>& value);

		Value& operator=(const std::unordered_map<std::string, Value>& value);

		Value& operator=(const std::unordered_map<Field, Value>& value);

		bool operator==(const Type type2) const;

		bool operator==(const Value& other) const;

		bool operator!=(const Value& right) const;

	protected:

		void copyData(const Value& other);

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

		FieldValue& operator=(const FieldValue& right)
		{
			field = right.field;
			value = right.value;
			return *this;
		}

		FieldValue& operator=(FieldValue&& right)
		{
			field = std::move(right.field);
			value = std::move(right.value);
			return *this;
		}
	};

#pragma region Serialization

	template<class TValue>
	std::string serialize(const TValue& value, const Options& options = Options());

	template<class TValue>
	void serialize(std::ostream&& stream, const TValue& value, const Options& options = Options());

	template<class TValue>
	void serialize(std::ostream& stream, const TValue& value, const Options& options = Options());

	template<class TValue, class... TValues>
	void serializeFieldValue(std::ostream& stream, Options& options, const FieldValue<TValue>& fieldValue, const FieldValue<TValues>&... fieldValues);

	template<class TValue>
	void serializeFieldValue(std::ostream& stream, Options& options, const FieldValue<TValue>& fieldValue);

	template<class TValue>
	void serializeValue(std::ostream& stream, const TValue* value, Options& options);

	void serializeValue(std::ostream& stream, const Value* value, Options& options);

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::optional<TValue>* value, Options& options);

	void serializeValue(std::ostream& stream, const bool* value, Options& options);

	template<class TInteger>
	void serializeValueInteger(std::ostream& stream, const TInteger* value, Options& options);

	void serializeValue(std::ostream& stream, const int8_t* value, Options& options);

	void serializeValue(std::ostream& stream, const uint8_t* value, Options& options);

	void serializeValue(std::ostream& stream, const int16_t* value, Options& options);

	void serializeValue(std::ostream& stream, const uint16_t* value, Options& options);

	void serializeValue(std::ostream& stream, const int32_t* value, Options& options);

	void serializeValue(std::ostream& stream, const uint32_t* value, Options& options);

	void serializeValue(std::ostream& stream, const int64_t* value, Options& options);

	void serializeValue(std::ostream& stream, const uint64_t* value, Options& options);

	template<class TFloat>
	void serializeValueFloat(std::ostream& stream, const TFloat* value, Options& options);

	void serializeValue(std::ostream& stream, const float* value, Options& options);

	void serializeValue(std::ostream& stream, const double* value, Options& options);

	void serializeValue(std::ostream& stream, const long double* value, Options& options);

	void serializeValue(std::ostream& stream, const std::string* value, Options& options);

	void serializeValue(std::ostream& stream, const std::wstring* value, Options& options);

	template<class TArray>
	void serializeValueArray(std::ostream& stream, const TArray* value, Options& options);

	template<class TValue, std::size_t TArraySize>
	void serializeValue(std::ostream& stream, const class std::array<TValue, TArraySize>* value, Options& options);

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::vector<TValue>* value, Options& options);

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::deque<TValue>* value, Options& options);

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::forward_list<TValue>* value, Options& options);

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::list<TValue>* value, Options& options);

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::set<TValue>* value, Options& options);

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::multiset<TValue>* value, Options& options);

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::unordered_set<TValue>* value, Options& options);

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::unordered_multiset<TValue>* value, Options& options);

	template<class TObject>
	void serializeValueObject(std::ostream& stream, const TObject* value, Options& options);

	template<class TField, class TValue>
	void serializeValue(std::ostream& stream, const std::map<TField, TValue>* value, Options& options);

	template<class TField, class TValue>
	void serializeValue(std::ostream& stream, const std::multimap<TField, TValue>* value, Options& options);

	template<class TField, class TValue>
	void serializeValue(std::ostream& stream, const std::unordered_map<TField, TValue>* value, Options& options);

	template<class TField, class TValue>
	void serializeValue(std::ostream& stream, const std::unordered_multimap<TField, TValue>* value, Options& options);

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::stack<TValue>* value, Options& options);

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::queue<TValue>* value, Options& options);

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::priority_queue<TValue>* value, Options& options);

#pragma endregion

#pragma region Deserialization

	bool isValid(const std::string& json, const Options& options = Options()) noexcept;

	bool isValid(std::istream&& stream, const Options& options = Options()) noexcept;

	bool isValid(std::istream& stream, const Options& options = Options()) noexcept;

	template<class TValue>
	void deserialize(const std::string& json, TValue& value, const Options& options = Options());

	template<class TValue>
	void deserialize(std::istream&& stream, TValue& value, const Options& options = Options());

	template<class TValue>
	void deserialize(std::istream& stream, TValue& value, const Options& options = Options());

	template<class TValue, class... TValues>
	void deserializeFieldValue(const Value& value, const FieldValue<TValue>& fieldValue, const FieldValue<TValues>&... fieldValues);

	void deserializeFieldValue(const Value&);

	template<class TValue>
	void deserializeValue(std::istream& stream, TValue* value, Options& options);

	void deserializeValue(std::istream& stream, Value* value, Options& options);

	template<class TValue>
	void deserializeValue(std::istream& stream, std::optional<TValue>* value, Options& options);

	void deserializeValue(std::istream& stream, bool* value, Options& options);

	void deserializeValue(std::istream& stream, int8_t* value, Options& options);

	void deserializeValue(std::istream& stream, uint8_t* value, Options& options);

	void deserializeValue(std::istream& stream, int16_t* value, Options& options);

	void deserializeValue(std::istream& stream, uint16_t* value, Options& options);

	void deserializeValue(std::istream& stream, int32_t* value, Options& options);

	void deserializeValue(std::istream& stream, uint32_t* value, Options& options);

	void deserializeValue(std::istream& stream, int64_t* value, Options& options);

	void deserializeValue(std::istream& stream, uint64_t* value, Options& options);

	void deserializeValue(std::istream& stream, float* value, Options& options);

	void deserializeValue(std::istream& stream, double* value, Options& options);

	void deserializeValue(std::istream& stream, long double* value, Options& options);

	void deserializeValue(std::istream& stream, std::string* value, Options& options);

	void deserializeValue(std::istream& stream, std::wstring* value, Options& options);

	template<class TValue, std::size_t TSize>
	void deserializeValue(std::istream& stream, std::array<TValue, TSize>* container, Options& options);

	template<class TValue>
	void deserializeValue(std::istream& stream, std::vector<TValue>* container, Options& options);

	template<class TValue>
	void deserializeValue(std::istream& stream, std::deque<TValue>* container, Options& options);

	template<class TValue>
	void deserializeValue(std::istream& stream, std::forward_list<TValue>* container, Options& options);

	template<class TValue>
	void deserializeValue(std::istream& stream, std::list<TValue>* container, Options& options);

	template<class TValue>
	void deserializeValue(std::istream& stream, std::set<TValue>* container, Options& options);

	template<class TKey, class TValue>
	void deserializeValue(std::istream& stream, std::map<TKey, TValue>* container, Options& options);

	template<class TValue>
	void deserializeValue(std::istream& stream, std::multiset<TValue>* container, Options& options);

	template<class TKey, class TValue>
	void deserializeValue(std::istream& stream, std::multimap<TKey, TValue>* container, Options& options);

	template<class TValue>
	void deserializeValue(std::istream& stream, std::unordered_set<TValue>* container, Options& options);

	template<class TKey, class TValue>
	void deserializeValue(std::istream& stream, std::unordered_map<TKey, TValue>* container, Options& options);

	template<class TValue>
	void deserializeValue(std::istream& stream, std::unordered_multiset<TValue>* container, Options& options);

	template<class TKey, class TValue>
	void deserializeValue(std::istream& stream, std::unordered_multimap<TKey, TValue>* container, Options& options);

	template<class TValue>
	void deserializeValue(std::istream& stream, std::stack<TValue>* container, Options& options);

	template<class TValue>
	void deserializeValue(std::istream& stream, std::queue<TValue>* container, Options& options);

	template<class TValue>
	void deserializeValue(std::istream& stream, std::priority_queue<TValue>* container, Options& options);

	void deserializeValueNumberAsString(std::istream& stream, std::string* value, Options& options);

	template<class TContainer, class TValue>
	void deserializeValueContainer(std::istream& stream, TContainer* container, std::function<void(TContainer*, TValue*, std::size_t index)> append, Options& options);

	template<class TContainer, class TValue>
	void deserializeValueContainerIt(std::istream& stream, TContainer* container, std::function<typename TContainer::iterator(TContainer*)> initIt, std::function<void(TContainer*, TValue*, typename TContainer::iterator&)> append, Options& options);

	template<class TContainer, class TKey, class TValue>
	void deserializeValueMap(std::istream& stream, TContainer* map, Options& options);

	void deserializeValue(const Value& value, Value* value2);

	template<class TValue>
	void deserializeValue(const Value& value, TValue* value2);

	template<class TValue>
	void deserializeValue(const Value& value, std::optional<TValue>* optional);

	void deserializeValue(const Value& value, bool* boolean);

	void deserializeValue(const Value& value, int8_t* number);

	void deserializeValue(const Value& value, int16_t* number);

	void deserializeValue(const Value& value, int32_t* number);

	void deserializeValue(const Value& value, int64_t* number);

	void deserializeValue(const Value& value, uint8_t* number);

	void deserializeValue(const Value& value, uint16_t* number);

	void deserializeValue(const Value& value, uint32_t* number);

	void deserializeValue(const Value& value, uint64_t* number);

	void deserializeValue(const Value& value, float* number);

	void deserializeValue(const Value& value, double* number);

	void deserializeValue(const Value& value, long double* number);

	void deserializeValue(const Value& value, std::string* string);

	void deserializeValue(const Value& value, std::wstring* string);

	template<class TValue, std::size_t TSize>
	void deserializeValue(const Value& value, std::array<TValue, TSize>* container);

	template<class TValue>
	void deserializeValue(const Value& value, std::vector<TValue>* container);

	template<class TValue>
	void deserializeValue(const Value& value, std::deque<TValue>* container);

	template<class TValue>
	void deserializeValue(const Value& value, std::forward_list<TValue>* container);

	template<class TValue>
	void deserializeValue(const Value& value, std::list<TValue>* container);

	template<class TValue>
	void deserializeValue(const Value& value, std::set<TValue>* container);

	template<class TKey, class TValue>
	void deserializeValue(const Value& value, std::map<TKey, TValue>* container);

	template<class TValue>
	void deserializeValue(const Value& value, std::multiset<TValue>* container);

	template<class TKey, class TValue>
	void deserializeValue(const Value& value, std::multimap<TKey, TValue>* container);

	template<class TValue>
	void deserializeValue(const Value& value, std::unordered_set<TValue>* container);

	template<class TKey, class TValue>
	void deserializeValue(const Value& value, std::unordered_map<TKey, TValue>* container);

	template<class TValue>
	void deserializeValue(const Value& value, std::unordered_multiset<TValue>* container);

	template<class TKey, class TValue>
	void deserializeValue(const Value& value, std::unordered_multimap<TKey, TValue>* container);

	template<class TValue>
	void deserializeValue(const Value& value, std::stack<TValue>* container);

	template<class TValue>
	void deserializeValue(const Value& value, std::queue<TValue>* container);

	template<class TValue>
	void deserializeValue(const Value& value, std::priority_queue<TValue>* container);

#pragma endregion
}

std::istream& operator>>(std::istream& stream, JSON::Field& field);

std::ostream& operator<<(std::ostream& stream, const JSON::Field& key);

#pragma GCC diagnostic pop
