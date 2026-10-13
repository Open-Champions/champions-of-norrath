// Host contract for lightSetColorScale__Ffff. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" {
unsigned char charAmbient[8], charDirect[8], charDirect2[8];
float charScaleR, charScaleG, charScaleB;
void lightSetColorScale__Ffff(float, float, float);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    short* a = reinterpret_cast<short*>(charAmbient);
    short* d = reinterpret_cast<short*>(charDirect);
    short* e = reinterpret_cast<short*>(charDirect2);
    short av[4] = {100, 200, -300, 7}, dv[4] = {10, -21, 33, 8}, ev[4] = {1000, 2000, 3000, 9};
    std::memcpy(a, av, 8); std::memcpy(d, dv, 8); std::memcpy(e, ev, 8);
    lightSetColorScale__Ffff(0.5f, 1.5f, 0.25f);
    expect(charScaleR == 0.5f && charScaleG == 1.5f && charScaleB == 0.25f, "scales stored");
    expect(a[0] == 50 && a[1] == 300 && a[2] == -75 && a[3] == 7, "ambient scaled, fourth untouched");
    expect(d[0] == 5 && d[1] == -31 && d[2] == 8 && d[3] == 8, "direct truncates toward zero");
    expect(e[0] == 500 && e[1] == 3000 && e[2] == 750 && e[3] == 9, "direct2 scaled");
    if (failures) return 1;
    std::printf("lightSetColorScale contract passed\n");
    return 0;
}
