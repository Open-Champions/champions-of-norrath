// Host check for the beardSetting object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "data_beard_setting.cpp"

#include <cstddef>

int main() {
    if (sizeof(beardSetting) != 16) return 1;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&beardSetting);
    for (std::size_t i = 0; i < sizeof(beardSetting); ++i)
        if (bytes[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&beardSetting) % alignof(int) != 0) return 3;
    return 0;
}
