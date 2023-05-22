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
	Field::Field(const TString& fieldName, const Options& options)
		: options(options)
	{
		std::ostringstream stream;
		stream << fieldName;
		name = stream.str();
	}

#pragma region Serialization

	template<class TValue>
	std::string serialize(const TValue& value, const Options& options)
	{
		std::ostringstream stream;
		serialize(stream, value, options);
		return stream.str();
	}

	template<class TValue>
	void serialize(std::ostream&& stream, const TValue& value, const Options& options)
	{
		std::ostream stream2(stream.rdbuf());
		serialize(stream2, value, options);
	}

	template<class TValue>
	void serialize(std::ostream& stream, const TValue& value, const Options& options)
	{
		Options options2(options);
		serializeValue(stream, &value, options2);
	}

	template<class TValue, class... TValues>
	void serializeFieldValue(std::ostream& stream, Options& options, const FieldValue<TValue>& fieldValue, const FieldValue<TValues>&... fieldValues)
	{
		serializeFieldValue(stream, options, fieldValue);
		serializeUnicodeChar(stream, codePointToUnicode(',', options), options);
		serializeFieldValue(stream, options, fieldValues...);
	}

	template<class TValue>
	void serializeFieldValue(std::ostream& stream, Options& options, const FieldValue<TValue>& fieldValue)
	{
		serializeValue(stream, &fieldValue.field.name, options);
		serializeUnicodeChar(stream, codePointToUnicode(':', options), options);
		serializeValue(stream, &fieldValue.value, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& stream, const TValue* value, Options& options)
	{
		value->toJSON(stream, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::optional<TValue>* value, Options& options)
	{
		if (*value)
		{
			serializeValue(stream, &**value, options);
		}
		else
		{
			serializeUnicodeChar(stream, codePointToUnicode('n', options), options);
			serializeUnicodeChar(stream, codePointToUnicode('u', options), options);
			serializeUnicodeChar(stream, codePointToUnicode('l', options), options);
			serializeUnicodeChar(stream, codePointToUnicode('l', options), options);
		}
	}

	template<class TInteger>
	void serializeValueInteger(std::ostream& stream, const TInteger* value, Options& options)
	{
		if ((options & Encoding::UTF8) == Encoding::UTF8 || (options == Encoding::None && JSON_DEFAULT_ENCODING == Encoding::UTF8)) [[likely]]
		{
			stream << *value;
		}
		else [[unlikely]]
		{
			std::ostringstream stream2;
			stream2 << *value;
			std::string str = stream2.str();
			for (char c : str)
			{
				serializeUnicodeChar(stream, codePointToUnicode(c, options), options);
			}
		}
	}

	template<class TFloat>
	void serializeValueFloat(std::ostream& stream, const TFloat* value, Options& options)
	{
		std::streamsize precision = (options.precision != -1 ? options.precision : std::numeric_limits<TFloat>::max_digits10);
		if ((options & Encoding::UTF8) == Encoding::UTF8 || (options == Encoding::None && JSON_DEFAULT_ENCODING == Encoding::UTF8)) [[likely]]
		{
			stream << std::setprecision(precision) << *value;
		}
		else [[unlikely]]
		{
			std::ostringstream stream2;
			stream2 << std::setprecision(precision) << *value;
			std::string str = stream2.str();
			for (char c : str)
			{
				serializeUnicodeChar(stream, codePointToUnicode(c, options), options);
			}
		}
	}

	template<class TArray>
	void serializeValueArray(std::ostream& stream, const TArray* value, Options& options)
	{
		serializeUnicodeChar(stream, codePointToUnicode('[', options), options);
		bool appendcomma = false;
		for (const auto& item : *value)
		{
			if (appendcomma) [[likely]]
			{
				serializeUnicodeChar(stream, codePointToUnicode(',', options), options);
			}
			else [[unlikely]]
			{
				appendcomma = true;
			}
			serializeValue(stream, &item, options);
		}
		serializeUnicodeChar(stream, codePointToUnicode(']', options), options);
	}

	template<class TValue, std::size_t TArraySize>
	void serializeValue(std::ostream& stream, const class std::array<TValue, TArraySize>* value, Options& options)
	{
		serializeValueArray(stream, value, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::vector<TValue>* value, Options& options)
	{
		serializeValueArray(stream, value, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::deque<TValue>* value, Options& options)
	{
		serializeValueArray(stream, value, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::forward_list<TValue>* value, Options& options)
	{
		serializeValueArray(stream, value, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::list<TValue>* value, Options& options)
	{
		serializeValueArray(stream, value, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::set<TValue>* value, Options& options)
	{
		serializeValueArray(stream, value, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::multiset<TValue>* value, Options& options)
	{
		serializeValueArray(stream, value, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::unordered_set<TValue>* value, Options& options)
	{
		serializeValueArray(stream, value, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::unordered_multiset<TValue>* value, Options& options)
	{
		serializeValueArray(stream, value, options);
	}

	template<class TObject>
	void serializeValueObject(std::ostream& stream, const TObject* value, Options& options)
	{
		serializeUnicodeChar(stream, codePointToUnicode('{', options), options);
		bool appendcomma = false;
		for (const auto& pair : *value)
		{
			if (appendcomma) [[likely]]
			{
				serializeUnicodeChar(stream, codePointToUnicode(',', options), options);
			}
			else [[unlikely]]
			{
				appendcomma = true;
			}
			serializeFieldValue(stream, options, FieldValue<const decltype(pair.second)>(Field(pair.first, options), pair.second));
		}
		serializeUnicodeChar(stream, codePointToUnicode('}', options), options);
	}

	template<class TField, class TValue>
	void serializeValue(std::ostream& stream, const std::map<TField, TValue>* value, Options& options)
	{
		serializeValueObject(stream, value, options);
	}

	template<class TField, class TValue>
	void serializeValue(std::ostream& stream, const std::multimap<TField, TValue>* value, Options& options)
	{
		serializeValueObject(stream, value, options);
	}

	template<class TField, class TValue>
	void serializeValue(std::ostream& stream, const std::unordered_map<TField, TValue>* value, Options& options)
	{
		serializeValueObject(stream, value, options);
	}

	template<class TField, class TValue>
	void serializeValue(std::ostream& stream, const std::unordered_multimap<TField, TValue>* value, Options& options)
	{
		serializeValueObject(stream, value, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::stack<TValue>* value, Options& options)
	{
		std::stack<TValue> copy(*value);
		std::deque<TValue> copy2;
		while (!copy.empty())
		{
			copy2.emplace_front(std::move(copy.top()));
			copy.pop();
		}
		serializeValue(stream, &copy2, options);
	}

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::queue<TValue>* value, Options& options)
	{
		std::queue<TValue> copy(*value);
		serializeUnicodeChar(stream, codePointToUnicode('[', options), options);
		bool appendcomma = false;
		while (!copy.empty())
		{
			const TValue& item = copy.front();
			if (appendcomma) [[likely]]
			{
				serializeUnicodeChar(stream, codePointToUnicode(',', options), options);
			}
			else [[unlikely]]
			{
				appendcomma = true;
			}
			serializeValue(stream, &item, options);
			copy.pop();
		}
		serializeUnicodeChar(stream, codePointToUnicode(']', options), options);
	}

	template<class TValue>
	void serializeValue(std::ostream& stream, const std::priority_queue<TValue>* value, Options& options)
	{
		std::priority_queue<TValue> copy(*value);
		serializeUnicodeChar(stream, codePointToUnicode('[', options), options);
		bool appendcomma = false;
		while (!copy.empty())
		{
			const TValue& item = copy.top();
			if (appendcomma) [[likely]]
			{
				serializeUnicodeChar(stream, codePointToUnicode(',', options), options);
			}
			else [[unlikely]]
			{
				appendcomma = true;
			}
			serializeValue(stream, &item, options);
			copy.pop();
		}
		serializeUnicodeChar(stream, codePointToUnicode(']', options), options);
	}

#pragma endregion

#pragma region Deserialization

	template<class TValue>
	void deserialize(const std::string& json, TValue& value, const Options& options)
	{
		std::istringstream stream(json);
		deserialize(stream, value, options);
	}

	template<class TValue>
	void deserialize(std::istream&& stream, TValue& value, const Options& options)
	{
		std::istream stream2(stream.rdbuf());
		deserialize(stream2, value, options);
	}

	template<class TValue>
	void deserialize(std::istream& stream, TValue& value, const Options& options)
	{
		Options options2(options);
		deserializeValue(stream, &value, options2);
	}

	template<class TValue, class... TValues>
	void deserializeFieldValue(const Value& value, const FieldValue<TValue>& fieldValue, const FieldValue<TValues>&... fieldValues)
	{
		if (value.type != Type::Object) [[unlikely]]
		{
			throw std::runtime_error("json_deserialize_notanobject");
		}
		auto it = value.ptrObject->find(fieldValue.field.name);
		if (it != value.ptrObject->end()) [[likely]]
		{
			deserializeValue(it->second, &fieldValue.value);
		}
		deserializeFieldValue(value, fieldValues...);
	}

	template<class TValue>
	void deserializeValue(std::istream& stream, TValue* value, Options& options)
	{
		value->fromJSON(stream, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& stream, std::optional<TValue>* value, Options& options)
	{
		int32_t codePoint = 0;
		int size = 0;
		size += JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
		if (codePoint == 'n') [[likely]]
		{
			size += JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
			if (codePoint == 'u') [[likely]]
			{
				size += JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
				if (codePoint == 'l') [[likely]]
				{
					size += JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
					if (codePoint == 'l') [[likely]]
					{
						if (value) [[likely]]
						{
							*value = std::nullopt;
						}
						return;
					}
				}
			}
		}
			if (codePoint == EOF) [[unlikely]]
			{
				throw std::runtime_error("json_deserialize_null_truncated");
			}

					for (int i = 0; i < size; i++)
					{
						stream.unget();
					}

					if (value) [[likely]]
					{
						if (!*value) [[unlikely]]
						{
							*value = TValue();
						}
						deserializeValue(stream, &**value, options);
					}
					else [[unlikely]]
					{
						deserializeValue(stream, (TValue*)nullptr, options);
					}
	}

	template<class TValue, std::size_t TSize>
	void deserializeValue(std::istream& stream, std::array<TValue, TSize>* container, Options& options)
	{
		deserializeValueContainer<std::array<TValue, TSize>, TValue>(stream, container, [](auto* container, auto* value, auto index) { (*container)[index] = std::move(*value); }, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& stream, std::vector<TValue>* container, Options& options)
	{
		if (container) [[likely]]
		{
			container->clear();
		}
		deserializeValueContainer<std::vector<TValue>, TValue>(stream, container, [](auto* container, auto* value, auto) { container->emplace_back(std::move(*value)); }, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& stream, std::deque<TValue>* container, Options& options)
	{
		if (container) [[likely]]
		{
			container->clear();
		}
		deserializeValueContainer<std::deque<TValue>, TValue>(stream, container, [](auto* container, auto* value, auto) { container->emplace_back(std::move(*value)); }, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& stream, std::forward_list<TValue>* container, Options& options)
	{
		if (container) [[likely]]
		{
			container->clear();
		}
		deserializeValueContainerIt<std::forward_list<TValue>, TValue>(stream, container, [](auto* container) { return container->before_begin(); }, [](auto* container, auto* value, auto& it) { container->emplace_after(it, std::move(*value)); it++; }, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& stream, std::list<TValue>* container, Options& options)
	{
		if (container) [[likely]]
		{
			container->clear();
		}
		deserializeValueContainer<std::list<TValue>, TValue>(stream, container, [](auto* container, auto* value, auto) { container->emplace_back(std::move(*value)); }, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& stream, std::set<TValue>* container, Options& options)
	{
		if (container) [[likely]]
		{
			container->clear();
		}
		deserializeValueContainer<std::set<TValue>, TValue>(stream, container, [](auto* container, auto* value, auto) { container->emplace(std::move(*value)); }, options);
	}

	template<class TKey, class TValue>
	void deserializeValue(std::istream& stream, std::map<TKey, TValue>* container, Options& options)
	{
		if (container) [[likely]]
		{
			container->clear();
		}
		deserializeValueMap<std::map<TKey, TValue>, TKey, TValue>(stream, container, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& stream, std::multiset<TValue>* container, Options& options)
	{
		if (container) [[likely]]
		{
			container->clear();
		}
		deserializeValueContainer<std::multiset<TValue>, TValue>(stream, container, [](auto* container, auto* value, auto) { container->emplace(std::move(*value)); }, options);
	}

	template<class TKey, class TValue>
	void deserializeValue(std::istream& stream, std::multimap<TKey, TValue>* container, Options& options)
	{
		if (container) [[likely]]
		{
			container->clear();
		}
		deserializeValueMap<std::multimap<TKey, TValue>, TKey, TValue>(stream, container, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& stream, std::unordered_set<TValue>* container, Options& options)
	{
		if (container) [[likely]]
		{
			container->clear();
		}
		deserializeValueContainerIt<std::unordered_set<TValue>, TValue>(stream, container, [](auto* container) { return container->begin(); }, [](auto* container, auto* value, auto& it) { it = container->emplace_hint(it, std::move(*value)); }, options);
	}

	template<class TKey, class TValue>
	void deserializeValue(std::istream& stream, std::unordered_map<TKey, TValue>* container, Options& options)
	{
		if (container) [[likely]]
		{
			container->clear();
		}
		deserializeValueMap<std::unordered_map<TKey, TValue>, TKey, TValue>(stream, container, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& stream, std::unordered_multiset<TValue>* container, Options& options)
	{
		if (container) [[likely]]
		{
			container->clear();
		}
		deserializeValueContainer<std::unordered_multiset<TValue>, TValue>(stream, container, [](auto* container, auto* value, auto) { container->emplace(std::move(*value)); }, options);
	}

	template<class TKey, class TValue>
	void deserializeValue(std::istream& stream, std::unordered_multimap<TKey, TValue>* container, Options& options)
	{
		if (container) [[likely]]
		{
			container->clear();
		}
		deserializeValueMap<std::unordered_multimap<TKey, TValue>, TKey, TValue>(stream, container, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& stream, std::stack<TValue>* container, Options& options)
	{
		std::deque<TValue>* deque = nullptr;
		if (container) [[likely]]
		{
			deque = new std::deque<TValue>();
		}
		deserializeValueContainer<std::deque<TValue>, TValue>(stream, deque, [](auto* container, auto* value, auto) { container->emplace_back(std::move(*value)); }, options);
		if (container) [[likely]]
		{
			*container = std::stack<TValue>(*deque);
		}
	}

	template<class TValue>
	void deserializeValue(std::istream& stream, std::queue<TValue>* container, Options& options)
	{
		if (container) [[likely]]
		{
			*container = std::queue<TValue>();
		}
		deserializeValueContainer<std::queue<TValue>, TValue>(stream, container, [](auto* container, auto* value, auto) { container->emplace(std::move(*value)); }, options);
	}

	template<class TValue>
	void deserializeValue(std::istream& stream, std::priority_queue<TValue>* container, Options& options)
	{
		if (container) [[likely]]
		{
			*container = std::priority_queue<TValue>();
		}
		deserializeValueContainer<std::priority_queue<TValue>, TValue>(stream, container, [](auto* container, auto* value, auto) { container->emplace(std::move(*value)); }, options);
	}

	template<class TContainer, class TValue>
	void deserializeValueContainer(std::istream& stream, TContainer* container, std::function<void(TContainer*, TValue*, std::size_t index)> append, Options& options)
	{
		int32_t codePoint = 0;
		bool first = true;
		std::size_t index = 0;
		while (true)
		{
			JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
			if (codePoint == EOF) [[unlikely]]
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
					JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
					if (codePoint == EOF) [[unlikely]]
					{
						throw std::runtime_error("json_deserialize_array_truncated");
					}
						if (codePoint == ']') [[unlikely]]
						{
							return;
						}
					stream.unget();
					first = false;
				}
					if (container) [[likely]]
					{
						TValue value;
						deserializeValue(stream, &value, options);
						append(container, &value, index);
						index++;
					}
					else [[unlikely]]
					{
						deserializeValue(stream, static_cast<TValue*>(nullptr), options);
					}
		}
	}

	template<class TContainer, class TValue>
	void deserializeValueContainerIt(std::istream& stream, TContainer* container, std::function<typename TContainer::iterator(TContainer*)> initIt, std::function<void(TContainer*, TValue*, typename TContainer::iterator&)> append, Options& options)
	{
		int32_t codePoint = 0;
		bool first = true;
		typename TContainer::iterator it;
		if (container) [[likely]]
		{
			it = initIt(container);
		}
			while (true)
			{
				JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
				if (codePoint == EOF) [[unlikely]]
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
						int bytesCount = JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
						if (codePoint == EOF) [[unlikely]]
						{
							throw std::runtime_error("json_deserialize_array_truncated");
						}
							if (codePoint == ']') [[unlikely]]
							{
								return;
							}
						JSON_UNGET(stream, bytesCount);
						first = false;
					}
						if (container) [[likely]]
						{
							TValue value;
							deserializeValue(stream, &value, options);
							append(container, &value, it);
						}
						else [[unlikely]]
						{
							deserializeValue(stream, static_cast<TValue*>(nullptr), options);
						}
			}
	}

	template<class TContainer, class TKey, class TValue>
	void deserializeValueMap(std::istream& stream, TContainer* map, Options& options)
	{
		int32_t codePoint = 0;
		bool first = true;
		while (true)
		{
			JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
			if (codePoint == EOF) [[unlikely]]
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
					int bytesCount = JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
					if (codePoint == EOF) [[unlikely]]
					{
						throw std::runtime_error("json_deserialize_map_truncated");
					}
						if (codePoint == '}') [[unlikely]]
						{
							return;
						}
					JSON_UNGET(stream, bytesCount);
					first = false;
				}
			std::string field;
			if (map) [[likely]]
			{
				deserializeValue(stream, &field, options);
			}
			else [[unlikely]]
			{
				deserializeValue(stream, (std::string*)nullptr, options);
			}
			JSON_READ_CHAR_NO_SPACES(stream, codePoint, options.encoding);
			if (codePoint == EOF) [[unlikely]]
			{
				throw std::runtime_error("json_deserialize_map_truncated");
			}
				if (codePoint != ':') [[unlikely]]
				{
					throw std::runtime_error("json_deserialize_map_invalid(:)");
				}
					if (map) [[likely]]
					{
						TKey key;
						std::istringstream stream2(field);
						stream2 >> key;
						TValue value;
						deserializeValue(stream, &value, options);
						map->insert({ key , value });
					}
					else [[unlikely]]
					{
						deserializeValue(stream, (TValue*)nullptr, options);
					}
		}
	}

	template<class TValue>
	void deserializeValue(const Value& value, TValue* value2)
	{
		value2->fromJSON(value);
	}

	template<class TValue>
	void deserializeValue(const Value& value, std::optional<TValue>* optional)
	{
		if (value == Type::Null) [[likely]]
		{
			if (*optional) [[unlikely]]
			{
				*optional = std::nullopt;
			}
		}
		else [[unlikely]]
		{
			if (!*optional) [[likely]]
			{
				*optional = TValue();
				deserializeValue(value, &**optional);
			}
		}
	}

	template<class TValue, std::size_t TSize>
	void deserializeValue(const Value& value, std::array<TValue, TSize>* container)
	{
		if (value == Type::Array) [[likely]]
		{
			std::size_t index = 0;
			for (auto& item : *value.ptrArray)
			{
				auto& value2 = (*container)[index];
				deserializeValue(item, &value2);
				index++;
			}
			return;
		}
		throw std::runtime_error("json_deserialize_array_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& value, std::vector<TValue>* container)
	{
		if (value == Type::Array) [[likely]]
		{
			container->resize(value.ptrArray->size());
			std::size_t index = 0;
			for (auto& item : *value.ptrArray)
			{
				auto& value2 = (*container)[index];
				deserializeValue(item, &value2);
				index++;
			}
			return;
		}
		throw std::runtime_error("json_deserialize_vector_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& value, std::deque<TValue>* container)
	{
		if (value == Type::Array) [[likely]]
		{
			container->resize(value.ptrArray->size());
			std::size_t index = 0;
			for (auto& item : *value.ptrArray)
			{
				auto& value2 = (*container)[index];
				deserializeValue(item, &value2);
				index++;
			}
			return;
		}
		throw std::runtime_error("json_deserialize_vector_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& value, std::forward_list<TValue>* container)
	{
		if (value == Type::Array) [[likely]]
		{
			container->resize(value.ptrArray->size());
			auto it = container->begin();
			for (auto& item : *value.ptrArray)
			{
				auto& value2 = *it;
				deserializeValue(item, &value2);
				it++;
			}
			return;
		}
		throw std::runtime_error("json_deserialize_forwardlist_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& value, std::list<TValue>* container)
	{
		if (value == Type::Array) [[likely]]
		{
			container->resize(value.ptrArray->size());
			auto it = container->begin();
			for (auto& item : *value.ptrArray)
			{
				auto& value2 = *it;
				deserializeValue(item, &value2);
				it++;
			}
			return;
		}
		throw std::runtime_error("json_deserialize_list_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& value, std::set<TValue>* container)
	{
		if (value == Type::Array) [[likely]]
		{
			for (auto& item : *value.ptrArray)
			{
				TValue value2;
				deserializeValue(item, &value2);
				container->emplace(std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_set_badtype");
	}

	template<class TKey, class TValue>
	void deserializeValue(const Value& value, std::map<TKey, TValue>* container)
	{
		if (value == Type::Object) [[likely]]
		{
			for (auto& pair : *value.ptrObject)
			{
				auto& value2 = (*container)[pair.first];
				deserializeValue(pair.second, &value2);
			}
			return;
		}
		throw std::runtime_error("json_deserialize_map_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& value, std::multiset<TValue>* container)
	{
		if (value == Type::Array) [[likely]]
		{
			for (auto& item : *value.ptrArray)
			{
				TValue value2;
				deserializeValue(item, &value2);
				container->emplace(std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_multiset_badtype");
	}

	template<class TKey, class TValue>
	void deserializeValue(const Value& value, std::multimap<TKey, TValue>* container)
	{
		if (value == Type::Object) [[likely]]
		{
			for (auto& pair : *value.ptrObject)
			{
				TValue value2;
				deserializeValue(pair.second, &value2);
				container->emplace(pair.first, std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_multimap_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& value, std::unordered_set<TValue>* container)
	{
		if (value == Type::Array) [[likely]]
		{
			for (auto& item : *value.ptrArray)
			{
				TValue value2;
				deserializeValue(item, &value2);
				container->emplace(std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_unorderedset_badtype");
	}

	template<class TKey, class TValue>
	void deserializeValue(const Value& value, std::unordered_map<TKey, TValue>* container)
	{
		if (value == Type::Object) [[likely]]
		{
			for (auto& pair : *value.ptrObject)
			{
				auto& value2 = (*container)[pair.first];
				deserializeValue(pair.second, &value2);
			}
			return;
		}
		throw std::runtime_error("json_deserialize_unorderedmap_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& value, std::unordered_multiset<TValue>* container)
	{
		if (value == Type::Array) [[likely]]
		{
			for (auto& item : *value.ptrArray)
			{
				TValue value2;
				deserializeValue(item, &value2);
				container->emplace(std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_unorderedmultiset_badtype");
	}

	template<class TKey, class TValue>
	void deserializeValue(const Value& value, std::unordered_multimap<TKey, TValue>* container)
	{
		if (value == Type::Object) [[likely]]
		{
			for (auto& pair : *value.ptrObject)
			{
				TValue value2;
				deserializeValue(pair.second, &value2);
				container->emplace(pair.first, std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_unorderedmultimap_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& value, std::stack<TValue>* container)
	{
		if (value == Type::Array) [[likely]]
		{
			for (auto& item : *value.ptrArray)
			{
				TValue value2;
				deserializeValue(item, &value2);
				container->emplace(std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_stack_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& value, std::queue<TValue>* container)
	{
		if (value == Type::Array) [[likely]]
		{
			for (auto& item : *value.ptrArray)
			{
				TValue value2;
				deserializeValue(item, &value2);
				container->emplace(std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_queue_badtype");
	}

	template<class TValue>
	void deserializeValue(const Value& value, std::priority_queue<TValue>* container)
	{
		if (value == Type::Array) [[likely]]
		{
			for (auto& item : *value.ptrArray)
			{
				TValue value2;
				deserializeValue(item, &value2);
				container->emplace(std::move(value2));
			}
			return;
		}
		throw std::runtime_error("json_deserialize_priorityqueue_badtype");
	}

#pragma endregion
}

#pragma GCC diagnostic pop
