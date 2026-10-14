// Host check only; not a build input.
#include "tex_normalized_width.cpp"
#include <cstdio>
int main() {
    alignas(4) unsigned char t[8] = {0x34, 0x12, 0x22, 0x11, 0x55, 0x66, 0x77, 0x88};
    bool ok = texNormalizedWidth__FP8_texture(t) == 0x1234;
    t[0] = 0x00; t[1] = 0x80;
    ok = ok && texNormalizedWidth__FP8_texture(t) == -32768;
    t[0] = 0xff; t[1] = 0xff;
    ok = ok && texNormalizedWidth__FP8_texture(t) == -1;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
