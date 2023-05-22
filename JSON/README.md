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
		JSON::Field("PiField", 2), pi, // Set float precision to 2 digits
		"ProverbField", proverb,
		"FibonacciField", fibonacci
	);
};
```
You should use the types declared in [cstdint](https://en.cppreference.com/w/cpp/header/cstdint).  
*The JSON macro supports up to 50 fields by object (this limitation can be increased manually in the [macros](https://github.com/antlafarge/cpp-tools/blob/main/JSON/src/json_definitions.h#L26)).*

## Serialization

```cpp
Sample object{ {}, { true }, 3.1415926535, "Haste makes waste", { 0,1,1,2,3,5,8,13,21,34 } };
std::string json = JSON::serialize(object);
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

## Deserialization

```cpp
Sample object2;
JSON::deserialize(json, object2);
```

# External object (de)serialization

If you want to (de)serialize an external object that you can't modify, you can implement custom (de)serialization functions.  
Instead of including `json.h`, you have to include :
- `json_definitions.h` before your custom (de)serialization functions.
- `json_implementations.h` after your custom (de)serialization functions.

```cpp
#include "json_definitions.h"

namespace JSON
{
	template<std::size_t TBitsetSize>
	void serializeValue(std::ostream& stream, const std::bitset<TBitsetSize>* value, const Field* field = nullptr)
	{
		stream << '"' << value->to_string() << '"';
	}

	template<std::size_t TBitsetSize>
	void deserializeValue(std::istream& stream, std::bitset<TBitsetSize>* value)
	{
		if (value != nullptr)
		{
			std::string data;
			JSON::deserializeValue(stream, &data);
			(* value) = std::bitset<TBitsetSize>(data);
		}
		else
		{
			JSON::deserializeValue(stream, (std::string*)nullptr);
		}
	}
}

#include "json_implementations.h"

void testCustomDeSerializationFunctions()
{
	std::bitset<2> bs{ 0xA };
	std::string json = JSON::serialize(bs);

	std::bitset<2> bs2;
	JSON::deserialize(json, bs2);

	assert(bs == bs2);
}
```

# (De)serialization from File

```cpp
#include <fstream>
#include "json.h"

std::map<std::string, std::string> data{ { "Field", "Value" } };

// Write to file
JSON::serialize(std::ofstream("data.json"), data);

// Read from file
JSON::deserialize(std::ifstream("data.json"), data);
```

# Not typed (de)serialization

If you want to keep dynamic JSON data in your program, you can use the class `JSON::Value`.

```cpp
auto jsValue = JSON::Value::createObject();
jsValue.getObject()["PI"] = JSON::Value(3.14159);
std::streamsize precision = 3;
std::string json = JSON::serialize(jsValue, JSON::Options(precision));

std::cout << json << std::endl; // {"PI":3.14}

JSON::Value jsValue2;
JSON::deserialize(json, jsValue2);

if (jsValue2 == JSON::Type::Object)
{
	auto it = jsValue2.getObject().find("PI");
	if (it != jsValue2.getObject().end())
	{
		float pi = (float)it->second.getNumberF();
		std::cout << pi << std::endl; // 3.14
	}
}
```
