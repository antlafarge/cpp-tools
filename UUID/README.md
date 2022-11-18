Universally unique identifier (UUID)
====================================

## String to UUID

    UUID uuid("123e4567-e89b-12d3-a456-426614174000");
    
    uuid.fromString("Ej5FZ+ibEtOkVkJmFBdAAA==");

## UUID to String

    std::cout << uuid.toString(); // 123e4567-e89b-12d3-a456-426614174000 (default)
    std::cout << uuid.toString(UUID::Format::N); // 123e4567e89b12d3a456426614174000
    std::cout << uuid.toString(UUID::Format::B); // {123e4567-e89b-12d3-a456-426614174000}
    std::cout << uuid.toString(UUID::Format::P); // (123e4567-e89b-12d3-a456-426614174000)
    std::cout << uuid.toString(UUID::Format::X); // {0x123e4567,0xe89b,0x12d3,{0xa4,0x56,0x42,0x66,0x14,0x17,0x40,0x00}}
    std::cout << uuid.toString(UUID::Format::URN); // urn:uuid:123e4567-e89b-12d3-a456-426614174000
    std::cout << uuid.toString(UUID::Format::Base64); // Ej5FZ+ibEtOkVkJmFBdAAA==

## Generate an UUID (fully randomized, version 4)

    UUID uuid = UUID::generate();
    std::cout << uuid.getVersion(); // 4
    std::cout << uuid.getVariant(); // 1

## Create from 128 bits raw buffer

The first 128 bits of the buffer will be used to create the UUID

    char* buffer; // buffer must be 128 bits or more
    UUID uuid(buffer);
