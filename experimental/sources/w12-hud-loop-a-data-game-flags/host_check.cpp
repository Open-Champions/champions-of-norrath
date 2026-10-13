// Host check for the gameFlags object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "data_game_flags.cpp"

#include <cstddef>

int main() {
    if (sizeof(gameFlags) != 16) return 1;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&gameFlags);
    for (std::size_t i = 0; i < sizeof(gameFlags); ++i)
        if (bytes[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&gameFlags) % alignof(int) != 0) return 3;
    return 0;
}
