// Host contract for the ParticleDef constructor. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" void __11ParticleDefiiiiiiiifffiiiPFPv_v(void*, int, int, int, int, int, int, int, int, float, float, float,
                                                     int, int, int, void (*)(void*));
static int calls = 0;
static void* def_seen = nullptr;
static int ints[11];
static float floats[3];
static void (*cb_seen)(void*) = nullptr;
static unsigned char word4_at_call[4], word8_at_call[4];
extern "C" void setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(void* d, int a, int b, int c, int e, int f, int g, int h,
                                                                     int i, float p, float q, float r, int m, int t, int n,
                                                                     void (*cb)(void*)) {
    ++calls;
    def_seen = d;
    const int v[11] = {a, b, c, e, f, g, h, i, m, t, n};
    std::memcpy(ints, v, sizeof v);
    floats[0] = p; floats[1] = q; floats[2] = r;
    cb_seen = cb;
    std::memcpy(word4_at_call, static_cast<unsigned char*>(d) + 4, 4);
    std::memcpy(word8_at_call, static_cast<unsigned char*>(d) + 8, 4);
}
static void callback(void*) {}

int main() {
    alignas(16) unsigned char d[96];
    std::memset(d, 0x33, sizeof d);
    __11ParticleDefiiiiiiiifffiiiPFPv_v(d, 1, 2, 3, 4, 5, 6, 7, 8, 1.5f, 2.5f, 3.5f, 9, 10, 11, callback);
    int bad = 0;
    auto expect = [&](bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); bad = 1; } };
    expect(calls == 1 && def_seen == d, "one call with the same object");
    for (int i = 0; i < 11; ++i) expect(ints[i] == i + 1, "integer arguments pass through in order");
    expect(floats[0] == 1.5f && floats[1] == 2.5f && floats[2] == 3.5f && cb_seen == callback, "floats and callback pass through");
    expect(word4_at_call[0] == 0xff && word4_at_call[3] == 0xff, "word at +4 is all ones before the call");
    expect(word8_at_call[0] == 0 && word8_at_call[3] == 0, "word at +8 is zero before the call");
    expect(d[3] == 0x33 && d[12] == 0x33, "neighbouring bytes untouched");
    if (bad) return 1;
    std::printf("particle-def-ctor contract passed\n");
    return 0;
}
