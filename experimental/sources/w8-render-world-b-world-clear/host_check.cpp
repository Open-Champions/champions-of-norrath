#include <cstring>
extern "C" {
int routeClearStart, rqTail, rqHead, worldWaterRainTimer;
unsigned char routeEnable;
alignas(8) unsigned char worldAnimation[64 * 48 + 16];
alignas(8) unsigned char DynamicLightArray[16 * 68 + 16];
void worldClear__Fv();
}
static int calls;
static void* order[32];
extern "C" void lightDeactivate__FP5Light(void* light) {
    // The record's first words are still intact when the callee runs.
    unsigned char* rec = static_cast<unsigned char*>(light) - 8;
    if (calls < 32) order[calls] = light;
    if (rec[0] != 0xaa) calls += 1000;
    ++calls;
}
int main() {
    routeClearStart = rqTail = rqHead = worldWaterRainTimer = 9;
    routeEnable = 0;
    std::memset(worldAnimation, 0x5a, sizeof worldAnimation);
    std::memset(DynamicLightArray, 0xaa, sizeof DynamicLightArray);
    worldClear__Fv();
    if (routeClearStart || rqTail || rqHead || worldWaterRainTimer) return 1;
    if (routeEnable != 1) return 2;
    for (int i = 0; i < 64; ++i) {
        const unsigned char* r = worldAnimation + i * 48;
        for (int b = 0; b < 48; ++b) {
            const bool cleared = b == 4 || (b >= 40 && b < 44);
            if (r[b] != (cleared ? 0 : 0x5a)) return 3;
        }
    }
    for (int b = 64 * 48; b < int(sizeof worldAnimation); ++b) if (worldAnimation[b] != 0x5a) return 4;
    if (calls != 16) return 5;
    for (int i = 0; i < 16; ++i) {
        if (order[i] != DynamicLightArray + i * 68 + 8) return 6;
        const unsigned char* r = DynamicLightArray + i * 68;
        for (int b = 0; b < 68; ++b) if (r[b] != (b < 8 ? 0 : 0xaa)) return 7;
    }
    for (int b = 16 * 68; b < int(sizeof DynamicLightArray); ++b) if (DynamicLightArray[b] != 0xaa) return 8;
    return 0;
}
