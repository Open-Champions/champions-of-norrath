// Host check only; not a build input.
#include "init.cpp"
#include <cstring>
int main() {
    alignas(4) unsigned char o[128], e[128];
    std::memset(o, 0x5a, sizeof o); std::memset(e, 0x5a, sizeof e);
    if (Init__13VIPointSprite11VIPointType(o, 7) != 0) return 1;
    auto f = [&](int off, float v) { std::memcpy(e + off, &v, 4); };
    const float r = 0.1f;
    f(8, -r); f(12, 0.0f - r); f(16, 0.0f - r); f(20, 0.0f + r); f(24, 0.0f + r); f(28, 0.0f + r);
    f(32, 0); f(36, 0); f(40, 0); f(44, r);
    unsigned t = 7; std::memcpy(e + 64, &t, 4);
    if (std::memcmp(o, e, sizeof o)) return 2;
    unsigned bits; std::memcpy(&bits, o + 44, 4);
    return bits != 0x3dcccccdu;
}
