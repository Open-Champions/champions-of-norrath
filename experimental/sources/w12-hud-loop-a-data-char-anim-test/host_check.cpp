// Host check for the charAnimTest object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "data_char_anim_test.cpp"

#include <cstddef>

int main() {
    if (sizeof(charAnimTest) != 4) return 1;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&charAnimTest);
    for (std::size_t i = 0; i < sizeof(charAnimTest); ++i)
        if (bytes[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&charAnimTest) % alignof(int) != 0) return 3;
    return 0;
}
