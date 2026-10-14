#include "word.hpp"
#include <cstdio>
int main() {
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&worldWaterBumpAmount);
    static const unsigned char expect[4] = { 0x00, 0x00, 0x24, 0x42 };
    for (unsigned i = 0; i != 4; ++i) {
        if (bytes[i] != expect[i]) return 1;
    }
    worldWaterBumpAmount = static_cast<decltype(worldWaterBumpAmount)>(worldWaterBumpAmount + worldWaterBumpAmount);
    bool changed = false;
    for (unsigned i = 0; i != 4; ++i) changed = changed || bytes[i] != expect[i];
    if (!changed) return 2;
    for (unsigned i = 0; i != 4; ++i) {
        const_cast<unsigned char*>(bytes)[i] = expect[i];
    }
    for (unsigned i = 0; i != 4; ++i) {
        if (bytes[i] != expect[i]) return 3;
    }
    std::puts("Record worldWaterBumpAmount.");
    return 0;
}
