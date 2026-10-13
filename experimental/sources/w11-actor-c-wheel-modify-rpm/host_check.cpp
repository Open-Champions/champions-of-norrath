// Host contract for ModifyRPM__5Wheelf; not an EE input. Includes the source.
#include "modify_rpm.cpp"
#include <cstring>
#include <initializer_list>
int main() {
    for (float v : {0.0f, -0.0f, 1.5f, -123.25f, 1e30f, 3.0e-39f}) {
        unsigned char wheel[512];
        std::memset(wheel, 0x5a, sizeof wheel);
        unsigned char before[512];
        std::memcpy(before, wheel, sizeof wheel);
        ModifyRPM__5Wheelf(wheel, v);
        std::memcpy(before + 364, &v, 4);
        if (std::memcmp(before, wheel, sizeof wheel) != 0) return 1;
    }
    return 0;
}
