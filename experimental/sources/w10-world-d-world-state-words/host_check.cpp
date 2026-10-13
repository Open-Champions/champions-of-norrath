// Host contract for the world words and tables. Not an EE plan input.
#include <cstdio>
extern "C" {
extern unsigned char worldLastDrawSucceded;
extern int worldWaterRainTimer;
extern int worldActivityLevel[8];
extern unsigned char worldAnimation[64 * 48];
extern float worldBoundingBox[6];
}
int main() {
    if (worldLastDrawSucceded != 1 || worldWaterRainTimer != 0) return 1;
    for (int i = 0; i != 8; ++i) if (worldActivityLevel[i] != 0) return 2;
    for (unsigned i = 0; i != sizeof worldAnimation; ++i) if (worldAnimation[i] != 0) return 3;
    for (int i = 0; i != 6; ++i) if (worldBoundingBox[i] != 0.0f) return 4;
    if (sizeof worldActivityLevel != 32 || sizeof worldBoundingBox != 24) return 5;
    worldLastDrawSucceded = 0; worldWaterRainTimer = 9;
    if (worldLastDrawSucceded != 0 || worldWaterRainTimer != 9) return 6;
    std::printf("world state words contract passed\n");
    return 0;
}
