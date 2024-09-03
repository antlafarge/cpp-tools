JSON
====

Code and macros to easily serialize and deserialize a struct to/from JSON format.

# Custom struct (de)serialization

## Declaration

```cpp
#include "json.h"

struct Sample
{
	std::optional<int32_t> null;
	std::optional<bool> working;
	double_t pi = 0.0;
	std::string proverb;
	std::vector<int32_t> fibonacci;

	JSON(
		"NullField", null,
		"WorkingField", working,
		JSON::Field("PiField", 3), pi, // Set float precision to 2 digits
		"ProverbField", proverb,
		"FibonacciField", fibonacci
	);
};
```
You should use the types declared in [cstdint](https://en.cppreference.com/w/cpp/header/cstdint).  
*The JSON macro supports up to 50 fields by object (this limitation can be increased manually in the [macros](https://github.com/antlafarge/cpp-tools/blob/main/JSON/src/json_definitions.h#L26)).*

## (De)serialization

```cpp
void testDeSerialization()
{
	std::string json;

	// Serialization
	{
		Sample object{ {}, { true }, 3.1415926535, "Haste makes waste", { 0,1,1,2,3,5,8,13,21,34 } };
		json = JSON::serialize(object);
		std::cout << json << std::endl; // {"NullField":null,"WorkingField":true,"PiField":3.14,"ProverbField":"Haste makes waste","FibonacciField":[0,1,1,2,3,5,8,13,21,34]}
	}

	// Deserialization
	{
		Sample object;
		JSON::deserialize(json, object);
	}
}
```

Expected JSON output (formatted for display) :
```json
{
	"NullField": null,
	"WorkingField": true,
	"PiField": 3.14,
	"ProverbField": "Haste makes waste",
	"FibonacciField": [0,1,1,2,3,5,8,13,21,34]
}
```

# External object (de)serialization

If you want to (de)serialize an external object that you can't modify, you can implement custom (de)serialization functions.  
Instead of including `json.h`, you have to include :
- `json_definitions.h` before your custom (de)serialization functions.
- `json_implementations.h` after your custom (de)serialization functions.

```cpp
#include "json_definitions.h"

#include <bitset>
#include <cassert>

namespace JSON
{
	template<std::size_t TBitsetSize>
	void serializeValue(std::ostream& dstStream, const std::bitset<TBitsetSize>* srcValue, const Options& options = Options())
	{
		serializeCodePointChar(dstStream, '"', options.jsonEncoding);
		convert(dstStream, srcValue->to_string(), options.jsonEncoding, JSON::Encoding::UTF8);
		serializeCodePointChar(dstStream, '"', options.jsonEncoding);
	}

	template<std::size_t TBitsetSize>
	void deserializeValue(std::istream& srcStream, std::bitset<TBitsetSize>* dstValue, const Options& options = Options())
	{
		if (dstValue != nullptr)
		{
			std::string dstString;
			JSON::deserializeValue(srcStream, &dstString, options);
			(*dstValue) = std::bitset<TBitsetSize>(dstString);
		}
		else
		{
			// You must handle the case value is null, because this is used to check json validity (functions isValid)
			JSON::deserializeValue(srcStream, static_cast<std::string*>(nullptr), options);
		}
	}
}

#include "json_implementations.h"

void testCustomDeSerializationFunctions()
{
	std::bitset<2> bs{ 0xA };
	std::string json = JSON::serialize(bs);
	std::cout << json << std::endl; // "10"

	std::bitset<2> bs2;
	JSON::deserialize(json, bs2);

	assert(bs == bs2);
}
```

# (De)serialization from File

```cpp
#include <cassert>
#include "json.h"

void testDeSerializeFromFile()
{
	// Write to file
	std::map<std::string, std::string> data { { "Field", "Value" } };
	JSON::serialize(std::ofstream("data.json"), data);

	// Read from file
	std::map<std::string, std::string> data2;
	JSON::deserialize(std::ifstream("data.json"), data2);

	assert(data == data2);
}
```

# Not typed (de)serialization

If you want to keep dynamic JSON data in your program, you can use the class `JSON::Value`.

```cpp
#include "json.h"

void testNotTypedDeSerialization()
{
	std::string json;

	// Serialize
	{
		auto jsValue = JSON::Value::createObject();
		jsValue.getObject()["PI"] = JSON::Value(3.14159);
		int32_t precision = 3;
		json = JSON::serialize(jsValue, JSON::Options(precision));
	}

	std::cout << json << std::endl; // {"PI":3.14}

	// Deserialize
	{
		JSON::Value jsValue2;
		JSON::deserialize(json, jsValue2);

		// Check and get data
		if (jsValue2.isObject())
		{
			auto jsObject2 = jsValue2.getObject();
			auto it = jsObject2.find("PI");
			if (it != jsObject2.end())
			{
				auto jsInnerValue = it->second;
				if (jsInnerValue.isNumberF())
				{
					float pi = static_cast<float>(jsInnerValue.getNumberF());
					std::cout << pi << std::endl; // 3.14
				}
			}
		}
	}
}
```
