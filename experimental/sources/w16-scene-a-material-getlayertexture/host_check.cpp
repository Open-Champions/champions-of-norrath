// Host check only; not a build input.
#include "get_layer_texture.cpp"
#include <cstdio>
int main() {
    alignas(8) static unsigned char m[144 * 4];
    for (int i = 0; i < 4; ++i) { *(unsigned*)(m + 144 * i + 48) = 0x500 + i; *(unsigned*)(m + 144 * i + 44) = 9; *(unsigned*)(m + 144 * i + 52) = 9; }
    bool ok = GetLayerTexture__10VIMateriali(m, 0) == 0x500 && GetLayerTexture__10VIMateriali(m, 3) == 0x503;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
