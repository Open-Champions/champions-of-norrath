// Host check only; not a build input. Build with the host C++ compiler. Identity is stubbed to
// write an identity matrix and the test checks the call happens first and that only floats 0, 5
// and 10 change, to the scale value (NaN and infinity bit patterns included).
#include "scale.cpp"
#include <cstdio>
#include <cstring>

static int identity_calls;
extern "C" void Identity__10VIMatrix44(void* self) {
    ++identity_calls;
    float* m = static_cast<float*>(self);
    for (int i = 0; i < 16; ++i) m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
}

int main() {
    bool ok = true;
    const unsigned patterns[] = {0x3f800000u, 0x40490fdbu, 0xbf000000u, 0x7fc00001u, 0x7f800000u, 0x00000000u, 0x80000000u};
    for (unsigned bits : patterns) {
        float scale;
        std::memcpy(&scale, &bits, 4);
        alignas(16) float m[16];
        for (float& f : m) f = 9.0f;
        identity_calls = 0;
        Scale__10VIMatrix44f(m, scale);
        float want[16];
        for (int i = 0; i < 16; ++i) want[i] = (i % 5 == 0) ? 1.0f : 0.0f;
        want[0] = want[5] = want[10] = scale;
        ok = ok && identity_calls == 1 && std::memcmp(want, m, sizeof m) == 0;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
