Universally unique identifier (UUID)
====================================

## String to UUID

    UUID uuid("123e4567-e89b-12d3-a456-426614174000"); // Standard UUID format
    
    uuid.fromString("Ej5FZ+ibEtOkVkJmFBdAAA=="); // Base64

## UUID to String

    std::cout << uuid.toString();                     // 123e4567-e89b-12d3-a456-426614174000 (default)
    std::cout << uuid.toString(UUID::Format::D);      // 123e4567-e89b-12d3-a456-426614174000 (default)
    std::cout << uuid.toString(UUID::Format::N);      // 123e4567e89b12d3a456426614174000
    std::cout << uuid.toString(UUID::Format::B);      // {123e4567-e89b-12d3-a456-426614174000}
    std::cout << uuid.toString(UUID::Format::P);      // (123e4567-e89b-12d3-a456-426614174000)
    std::cout << uuid.toString(UUID::Format::X);      // {0x123e4567,0xe89b,0x12d3,{0xa4,0x56,0x42,0x66,0x14,0x17,0x40,0x00}}
    std::cout << uuid.toString(UUID::Format::URN);    // urn:uuid:123e4567-e89b-12d3-a456-426614174000
    std::cout << uuid.toString(UUID::Format::Base64); // Ej5FZ+ibEtOkVkJmFBdAAA==

## Generate an UUID (variant 1, version 4 (fully randomized))

    UUID uuid = UUID::generate(); // 1f979ca4-ad29-40ef-8352-f2082cba44d4
    std::cout << uuid.getVersion(); // 4
    std::cout << uuid.getVariant(); // 1

## Create from 128 bits raw buffer

The first 128 bits of the buffer will be used to create the UUID

    // Variant 1
    const uint8_t buffer[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    UUID uuid(buffer);
    std::cout << uuid.toString(); // "00112233-4455-6677-8899-aabbccddeeff"

    // Variant 2
    const uint8_t buffer[16] = { 0x33, 0x22, 0x11, 0x00, 0x55, 0x44, 0x77, 0x66, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    UUID uuid(buffer, true);
    std::cout << uuid.toString(); // "00112233-4455-6677-8899-aabbccddeeff"

*Note: `const char*` will be processed as c-string, and `const uint8_t*` will be processed as raw buffer input*
