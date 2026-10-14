#include "water_bump_speed.hpp"
#include <cstdio>
int main() {
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&worldWaterBumpSpeed);
    if (worldWaterBumpSpeed != 0) return 1;
    for (unsigned i = 0; i != 4; ++i) {
        if (bytes[i] != 0) return 2;
    }
    worldWaterBumpSpeed = 1;
    if (worldWaterBumpSpeed != 1) return 3;
    if (bytes[0] != 1 || bytes[1] != 0 || bytes[2] != 0 || bytes[3] != 0) return 4;
    worldWaterBumpSpeed = 0;
    if (worldWaterBumpSpeed != 0) return 5;
    std::puts("Record the water bump-speed word.");
    return 0;
}
