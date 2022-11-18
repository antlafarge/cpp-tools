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
	std::string encodedData = Base64::encode(byteArray, 10);
	std::cout << encodedData << std::endl; // AAECAwQFBgcICQ==
}
```

## vector<byte>

```cpp
#include "Base64.helpers.h"
#include <iostream>

void encodeData()
{
	std::vector<uint8_t> byteVector = { 3, 2, 1, 0 };
	std::string encodedData = Base64::encode(byteVector);
	std::cout << encodedData << std::endl; // AwIBAA==
}
```

## string

```cpp
#include "Base64.helpers.h"
#include <iostream>

void encodeData()
{
	std::string byteString = "A string to encode";
	std::string encodedData = Base64::encode(byteString);
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
	const char* byteCString = "A c-string to encode";
	std::string encodedData = Base64::encode(byteCString, strlen(byteCString));
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
	auto encodedData = Base64::decode("A", 1);
}
catch (const std::exception& ex)
{
    std::cerr << "Base64::decode failed (error " << ex.what() << ")" << std::endl; // Base64::decode failed (error base64_decode_96)
}
```

# Warning codes

If you get a warning as a result, the data was successfully processed, but the result data might be invalid  
When using high-level functions (from `base64.helpers.h`), the warnings are lost because errors handling is done by catching exceptions

## 1 MissingPaddingCharacters (0x01)

Missing base64 padding characters  
Warning indicating base64 string might be truncated  
For example `QQ` or `QQ=` (instead of `QQ==`) decoding result will be `A` with a warning `MissingPaddingCharacters`  
For example `QUI` (instead of `QQI=`) decoding result will be `AB` with a warning `MissingPaddingCharacters`

## 2 InvalidPaddingBits (0x02)

Invalid padding bits  
Padding bits are not equal to 0  
Warning indicating base64 string might be truncated  
For example `QR==` (instead of `QQ==`) decoding result will be `A` with a warning `InvalidPaddingBits`  
For example `QUJ=` (instead of `QUI=`) decoding result will be `AB` with a warning `InvalidPaddingBits`

## 3 MissingPaddingCharactersAndInvalidPaddingBits (0x03)

Aggregation of MissingPadding and InvalidPaddingBits warnings  
cf. MissingPaddingCharacters  
cf. InvalidPaddingBits
For example `QR` or `QR=` (instead of `QQ==`) decoding result will be `A` with a warning `MissingPaddingCharactersAndInvalidPaddingBits`  
For example `QUJ` (instead of `QUI=`) decoding result will be `AB` with a warning `MissingPaddingCharactersAndInvalidPaddingBits`

# Error codes

## 32 InvalidDestinationBufferSize (0x20)

Destination buffer size too small  
Allocate greater destination buffer

## 96 InvalidSourceBufferSize (0x60)

Invalid source buffer size  
Base64 encoded data size must be a multiple of 4

## 97 InvalidCharacter (0x61)

Invalid base64 character  
Source data is not a valid base64 encoded string
