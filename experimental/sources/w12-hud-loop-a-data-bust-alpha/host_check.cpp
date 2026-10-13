// Host check for the bustAlpha object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "data_bust_alpha.cpp"

#include <cstddef>

int main() {
    if (sizeof(bustAlpha) != 4) return 1;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&bustAlpha);
    for (std::size_t i = 0; i < sizeof(bustAlpha); ++i)
        if (bytes[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&bustAlpha) % alignof(int) != 0) return 3;
    return 0;
}
