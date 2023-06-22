#include "optional.polyfill.h"

void testOptional()
{
    std::cout << "optional" << std::endl;

    std::optional<int> opt;
    assert(!opt.has_value());

    opt = 1337;
    assert(opt.has_value());

    assert(*opt == 1337);

    assert(!(std::optional<int>() == std::optional<int>(1337)));
    assert(std::optional<int>(1337) == std::optional<int>(1337));
    assert(!(std::optional<int>(1337) == std::optional<int>(1338)));

    assert(std::optional<int>() != std::optional<int>(1337));
    assert(!(std::optional<int>(1337) != std::optional<int>(1337)));
    assert(std::optional<int>(1337) != std::optional<int>(1338));

    assert(std::optional<int>() < std::optional<int>(1337));
    assert(!(std::optional<int>(1337) < std::optional<int>(1337)));
    assert(std::optional<int>(1337) < std::optional<int>(1338));

    assert(std::optional<int>() <= std::optional<int>(1337));
    assert(std::optional<int>(1337) <= std::optional<int>(1337));
    assert(std::optional<int>(1337) <= std::optional<int>(1338));

    assert(!(std::optional<int>() > std::optional<int>(1337)));
    assert(!(std::optional<int>(1337) > std::optional<int>(1337)));
    assert(!(std::optional<int>(1337) > std::optional<int>(1338)));

    assert(!(std::optional<int>() >= std::optional<int>(1337)));
    assert(std::optional<int>(1337) >= std::optional<int>(1337));
    assert(!(std::optional<int>(1337) >= std::optional<int>(1338)));

    std::cout << "PASSED" << std::endl
              << std::endl;
}

int main()
{
    testOptional();

    return 0;
}
