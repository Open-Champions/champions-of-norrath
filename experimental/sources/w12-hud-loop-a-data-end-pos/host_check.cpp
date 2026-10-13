// Host check for the endPos object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "data_end_pos.cpp"

#include <cstddef>

int main() {
    if (sizeof(endPos) != 16) return 1;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&endPos);
    for (std::size_t i = 0; i < sizeof(endPos); ++i)
        if (bytes[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&endPos) % alignof(int) != 0) return 3;
    return 0;
}
