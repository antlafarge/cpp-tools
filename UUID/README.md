Universally unique identifier (UUID)
====================================

## String to UUID

    std::string str = "806e6967-4351-3efa-cc56-4af2be77b154";

    UUID uuid1(uuid);

    UUID uuid2 = UUID::toUUID(str);

## UUID to String

    std::string str1 = uuid.toString();

    std::string str2 = uuid.toString(UUID::Format::Base64);

## Generate an UUID (fully randomized, version 4)

    UUID uuid = UUID::generate();

## Create from 128 bits raw buffer

The first 128 bits of the buffer will be used to create the UUID

    char* buffer; // buffer must be 128 bits or more
    UUID uuid(buffer);
