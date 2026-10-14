// Host contract for lightSetAmbientOnly__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" {
int ambientOnly;
alignas(8) unsigned char charAmbient[8];
alignas(8) unsigned char charDirect[8];
alignas(8) unsigned char charDirect2[8];
void lightSetAmbientOnly__Fv();
}
int main() {
    int failures = 0;
    std::memset(charAmbient, 0x5a, 8); std::memset(charDirect, 0x5a, 8); std::memset(charDirect2, 0x5a, 8);
    ambientOnly = 0;
    lightSetAmbientOnly__Fv();
    if (ambientOnly != 1) { std::fprintf(stderr, "fail: flag\n"); ++failures; }
    for (int i = 0; i < 8; ++i) {
        if (charDirect[i] != 0 || charDirect2[i] != 0) { std::fprintf(stderr, "fail: not cleared at %d\n", i); ++failures; }
        if (charAmbient[i] != 0x5a) { std::fprintf(stderr, "fail: ambient touched at %d\n", i); ++failures; }
    }
    if (failures) return 1;
    std::printf("lightSetAmbientOnly contract passed\n");
    return 0;
}
