#include "map_data.cpp"

#include <cstdio>

int main() {
    if (sizeof(mapOffset) != 8 || sizeof(mapwidth) != 4 || sizeof(mapheight) != 4 || sizeof(maptiles) != 93312) return 1;
    if (mapOffset[0] != 0.0f || mapOffset[1] != 0.0f || mapwidth != 0 || mapheight != 0) return 2;
    for (unsigned char tile : maptiles) if (tile != 0) return 3;
    if (static_cast<void*>(&mapwidth) == static_cast<void*>(&mapheight)) return 4;
    std::puts("map storage contract passed");
    return 0;
}
