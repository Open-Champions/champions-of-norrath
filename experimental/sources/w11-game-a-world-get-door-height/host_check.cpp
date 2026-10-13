// Host contract for worldGetDoorHeight; not an EE input.
#include "world_get_door_height.cpp"
#include <cstdio>
#include <cstring>
int main() {
    int bad = 0;
    alignas(8) unsigned char header[128] = {};
    unsigned char doors[28 * 5] = {};
    unsigned char kinds[36 * 6] = {};
    const unsigned char* d = doors; const unsigned char* k = kinds;
    std::memcpy(header + 84, &d, sizeof d); std::memcpy(header + 32, &k, sizeof k);
    const short selectors[5] = {0, 5, 2, -0, 3};
    for (int i = 0; i < 5; ++i) std::memcpy(doors + 28 * i + 24, &selectors[i], 2);
    for (int i = 0; i < 6; ++i) { int v = 1000 + i; std::memcpy(kinds + 36 * i + 28, &v, 4); }
    const int want[5] = {1000, 1005, 1002, 1000, 1003};
    for (int i = 0; i < 5; ++i) if (worldGetDoorHeight__FP12_worldHeaderi(header, i) != want[i]) { std::printf("fail: door %d\n", i); bad = 1; }
    if (worldGetDoorHeight__FP12_worldHeaderi(header, -1) != 0 || worldGetDoorHeight__FP12_worldHeaderi(header, -100) != 0) { std::puts("fail: negative"); bad = 1; }
    if (!bad) std::puts("worldGetDoorHeight ok");
    return bad;
}
