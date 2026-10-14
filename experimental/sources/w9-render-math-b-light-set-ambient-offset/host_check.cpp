// Host contract for lightSetAmbientOffset__FPs. Not an EE plan input.
#include <cstdio>
extern "C" {
unsigned char scene[64];
alignas(8) unsigned char charAmbient[8];
void lightSetAmbientOffset__FPs(const short*);
}
int main() {
    int failures = 0;
    scene[48] = 10; scene[49] = 200; scene[50] = 255; scene[51] = 0;
    const short off[4] = {-11, 5000, -32768, 7};
    const short want[4] = {0, 5200, 0, 7};
    for (unsigned char& b : charAmbient) b = 0xee;
    lightSetAmbientOffset__FPs(off);
    const short* out = reinterpret_cast<short*>(charAmbient);
    for (int i = 0; i < 4; ++i)
        if (out[i] != want[i]) { std::fprintf(stderr, "fail: channel %d got %d want %d\n", i, out[i], want[i]); ++failures; }
    const short off2[4] = {-10, 0, -255, -1};
    lightSetAmbientOffset__FPs(off2);
    const short want2[4] = {0, 200, 0, 0};
    for (int i = 0; i < 4; ++i)
        if (out[i] != want2[i]) { std::fprintf(stderr, "fail: second %d got %d\n", i, out[i]); ++failures; }
    if (failures) return 1;
    std::printf("lightSetAmbientOffset contract passed\n");
    return 0;
}
