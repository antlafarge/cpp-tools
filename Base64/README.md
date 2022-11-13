Base64
======

# Includes

- Low-level api :  `#include "Base64.h"`
- High-level api : `#include "Base64.helpers.h"`

# Encode

## byte array

```cpp
#include "Base64.helpers.h"
#include <iostream>

void encodeData()
{
	uint8_t byteArray[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	auto encodedData1 = Base64::encode(byteArray, 10);
	std::cout << encodedData1 << std::endl; // AAECAwQFBgcICQ==
}
```

## vector<byte>

```cpp
#include "Base64.helpers.h"
#include <iostream>

void encodeData()
{
	std::vector<uint8_t> data = { 3, 2, 1, 0 };
	auto encodedData = Base64::encode(data.data(), data.size());
	std::cout << encodedData << std::endl; // AwIBAA==
}
```

## string

```cpp
#include "Base64.helpers.h"
#include <iostream>

void encodeData()
{
	std::string data = "A string to encode";
	auto encodedData = Base64::encode(data.data(), data.size());
	std::cout << encodedData << std::endl; // QSBzdHJpbmcgdG8gZW5jb2Rl
}
```

## c-string

```cpp
#include "Base64.helpers.h"
#include <cstring>
#include <iostream>

void encodeData()
{
	const char* data = "A c-string to encode";
	auto encodedData = Base64::encode(data, strlen(data));
	std::cout << encodedData << std::endl; // QSBjLXN0cmluZyB0byBlbmNvZGU=
}
```

# Decode

## byte array

```cpp
#include "Base64.helpers.h"
#include <iostream>

void decodeData()
{
	std::vector<uint8_t> decodedData = Base64::decode("CQgHBgUEAwIBAA==");
	for (uint8_t number : decodedData)
	{
		std::cout << (uint16_t)number << " "; // 9 8 7 6 5 4 3 2 1 0
	}
}
```

## string

```cpp
#include "Base64.helpers.h"
#include <iostream>

void decodeData()
{
	std::vector<uint8_t> decodedData = Base64::decode("QSBzdHJpbmcgdG8gZGVjb2Rl");
	std::string decodedString((const char*)decodedData.data(), decodedData.size());
	std::cout << decodedString << std::endl; // A string to decode
}
```
	
# Low-level / No exceptions

```cpp
#include "Base64.h"
#include <cstdio>
#include <cstring>

void encodeData()
{
    const char* data = "Some data to encode";
    uint64_t dataSize = strlen(data);
    uint64_t encodedDataSize = Base64::computeEncodedSize(dataSize);
    char* encodedData = new char[encodedDataSize];

    Base64::Result result = Base64::encode(data, dataSize, encodedData, encodedDataSize);

    if (Base64::succeed(result))
    {
        printf("%s\n", encodedData); // U29tZSBkYXRhIHRvIGVuY29kZQ==
    }
    else
    {
        printf("Base64::encode failed (error %i)\n", (uint32_t)result);
    }
}

void decodeData()
{
    const char* data = "U29tZSBkYXRhIHRvIGRlY29kZQ==";
    uint64_t dataSize = strlen(data);
    uint64_t decodedDataSize = Base64::computeDecodedSize(dataSize, data);
    char* decodedData = new char[decodedDataSize];

    Base64::Result result = Base64::decode(data, dataSize, decodedData, decodedDataSize);

    if (Base64::succeed(result))
    {
        printf("%s\n", decodedData); // Some data to decode
    }
    else
    {
        printf("Base64::decode failed (error %i)\n", (uint32_t)result);
    }
}
```

# Exceptions handling

High-level functions can throw exceptions

```cpp
try
{
	auto encodedData1 = Base64::decode("A", );
}
catch (const std::exception& ex)
{
    std::cerr << "Base64::decode failed (error " << ex.what() << ")" << std::endl; // Base64::decode failed (error base64_decode_96)
}
```

# Error codes

## 1 MissingPadding *(Warning)*

    Warning indicating missing base64 padding characters  
    This is not an error, this is a warning, decoding succeed

## 32 InvalidDestinationBufferSize

    Destination buffer size too small  
    Allocate greater destination buffer

## 96 InvalidSourceBufferSize

    Invalid source buffer size  
    Base64 encoded data size must be a multiple of 4

## 97 InvalidCharacter

    Invalid base64 character
    Source data is not a valid base64 encoded string
