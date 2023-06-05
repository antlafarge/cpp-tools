#pragma once

#include "json_definitions.h"

#include <iomanip>
#include <limits>
#include <sstream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

namespace JSON
{
	template<class TString>
	Field::Field(const TString& fieldName, Encoding memberEncoding, int32_t precision)
		: memberEncoding(memberEncoding)
		, precision(precision)
	{
		std::ostringstream stream;
		stream << fieldName;
		name = stream.str();
	}

#pragma region Serialization

	template<class TValue>
	std::string serialize(const TValue& srcValue, const Options& options)
	{
		std::ostringstream dstStream;
		serialize(dstStream, srcValue, options);
		return dstStream.str();
	}

	template<class TValue>
	void serialize(std::ofstream&& dstStream, const TValue& srcValue, const Options& options)
	{
		std::ostream dstStream2(dstStream.rdbuf());
		if (dstStream.tellp() == 0) [[likely]]
		{
			writeBOM(dstStream2, options.jsonEncoding);
		}
		serialize(dstStream2, srcValue, options);
	}

	template<class TValue>
	void serialize(std::ofstream& dstStream, const TValue& srcValue, const Options& options)
	{
		if (dstStream.tellp() == 0) [[likely]]
		{
			writeBOM(dstStream, options.jsonEncoding);
		}
		serialize(dstStream, srcValue, options);
	}

	template<class TValue>
	void serialize(std::ostream&& dstStream, const TValue& srcValue, const Options& options)
	{
		std::ostream dstStream2(dstStream.rdbuf());
		serialize(dstStream2, srcValue, options);
	}

	template<class TValue>
	void serialize(std::ostream& dstStream, const TValue& srcValue, const Options& options)
	{
		serializeValue(dstStream, &srcValue, options);
	}

	template<class TValue, class... TValues>
	void serializeFieldValue(std::ostream& dstStream, const Options& options, const FieldValue<TValue>& srcFieldValue, const FieldValue<TValues>&... srcFieldValues)
	{
		serializeFieldValue(dstStream, options, srcFieldValue);
		serializeCodePointChar(dstStream, ',', options.jsonEncoding);
		serializeFieldValue(dstStream, options, srcFieldValues...);
	}

	template<class TValue, class... TValues>
	void serializeFieldValue(std::ostream& dstStream, const FieldValue<TValue>& srcFieldValue, const FieldValue<TValues>&... srcFieldValues)
	{
		serializeFieldValue(dstStream, Options(), srcFieldValue, srcFieldValues...);
	}

	template<class TValue>
	void serializeFieldValue(std::ostream& dstStream, const Options& options, const FieldValue<TValue>& srcFieldValue)
	{
		serializeValue(dstStream, &srcFieldValue.field.name, options);

		serializeCodePointChar(dstStream, ':', options.jsonEncoding);

		serializeValue(dstStream, &srcFieldValue.value, Options(options.jsonEncoding, (srcFieldValue.field.memberEncoding != Encoding::Unknown ? srcFieldValue.field.memberEncoding : options.memberEncoding), (srcFieldValue.field.precision != -1 ? srcFieldValue.field.precision : options.precision)));
	}

	template<class TValue>
	void serializeFieldValue(std::ostream& dstStream, const FieldValue<TValue>& srcFieldValue)
	{
		serializeFieldValue(dstStream, Options(), srcFieldValue);
	}

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const TValue* srcValue, const Options& options)
	{
		srcValue->toJSON(dstStream, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::optional<TValue>* srcOptional, const Options& options)
	{
		if (*srcOptional)
		{
			serializeValue(dstStream, &**srcOptional, options);
		}
		else
		{
			serializeCodePointChar(dstStream, 'n', options.jsonEncoding);
			serializeCodePointChar(dstStream, 'u', options.jsonEncoding);
			serializeCodePointChar(dstStream, 'l', options.jsonEncoding);
			serializeCodePointChar(dstStream, 'l', options.jsonEncoding);
		}
	}

	template<class TInteger>
	void serializeValueInteger(std::ostream& dstStream, const TInteger* srcNumber, const Options& options)
	{
		if (options.jsonEncoding <= Encoding::UTF8) [[likely]]
		{
			dstStream << *srcNumber;
		}
		else [[unlikely]]
		{
			std::stringstream stream;
			stream << *srcNumber;
			stream.seekg(0, stream.beg);
			uint32_t codePoint;
			while (true)
			{
				codePoint = readChar(stream, codePoint, Encoding::UTF8);
				if (codePoint == JSON_EOF)
				{
					break;
				}
				serializeCodePointChar(dstStream, codePoint, options.jsonEncoding);
			}
		}
	}

	template<class TFloat>
	void serializeValueFloat(std::ostream& dstStream, const TFloat* srcNumber, const Options& options)
	{
		int32_t precision = (options.precision != -1 ? options.precision : std::numeric_limits<TFloat>::max_digits10);
		if (options.jsonEncoding <= Encoding::UTF8) [[likely]]
		{
			dstStream << std::setprecision(precision) << *srcNumber;
		}
		else [[unlikely]]
		{
			std::stringstream stream;
			stream << std::setprecision(precision) << *srcNumber;
			stream.seekg(0, stream.beg);
			uint32_t codePoint;
			while (true)
			{
				codePoint = readChar(stream, codePoint, Encoding::UTF8);
				if (codePoint == JSON_EOF)
				{
					break;
				}
				serializeCodePointChar(dstStream, codePoint, options.jsonEncoding);
			}
		}
	}

	template<class TArray>
	void serializeValueArray(std::ostream& dstStream, const TArray* srcContainer, const Options& options)
	{
		serializeCodePointChar(dstStream, '[', options.jsonEncoding);
		bool appendcomma = false;
		for (const auto& item : *srcContainer)
		{
			if (appendcomma) [[likely]]
			{
				serializeCodePointChar(dstStream, ',', options.jsonEncoding);
			}
			else [[unlikely]]
			{
				appendcomma = true;
			}
			serializeValue(dstStream, &item, options);
		}
		serializeCodePointChar(dstStream, ']', options.jsonEncoding);
	}

	template<class TValue, std::size_t TArraySize>
	void serializeValue(std::ostream& dstStream, const class std::array<TValue, TArraySize>* srcContainer, const Options& options)
	{
		serializeValueArray(dstStream, srcContainer, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::vector<TValue>* srcContainer, const Options& options)
	{
		serializeValueArray(dstStream, srcContainer, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::deque<TValue>* srcContainer, const Options& options)
	{
		serializeValueArray(dstStream, srcContainer, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::forward_list<TValue>* srcContainer, const Options& options)
	{
		serializeValueArray(dstStream, srcContainer, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::list<TValue>* srcContainer, const Options& options)
	{
		serializeValueArray(dstStream, srcContainer, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::set<TValue>* srcContainer, const Options& options)
	{
		serializeValueArray(dstStream, srcContainer, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::multiset<TValue>* srcContainer, const Options& options)
	{
		serializeValueArray(dstStream, srcContainer, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::unordered_set<TValue>* srcContainer, const Options& options)
	{
		serializeValueArray(dstStream, srcContainer, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::unordered_multiset<TValue>* srcContainer, const Options& options)
	{
		serializeValueArray(dstStream, srcContainer, options);
	}

	template<class TObject>
	void serializeValueObject(std::ostream& dstStream, const TObject* srcContainer, const Options& options)
	{
		serializeCodePointChar(dstStream, '{', options.jsonEncoding);
		bool appendcomma = false;
		for (const auto& pair : *srcContainer)
		{
			if (appendcomma) [[likely]]
			{
				serializeCodePointChar(dstStream, ',', options.jsonEncoding);
			}
			else [[unlikely]]
			{
				appendcomma = true;
			}
			serializeFieldValue(dstStream, options, FieldValue<const decltype(pair.second)>(Field(pair.first, options.memberEncoding, options.precision), pair.second));
		}
		serializeCodePointChar(dstStream, '}', options.jsonEncoding);
	}

	template<class TField, class TValue>
	void serializeValue(std::ostream& dstStream, const std::map<TField, TValue>* srcContainer, const Options& options)
	{
		serializeValueObject(dstStream, srcContainer, options);
	}

	template<class TField, class TValue>
	void serializeValue(std::ostream& dstStream, const std::multimap<TField, TValue>* srcContainer, const Options& options)
	{
		serializeValueObject(dstStream, srcContainer, options);
	}

	template<class TField, class TValue>
	void serializeValue(std::ostream& dstStream, const std::unordered_map<TField, TValue>* srcContainer, const Options& options)
	{
		serializeValueObject(dstStream, srcContainer, options);
	}

	template<class TField, class TValue>
	void serializeValue(std::ostream& dstStream, const std::unordered_multimap<TField, TValue>* srcContainer, const Options& options)
	{
		serializeValueObject(dstStream, srcContainer, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::stack<TValue>* srcContainer, const Options& options)
	{
		std::stack<TValue> copy(*srcContainer);
		std::deque<TValue> copy2;
		while (!copy.empty())
		{
			copy2.emplace_front(std::move(copy.top()));
			copy.pop();
		}
		serializeValue(dstStream, &copy2, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::queue<TValue>* srcContainer, const Options& options)
	{
		std::queue<TValue> copy(*srcContainer);
		serializeCodePointChar(dstStream, '[', options.jsonEncoding);
		bool appendcomma = false;
		while (!copy.empty())
		{
			const TValue& item = copy.front();
			if (appendcomma) [[likely]]
			{
				serializeCodePointChar(dstStream, ',', options.jsonEncoding);
			}
			else [[unlikely]]
			{
				appendcomma = true;
			}
			serializeValue(dstStream, &item, options);
			copy.pop();
		}
		serializeCodePointChar(dstStream, ']', options.jsonEncoding);
	}

	template<class TValue>
	void serializeValue(std::ostream& dstStream, const std::priority_queue<TValue>* srcContainer, const Options& options)
	{
		std::priority_queue<TValue> copy(*srcContainer);
		serializeCodePointChar(dstStream, '[', options.jsonEncoding);
		bool appendcomma = false;
		while (!copy.empty())
		{
			const TValue& item = copy.top();
			if (appendcomma) [[likely]]
			{
				serializeCodePointChar(dstStream, ',', options.jsonEncoding);
			}
			else [[unlikely]]
			{
				appendcomma = true;
			}
			serializeValue(dstStream, &item, options);
			copy.pop();
		}
		serializeCodePointChar(dstStream, ']', options.jsonEncoding);
	}

#pragma endregion

#pragma region Deserialization

	template<class TValue>
	void deserialize(const std::string& srcJson, TValue& dstValue, const Options& options)
	{
		std::istringstream srcStream(srcJson);
		deserialize(srcStream, dstValue, options);
	}

	template<class TValue>
	void deserialize(std::ifstream&& srcStream, TValue& dstValue, const Options& options)
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
		deserialize(srcStream2, dstValue, options2);
	}

	template<class TValue>
	void deserialize(std::ifstream& srcStream, TValue& dstValue, const Options& options)
	{
		Options options2(options);
		if (srcStream.gcount() == 0) [[likely]]
		{
			readBOM(srcStream, options2.jsonEncoding);
			if (options.jsonEncoding != JSON_DEFAULT_ENCODING) [[unlikely]]
			{
				options2.jsonEncoding = options.jsonEncoding;
			}
		}
		deserialize(srcStream, dstValue, options2);
	}

	template<class TValue>
	void deserialize(std::istream&& srcStream, TValue& dstValue, const Options& options)
	{
		std::istream stream2(srcStream.rdbuf());
		deserialize(stream2, dstValue, options);
	}

	template<class TValue>
	void deserialize(std::istream& srcStream, TValue& dstValue, const Options& options)
	{
		deserializeValue(srcStream, &dstValue, options);
	}

	template<class TValue, class... TValues>
	void deserializeFieldValue(const Value& dstValue, const Options& options, const FieldValue<TValue>& dstFieldValue, const FieldValue<TValues>&... dstFieldValues)
	{
		if (dstValue.type != Type::Object) [[unlikely]]
		{
			throw std::runtime_error("json_deserialize_notanobject");
		}
		auto it = dstValue.ptrObject->find(dstFieldValue.field.name);
		if (it != dstValue.ptrObject->end()) [[likely]]
		{
			deserializeValue(it->second, &dstFieldValue.value, Options(JSON_DEFAULT_ENCODING, (dstFieldValue.field.memberEncoding != JSON_DEFAULT_ENCODING ? dstFieldValue.field.memberEncoding : options.memberEncoding), (dstFieldValue.field.precision != -1 ? dstFieldValue.field.precision : options.precision)));
		}
		deserializeFieldValue(dstValue, options, dstFieldValues...);
	}

	template<class TValue, class... TValues>
	void deserializeFieldValue(const Value& dstValue, const FieldValue<TValue>& dstFieldValue, const FieldValue<TValues>&... dstFieldValues)
	{
		deserializeFieldValue(dstValue, Options(), dstFieldValues...);
	}

	template<class TValue>
	void deserializeValue(std::istream& srcStream, TValue* dstValue, const Options& options)
	{
		dstValue->fromJSON(srcStream, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::optional<TValue>* dstOptional, const Options& options)
	{
		uint32_t codePoint = 0;
		int32_t size = 0;
		size += readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
		if (codePoint == 'n') [[likely]]
		{
			size += readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
			if (codePoint == 'u') [[likely]]
			{
				size += readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
				if (codePoint == 'l') [[likely]]
				{
					size += readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
					if (codePoint == 'l') [[likely]]
					{
						if (dstOptional) [[likely]]
						{
							*dstOptional = std::nullopt;
						}
						return;
					}
				}
			}
		}

			if (codePoint == JSON_EOF) [[unlikely]]
			{
				throw std::runtime_error("json_deserialize_null_truncated");
			}

					JSON_UNGET(srcStream, size);

					if (dstOptional) [[likely]]
					{
						if (!*dstOptional) [[unlikely]]
						{
							*dstOptional = TValue();
						}
						deserializeValue(srcStream, &**dstOptional, options);
					}
					else [[unlikely]]
					{
						deserializeValue(srcStream, (TValue*)nullptr, options);
					}
	}

	template<class TValue, std::size_t TSize>
	void deserializeValue(std::istream& srcStream, std::array<TValue, TSize>* dstContainer, const Options& options)
	{
		deserializeValueContainer<std::array<TValue, TSize>, TValue>(srcStream, dstContainer, [](auto* dstContainer, auto* value, auto index) { (*dstContainer)[index] = std::move(*value); }, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::vector<TValue>* dstContainer, const Options& options)
	{
		if (dstContainer && !dstContainer->empty()) [[unlikely]]
		{
			dstContainer->clear();
		}
		deserializeValueContainer<std::vector<TValue>, TValue>(srcStream, dstContainer, [](auto* dstContainer, auto* value, auto) { dstContainer->emplace_back(std::move(*value)); }, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::deque<TValue>* dstContainer, const Options& options)
	{
		if (dstContainer && !dstContainer->empty()) [[unlikely]]
		{
			dstContainer->clear();
		}
		deserializeValueContainer<std::deque<TValue>, TValue>(srcStream, dstContainer, [](auto* dstContainer, auto* value, auto) { dstContainer->emplace_back(std::move(*value)); }, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::forward_list<TValue>* dstContainer, const Options& options)
	{
		if (dstContainer && !dstContainer->empty()) [[unlikely]]
		{
			dstContainer->clear();
		}
		deserializeValueContainerIt<std::forward_list<TValue>, TValue>(srcStream, dstContainer, [](auto* dstContainer) { return dstContainer->before_begin(); }, [](auto* dstContainer, auto* value, auto& it) { dstContainer->emplace_after(it, std::move(*value)); it++; }, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::list<TValue>* dstContainer, const Options& options)
	{
		if (dstContainer && !dstContainer->empty()) [[unlikely]]
		{
			dstContainer->clear();
		}
		deserializeValueContainer<std::list<TValue>, TValue>(srcStream, dstContainer, [](auto* dstContainer, auto* value, auto) { dstContainer->emplace_back(std::move(*value)); }, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::set<TValue>* dstContainer, const Options& options)
	{
		if (dstContainer && !dstContainer->empty()) [[unlikely]]
		{
			dstContainer->clear();
		}
		deserializeValueContainer<std::set<TValue>, TValue>(srcStream, dstContainer, [](auto* dstContainer, auto* value, auto) { dstContainer->emplace(std::move(*value)); }, options);
	}

	template<class TKey, class TValue>
	void deserializeValue(std::istream& srcStream, std::map<TKey, TValue>* dstContainer, const Options& options)
	{
		if (dstContainer && !dstContainer->empty()) [[unlikely]]
		{
			dstContainer->clear();
		}
		deserializeValueMap<std::map<TKey, TValue>, TKey, TValue>(srcStream, dstContainer, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::multiset<TValue>* dstContainer, const Options& options)
	{
		if (dstContainer && !dstContainer->empty()) [[unlikely]]
		{
			dstContainer->clear();
		}
		deserializeValueContainer<std::multiset<TValue>, TValue>(srcStream, dstContainer, [](auto* dstContainer, auto* value, auto) { dstContainer->emplace(std::move(*value)); }, options);
	}

	template<class TKey, class TValue>
	void deserializeValue(std::istream& srcStream, std::multimap<TKey, TValue>* dstContainer, const Options& options)
	{
		if (dstContainer && !dstContainer->empty()) [[unlikely]]
		{
			dstContainer->clear();
		}
		deserializeValueMap<std::multimap<TKey, TValue>, TKey, TValue>(srcStream, dstContainer, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::unordered_set<TValue>* dstContainer, const Options& options)
	{
		if (dstContainer && !dstContainer->empty()) [[unlikely]]
		{
			dstContainer->clear();
		}
		deserializeValueContainerIt<std::unordered_set<TValue>, TValue>(srcStream, dstContainer, [](auto* dstContainer) { return dstContainer->begin(); }, [](auto* dstContainer, auto* value, auto& it) { it = dstContainer->emplace_hint(it, std::move(*value)); }, options);
	}

	template<class TKey, class TValue>
	void deserializeValue(std::istream& srcStream, std::unordered_map<TKey, TValue>* dstContainer, const Options& options)
	{
		if (dstContainer && !dstContainer->empty()) [[unlikely]]
		{
			dstContainer->clear();
		}
		deserializeValueMap<std::unordered_map<TKey, TValue>, TKey, TValue>(srcStream, dstContainer, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::unordered_multiset<TValue>* dstContainer, const Options& options)
	{
		if (dstContainer && !dstContainer->empty()) [[unlikely]]
		{
			dstContainer->clear();
		}
		deserializeValueContainer<std::unordered_multiset<TValue>, TValue>(srcStream, dstContainer, [](auto* dstContainer, auto* value, auto) { dstContainer->emplace(std::move(*value)); }, options);
	}

	template<class TKey, class TValue>
	void deserializeValue(std::istream& srcStream, std::unordered_multimap<TKey, TValue>* dstContainer, const Options& options)
	{
		if (dstContainer && !dstContainer->empty()) [[unlikely]]
		{
			dstContainer->clear();
		}
		deserializeValueMap<std::unordered_multimap<TKey, TValue>, TKey, TValue>(srcStream, dstContainer, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::stack<TValue>* dstContainer, const Options& options)
	{
		std::deque<TValue>* deque = nullptr;
		if (dstContainer) [[likely]]
		{
			deque = new std::deque<TValue>();
		}
		deserializeValueContainer<std::deque<TValue>, TValue>(srcStream, deque, [](auto* dstContainer, auto* value, auto) { dstContainer->emplace_back(std::move(*value)); }, options);
		if (dstContainer) [[likely]]
		{
			*dstContainer = std::stack<TValue>(*deque);
		}
	}

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::queue<TValue>* dstContainer, const Options& options)
	{
		if (dstContainer) [[likely]]
		{
			*dstContainer = std::queue<TValue>();
		}
		deserializeValueContainer<std::queue<TValue>, TValue>(srcStream, dstContainer, [](auto* dstContainer, auto* value, auto) { dstContainer->emplace(std::move(*value)); }, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& srcStream, std::priority_queue<TValue>* dstContainer, const Options& options)
	{
		if (dstContainer) [[likely]]
		{
			*dstContainer = std::priority_queue<TValue>();
		}
		deserializeValueContainer<std::priority_queue<TValue>, TValue>(srcStream, dstContainer, [](auto* dstContainer, auto* value, auto) { dstContainer->emplace(std::move(*value)); }, options);
	}

	template<class TContainer, class TValue>
	void deserializeValueContainer(std::istream& srcStream, TContainer* dstContainer, std::function<void(TContainer*, TValue*, std::size_t index)> append, const Options& options)
	{
		uint32_t codePoint = 0;
		bool first = true;
		std::size_t index = 0;
		while (true)
		{
			readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
			if (codePoint == JSON_EOF) [[unlikely]]
			{
				throw std::runtime_error("json_deserialize_array_truncated");
			}
				if (!first) [[likely]]
				{
					if (codePoint == ']') [[unlikely]]
					{
						return;
					}
						if (codePoint != ',') [[unlikely]]
						{
							throw std::runtime_error("json_deserialize_array_invalid(,)");
						}
				}
				else [[unlikely]]
				{
					if (codePoint != '[') [[unlikely]]
					{
						throw std::runtime_error("json_deserialize_array_invalid([)");
					}
					readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
					if (codePoint == JSON_EOF) [[unlikely]]
					{
						throw std::runtime_error("json_deserialize_array_truncated");
					}
						if (codePoint == ']') [[unlikely]]
						{
							return;
						}
					JSON_UNGET(srcStream, 1);
					first = false;
				}
					if (dstContainer) [[likely]]
					{
						TValue value;
						deserializeValue(srcStream, &value, options);
						append(dstContainer, &value, index);
						index++;
					}
					else [[unlikely]]
					{
						deserializeValue(srcStream, static_cast<TValue*>(nullptr), options);
					}
		}
	}

	template<class TContainer, class TValue>
	void deserializeValueContainerIt(std::istream& srcStream, TContainer* dstContainer, std::function<typename TContainer::iterator(TContainer*)> initIt, std::function<void(TContainer*, TValue*, typename TContainer::iterator&)> append, const Options& options)
	{
		uint32_t codePoint = 0;
		bool first = true;
		typename TContainer::iterator it;
		if (dstContainer) [[likely]]
		{
			it = initIt(dstContainer);
		}
			while (true)
			{
				readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
				if (codePoint == JSON_EOF) [[unlikely]]
				{
					throw std::runtime_error("json_deserialize_array_truncated");
				}
					if (!first) [[likely]]
					{
						if (codePoint == ']') [[unlikely]]
						{
							return;
						}
							if (codePoint != ',') [[unlikely]]
							{
								throw std::runtime_error("json_deserialize_array_invalid(,)");
							}
					}
					else [[unlikely]]
					{
						if (codePoint != '[') [[unlikely]]
						{
							throw std::runtime_error("json_deserialize_array_invalid([)");
						}
						int32_t bytesCount = readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
						if (codePoint == JSON_EOF) [[unlikely]]
						{
							throw std::runtime_error("json_deserialize_array_truncated");
						}
							if (codePoint == ']') [[unlikely]]
							{
								return;
							}
						JSON_UNGET(srcStream, bytesCount);
						first = false;
					}
						if (dstContainer) [[likely]]
						{
							TValue value;
							deserializeValue(srcStream, &value, options);
							append(dstContainer, &value, it);
						}
						else [[unlikely]]
						{
							deserializeValue(srcStream, static_cast<TValue*>(nullptr), options);
						}
			}
	}

	template<class TContainer, class TKey, class TValue>
	void deserializeValueMap(std::istream& srcStream, TContainer* dstContainer, const Options& options)
	{
		uint32_t codePoint = 0;
		bool first = true;
		while (true)
		{
			readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
			if (codePoint == JSON_EOF) [[unlikely]]
			{
				throw std::runtime_error("json_deserialize_map_truncated");
			}
				if (!first) [[likely]]
				{
					if (codePoint == '}') [[unlikely]]
					{
						return;
					}
						if (codePoint != ',') [[unlikely]]
						{
							throw std::runtime_error("json_deserialize_map_invalid(,)");
						}
				}
				else [[unlikely]]
				{
					if (codePoint != '{') [[unlikely]]
					{
						throw std::runtime_error("json_deserialize_map_invalid({)");
					}
					int32_t bytesCount = readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
					if (codePoint == JSON_EOF) [[unlikely]]
					{
						throw std::runtime_error("json_deserialize_map_truncated");
					}
						if (codePoint == '}') [[unlikely]]
						{
							return;
						}
					JSON_UNGET(srcStream, bytesCount);
					first = false;
				}
			std::string field;
			if (dstContainer) [[likely]]
			{
				deserializeValue(srcStream, &field, options);
			}
			else [[unlikely]]
			{
				deserializeValue(srcStream, (std::string*)nullptr, options);
			}
			readCharNoSpaces(srcStream, codePoint, options.jsonEncoding);
			if (codePoint == JSON_EOF) [[unlikely]]
			{
				throw std::runtime_error("json_deserialize_map_truncated");
			}
				if (codePoint != ':') [[unlikely]]
				{
					throw std::runtime_error("json_deserialize_map_invalid(:)");
				}
					if (dstContainer) [[likely]]
					{
						TKey key;
						std::istringstream stream2(field);
						stream2 >> key;
						TValue value;
						deserializeValue(srcStream, &value, options);
						dstContainer->insert({ key , value });
					}
					else [[unlikely]]
					{
						deserializeValue(srcStream, (TValue*)nullptr, options);
					}
		}
	}

	template<class TValue>
	void deserializeValue(const Value& srcValue, TValue* dstValue, const Options& options)
	{
		dstValue->fromJSON(srcValue, options);
	}

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::optional<TValue>* dstOptional, const Options& options)
	{
		if (srcValue == Type::Null) [[likely]]
		{
			if (*dstOptional) [[unlikely]]
			{
				*dstOptional = std::nullopt;
			}
		}
		else [[unlikely]]
		{
			if (!*dstOptional) [[likely]]
			{
				*dstOptional = TValue();
				deserializeValue(srcValue, &**dstOptional, options);
			}
		}
	}

	template<class TValue, std::size_t TSize>
	void deserializeValue(const Value& srcValue, std::array<TValue, TSize>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Array) [[likely]]
		{
			std::size_t index = 0;
			for (auto& item : *srcValue.ptrArray)
			{
				auto& value2 = (*dstContainer)[index];
				deserializeValue(item, &value2, options);
				index++;
			}
			return;
		}
		throw std::runtime_error("json_deserialize_array_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::vector<TValue>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Array) [[likely]]
		{
			dstContainer->resize(srcValue.ptrArray->size());
			std::size_t index = 0;
			for (auto& item : *srcValue.ptrArray)
			{
				auto& value2 = (*dstContainer)[index];
				deserializeValue(item, &value2, options);
				index++;
			}
			return;
		}
		throw std::runtime_error("json_deserialize_vector_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::deque<TValue>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Array) [[likely]]
		{
			dstContainer->resize(srcValue.ptrArray->size());
			std::size_t index = 0;
			for (auto& item : *srcValue.ptrArray)
			{
				auto& value2 = (*dstContainer)[index];
				deserializeValue(item, &value2, options);
				index++;
			}
			return;
		}
		throw std::runtime_error("json_deserialize_vector_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::forward_list<TValue>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Array) [[likely]]
		{
			dstContainer->resize(srcValue.ptrArray->size());
			auto it = dstContainer->begin();
			for (auto& item : *srcValue.ptrArray)
			{
				auto& value2 = *it;
				deserializeValue(item, &value2, options);
				it++;
			}
			return;
		}
		throw std::runtime_error("json_deserialize_forwardlist_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::list<TValue>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Array) [[likely]]
		{
			dstContainer->resize(srcValue.ptrArray->size());
			auto it = dstContainer->begin();
			for (auto& item : *srcValue.ptrArray)
			{
				auto& value2 = *it;
				deserializeValue(item, &value2, options);
				it++;
			}
			return;
		}
		throw std::runtime_error("json_deserialize_list_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::set<TValue>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Array) [[likely]]
		{
			for (auto& item : *srcValue.ptrArray)
			{
				TValue value2;
				deserializeValue(item, &value2, options);
				dstContainer->emplace(std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_set_badtype");
	}

	template<class TKey, class TValue>
	void deserializeValue(const Value& srcValue, std::map<TKey, TValue>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Object) [[likely]]
		{
			for (auto& pair : *srcValue.ptrObject)
			{
				auto& value2 = (*dstContainer)[pair.first];
				deserializeValue(pair.second, &value2, options);
			}
			return;
		}
		throw std::runtime_error("json_deserialize_map_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::multiset<TValue>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Array) [[likely]]
		{
			for (auto& item : *srcValue.ptrArray)
			{
				TValue value2;
				deserializeValue(item, &value2, options);
				dstContainer->emplace(std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_multiset_badtype");
	}

	template<class TKey, class TValue>
	void deserializeValue(const Value& srcValue, std::multimap<TKey, TValue>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Object) [[likely]]
		{
			for (auto& pair : *srcValue.ptrObject)
			{
				TValue value2;
				deserializeValue(pair.second, &value2, options);
				dstContainer->emplace(pair.first, std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_multimap_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::unordered_set<TValue>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Array) [[likely]]
		{
			for (auto& item : *srcValue.ptrArray)
			{
				TValue value2;
				deserializeValue(item, &value2, options);
				dstContainer->emplace(std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_unorderedset_badtype");
	}

	template<class TKey, class TValue>
	void deserializeValue(const Value& srcValue, std::unordered_map<TKey, TValue>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Object) [[likely]]
		{
			for (auto& pair : *srcValue.ptrObject)
			{
				auto& value2 = (*dstContainer)[pair.first];
				deserializeValue(pair.second, &value2, options);
			}
			return;
		}
		throw std::runtime_error("json_deserialize_unorderedmap_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::unordered_multiset<TValue>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Array) [[likely]]
		{
			for (auto& item : *srcValue.ptrArray)
			{
				TValue value2;
				deserializeValue(item, &value2, options);
				dstContainer->emplace(std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_unorderedmultiset_badtype");
	}

	template<class TKey, class TValue>
	void deserializeValue(const Value& srcValue, std::unordered_multimap<TKey, TValue>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Object) [[likely]]
		{
			for (auto& pair : *srcValue.ptrObject)
			{
				TValue value2;
				deserializeValue(pair.second, &value2, options);
				dstContainer->emplace(pair.first, std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_unorderedmultimap_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::stack<TValue>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Array) [[likely]]
		{
			for (auto& item : *srcValue.ptrArray)
			{
				TValue value2;
				deserializeValue(item, &value2, options);
				dstContainer->emplace(std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_stack_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::queue<TValue>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Array) [[likely]]
		{
			for (auto& item : *srcValue.ptrArray)
			{
				TValue value2;
				deserializeValue(item, &value2, options);
				dstContainer->emplace(std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_queue_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& srcValue, std::priority_queue<TValue>* dstContainer, const Options& options)
	{
		if (srcValue == Type::Array) [[likely]]
		{
			for (auto& item : *srcValue.ptrArray)
			{
				TValue value2;
				deserializeValue(item, &value2, options);
				dstContainer->emplace(std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_priorityqueue_badtype");
	}

#pragma endregion
}

#pragma GCC diagnostic pop
