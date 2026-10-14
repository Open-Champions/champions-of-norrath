// Host check only; not a build input.
#include "tex_normalized_height.cpp"
#include <cstdio>
int main() {
    alignas(4) unsigned char t[8] = {0x11, 0x22, 0x34, 0x12, 0x55, 0x66, 0x77, 0x88};
    bool ok = texNormalizedHeight__FP8_texture(t) == 0x1234;
    t[2] = 0x00; t[3] = 0x80;
    ok = ok && texNormalizedHeight__FP8_texture(t) == -32768;
    t[2] = 0xff; t[3] = 0xff;
    ok = ok && texNormalizedHeight__FP8_texture(t) == -1;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
