// Host check for the scrollmapTexCopy object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_scrollmap_tex_copy.cpp"

#include <cstddef>

int main() {
    if (sizeof(scrollmapTexCopy) != 128) return 1;
    for (std::size_t i = 0; i < sizeof(scrollmapTexCopy); ++i)
        if (scrollmapTexCopy[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&scrollmapTexCopy) % 8 != 0) return 3;
    return 0;
}
