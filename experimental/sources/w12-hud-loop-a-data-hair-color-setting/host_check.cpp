// Host check for the hairColorSetting object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "data_hair_color_setting.cpp"

#include <cstddef>

int main() {
    if (sizeof(hairColorSetting) != 16) return 1;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&hairColorSetting);
    for (std::size_t i = 0; i < sizeof(hairColorSetting); ++i)
        if (bytes[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&hairColorSetting) % alignof(int) != 0) return 3;
    return 0;
}
