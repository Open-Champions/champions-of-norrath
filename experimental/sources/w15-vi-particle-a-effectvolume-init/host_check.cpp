// Host check only; not a build input.
#include "init.cpp"
#include <cstring>
static int calls; static unsigned char* sv; static unsigned st; static float sf;
extern "C" void SetType__14VIEffectVolume18VIEffectVolumeTypef(void* s, unsigned t, float v) {
    calls++; sv = static_cast<unsigned char*>(s); st = t; sf = v;
    *reinterpret_cast<unsigned*>(sv + 36) = t;   // the real routine stores the type
}
int main() {
    alignas(4) unsigned char o[128], e[128];
    std::memset(o, 0x5a, sizeof o); std::memset(e, 0x5a, sizeof e);
    float box[6] = {1, 2, 3, 4, 5, 6};
    if (Init__14VIEffectVolumeRC6VIBBox(o, box) != 0) return 1;
    if (calls != 1 || sv != o || st != 0 || sf != -1.0f) return 2;
    auto w = [&](int off, unsigned v) { std::memcpy(e + off, &v, 4); };
    auto f = [&](int off, float v) { std::memcpy(e + off, &v, 4); };
    f(8, 1); f(12, 2); f(16, 3); f(20, 4); f(24, 5); f(28, 6);
    w(32, 1); w(36, 0); w(72, 1); f(44, 1.0f); f(48, 20.0f); f(40, 1.0f);
    w(0, 0); w(4, 0); w(52, 0); w(56, 0); w(60, 0);
    return std::memcmp(o, e, sizeof o) != 0;
}
