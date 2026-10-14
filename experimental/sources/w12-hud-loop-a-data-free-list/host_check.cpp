// Host check for the freeList object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "data_free_list.cpp"

#include <cstddef>

int main() {
    if (sizeof(freeList) != 4) return 1;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&freeList);
    for (std::size_t i = 0; i < sizeof(freeList); ++i)
        if (bytes[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&freeList) % alignof(int) != 0) return 3;
    return 0;
}
