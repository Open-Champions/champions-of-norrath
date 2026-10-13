// Host check for the invHeader object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "data_inv_header.cpp"

#include <cstddef>

int main() {
    if (sizeof(invHeader) != 4) return 1;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&invHeader);
    for (std::size_t i = 0; i < sizeof(invHeader); ++i)
        if (bytes[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&invHeader) % alignof(int) != 0) return 3;
    return 0;
}
