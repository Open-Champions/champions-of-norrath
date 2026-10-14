// Host contract for lightSetAmbientColor__FPs. Not an EE plan input.
#include <cstdio>
extern "C" {
alignas(8) unsigned char charAmbient[8];
void lightSetAmbientColor__FPs(const unsigned short*);
}
int main() {
    int failures = 0;
    const unsigned short in[4] = {0x8001, 2, 0xffff, 0x1234};
    for (unsigned char& b : charAmbient) b = 0xee;
    lightSetAmbientColor__FPs(in);
    const unsigned short* out = reinterpret_cast<unsigned short*>(charAmbient);
    for (int i = 0; i < 4; ++i)
        if (out[i] != in[i]) { std::fprintf(stderr, "fail: channel %d\n", i); ++failures; }
    if (failures) return 1;
    std::printf("lightSetAmbientColor contract passed\n");
    return 0;
}
