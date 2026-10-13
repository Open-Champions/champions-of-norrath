// Host check for the arrowLeft object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "data_arrow_left.cpp"

#include <cstddef>

int main() {
    if (sizeof(arrowLeft) != 4) return 1;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&arrowLeft);
    for (std::size_t i = 0; i < sizeof(arrowLeft); ++i)
        if (bytes[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&arrowLeft) % alignof(int) != 0) return 3;
    return 0;
}
