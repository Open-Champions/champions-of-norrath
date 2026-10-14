// Host check only; not a build input. Build with the host C++ compiler. sinf, cosf and Identity are
// stubbed; the test checks the call order (sin, cos, identity), the angle passed to both, that
// exactly floats 0, 5, 1 and 4 change and their values.
#include <cstdio>
#include <cstring>
#include <string>
#define sinf norrath_host_sinf
#define cosf norrath_host_cosf
#include "rotate_z.cpp"
#undef sinf
#undef cosf

static std::string trace;
static float angles[2];
extern "C" float norrath_host_sinf(float a) { trace += "s"; angles[0] = a; return 0.25f + a * 0.001f; }
extern "C" float norrath_host_cosf(float a) { trace += "c"; angles[1] = a; return 0.5f - a * 0.002f; }
extern "C" void Identity__10VIMatrix44(void* self) {
    trace += "i";
    float* m = static_cast<float*>(self);
    for (int i = 0; i < 16; ++i) m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
}

int main() {
    bool ok = true;
    const float values[] = {0.0f, 1.0f, -2.5f, 3.14159f, 100.0f};
    for (float a : values) {
        alignas(16) float m[16];
        for (float& f : m) f = 9.0f;
        trace.clear();
        RotateZ__10VIMatrix44f(m, a);
        float s = 0.25f + a * 0.001f, c = 0.5f - a * 0.002f;
        float want[16];
        for (int i = 0; i < 16; ++i) want[i] = (i % 5 == 0) ? 1.0f : 0.0f;
        want[0] = c; want[5] = c; want[1] = s; want[4] = -s;
        ok = ok && trace == "sci" && angles[0] == a && angles[1] == a && std::memcmp(want, m, sizeof m) == 0;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
