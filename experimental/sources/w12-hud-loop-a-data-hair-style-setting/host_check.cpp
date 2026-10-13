// Host check for the hairStyleSetting object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "data_hair_style_setting.cpp"

#include <cstddef>

int main() {
    if (sizeof(hairStyleSetting) != 16) return 1;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&hairStyleSetting);
    for (std::size_t i = 0; i < sizeof(hairStyleSetting); ++i)
        if (bytes[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&hairStyleSetting) % alignof(int) != 0) return 3;
    return 0;
}
