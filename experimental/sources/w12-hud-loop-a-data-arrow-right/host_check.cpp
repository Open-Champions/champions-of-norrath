// Host check for the arrowRight object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "data_arrow_right.cpp"

#include <cstddef>

int main() {
    if (sizeof(arrowRight) != 4) return 1;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&arrowRight);
    for (std::size_t i = 0; i < sizeof(arrowRight); ++i)
        if (bytes[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&arrowRight) % alignof(int) != 0) return 3;
    return 0;
}
