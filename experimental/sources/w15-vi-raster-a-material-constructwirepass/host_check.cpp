// Host check only; not a build input.
#include "construct_wire_pass.cpp"
#include <cstdio>
#include <cstring>
int main() {
    bool ok = true;
    unsigned char material[512];
    unsigned char out[320];
    std::memset(material, 0, sizeof material);
    std::memset(out, 0xaa, sizeof out);
    unsigned char* c = material + 144 + 56;
    c[0] = 255; c[1] = 128; c[2] = 0; c[3] = 51;
    ok = ok && ConstructWirePass__10VIMaterialiR8VIRasterP7VIQWord(material, 1, nullptr, out) == 0;
    unsigned w[4]; std::memcpy(w, out, 16);
    ok = ok && w[0] == 2 && w[1] == 0 && w[2] == 0 && w[3] == 0;
    unsigned long long q[2];
    std::memcpy(q, out + 16, 16); ok = ok && q[0] == 0x1000000000008002ull && q[1] == 14;
    std::memcpy(q, out + 32, 16); ok = ok && q[0] == 0x50000ull && q[1] == 71;
    std::memcpy(q, out + 48, 16); ok = ok && q[0] == 0x8000000064ull && q[1] == 66;
    std::memcpy(q, out + 160, 16); ok = ok && q[0] == 0x2006400000008003ull && q[1] == 65;
    std::memcpy(q, out + 176, 16); ok = ok && q[0] == 0x2006c00000008003ull && q[1] == 65;
    std::memcpy(q, out + 192, 16); ok = ok && q[0] == 0x2005400000008003ull && q[1] == 65;
    float f[4]; std::memcpy(f, out + 208, 16);
    const float k = __builtin_bit_cast(float, 0x3b808080u);
    ok = ok && f[0] == 255.0f * k * 128.0f && f[1] == 128.0f * k * 128.0f && f[2] == 0.0f && f[3] == 51.0f * k * 128.0f;
    for (int i = 64; i < 160; ++i) ok = ok && out[i] == 0xaa;      // quadwords 4..9 untouched
    for (int i = 224; i < 320; ++i) ok = ok && out[i] == 0xaa;     // quadwords 14.. untouched
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
