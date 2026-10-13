// Host check for the expandList object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "data_expand_list.cpp"

#include <cstddef>

int main() {
    if (sizeof(expandList) != 320) return 1;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&expandList);
    for (std::size_t i = 0; i < sizeof(expandList); ++i)
        if (bytes[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&expandList) % alignof(int) != 0) return 3;
    return 0;
}
