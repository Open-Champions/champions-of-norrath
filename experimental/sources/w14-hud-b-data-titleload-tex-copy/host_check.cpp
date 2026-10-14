// Host check for the titleloadTexCopy object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_titleload_tex_copy.cpp"

#include <cstddef>

int main() {
    if (sizeof(titleloadTexCopy) != 64) return 1;
    for (std::size_t i = 0; i < sizeof(titleloadTexCopy); ++i)
        if (titleloadTexCopy[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&titleloadTexCopy) % 8 != 0) return 3;
    return 0;
}
