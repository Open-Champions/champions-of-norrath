// Host contract for the seven data definitions. Not an EE plan input.
#include <cstdio>
extern "C" {
extern short myYaw;
extern int texDecodeStarted;
extern float worldViewMatrix[16];
extern float worldCharViewMatrix[16];
extern int scissored;
extern int vifDirectQWordCount;
extern short yaws[12];
}
int main() {
    if (static_cast<unsigned short>(myYaw) != 0xe000u || texDecodeStarted != 0) return 1;
    if (scissored != 0 || vifDirectQWordCount != 0) return 2;
    for (float f : worldViewMatrix) if (f != 0.0f) return 3;
    for (float f : worldCharViewMatrix) if (f != 0.0f) return 4;
    for (short s : yaws) if (s != 0) return 5;
    if (sizeof worldViewMatrix != 64 || sizeof worldCharViewMatrix != 64 || sizeof yaws != 24) return 6;
    std::printf("view data contract passed\n");
    return 0;
}
