// Host contract for P_SetTrailColor__Fiiiii. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" {
unsigned char trail[1040 * 256];
void P_SetTrailColor__Fiiiii(int, int, int, int, int);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    *reinterpret_cast<short*>(trail + 6 * 1040 + 1024) = 6;
    P_SetTrailColor__Fiiiii(6, 0x111, 22, 33, 0x1ff);
    unsigned char* r = trail + 6 * 1040;
    expect(r[1034] == 0x11 && r[1035] == 22 && r[1036] == 33 && r[1037] == 0xff, "low bytes stored in order");
    unsigned char before[sizeof trail];
    std::memcpy(before, trail, sizeof trail);
    P_SetTrailColor__Fiiiii(0, 1, 2, 3, 4);
    P_SetTrailColor__Fiiiii(7, 1, 2, 3, 4);
    P_SetTrailColor__Fiiiii(6 + 256, 1, 2, 3, 4);
    expect(std::memcmp(before, trail, sizeof trail) == 0, "zero, empty or mismatched id changes nothing");
    if (failures) return 1;
    std::printf("P_SetTrailColor contract passed\n");
    return 0;
}
