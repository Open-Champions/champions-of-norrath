// Host contract for setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v. Not an EE plan input.
#include <cmath>
#include <cstdio>
#include <cstring>

extern "C" {
unsigned lifeTimes[4] = {10, 20, 30, 40};
void setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(void*, int, int, int, int, int, int, int, int, float, float,
                                                          float, int, int, int, void (*)(void*));
}
static int assert_calls = 0;
static unsigned assert_line = 0;
static char assert_file[32], assert_text[32];
extern "C" void __assert__FPCcUiT0i(const char* file, unsigned line, const char* text, int) {
    ++assert_calls;
    assert_line = line;
    std::snprintf(assert_file, sizeof assert_file, "%s", file);
    std::snprintf(assert_text, sizeof assert_text, "%s", text);
}
static int failures = 0;
static void expect(bool ok, const char* what) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; }
}
static unsigned word(const unsigned char* d, unsigned o) { unsigned v; std::memcpy(&v, d + o, 4); return v; }
static float flt(const unsigned char* d, unsigned o) { float v; std::memcpy(&v, d + o, 4); return v; }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
static void callback(void*) {}

int main() {
    alignas(16) unsigned char d[96];
    std::memset(d, 0xee, sizeof d);
    setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(d, 1, 2, 3, 4, 11, 22, 33, 44, 2.0f, 3.0f, 512.0f, 1, 0, 0x21, nullptr);
    expect(assert_calls == 0, "no assert for a null callback");
    expect(word(d, 0) == 1u, "tier * 4 + mode");
    expect(word(d, 12) == 0u, "callback word");
    expect(near(flt(d, 24), 2.0f), "r / 256");
    expect(near(flt(d, 28), 5.0f), "(q - p) * 100 / life");
    expect(word(d, 16) == 528u && word(d, 20) == 1040u && word(d, 32) == 1008u && word(d, 36) == 1520u, "grid words");
    expect(word(d, 40) == static_cast<unsigned>(reinterpret_cast<unsigned long>(d)), "self address");
    expect(near(flt(d, 44), 200.0f), "p * 100");
    expect(flt(d, 48) == 8388609.0f && flt(d, 52) == 8388610.0f && flt(d, 56) == 8388611.0f && flt(d, 60) == 8388612.0f, "biased starts");
    expect(near(flt(d, 64), 0.5f) && near(flt(d, 68), 1.0f) && near(flt(d, 72), 1.5f) && near(flt(d, 76), 2.0f), "end deltas / life");
    expect(d[80] == 0xee, "nothing past +76");

    // tier 2 writes +16 as a float and uses p * 100 * 0.01.
    setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(d, 1, 2, 3, 4, 11, 22, 33, 44, 2.0f, 3.0f, 512.0f, 0, 2, 0x21, nullptr);
    expect(near(flt(d, 16), 2.0f) && near(flt(d, 28), 2.0f) && word(d, 0) == 8u, "tier 2");
    // tier 3 keeps the starts exact; clamps: mode 9 -> 3, negative -> 0.
    setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(d, 1, 2, 3, 4, 11, 22, 33, 44, 2.0f, 3.0f, 512.0f, 9, 3, 0, nullptr);
    expect(flt(d, 48) == 1.0f && word(d, 0) == 15u && near(flt(d, 64), 10.0f / 40.0f), "tier 3 and mode clamp");
    setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(d, 1, 2, 3, 4, 11, 22, 33, 44, 2.0f, 3.0f, 512.0f, -5, 9, 0, nullptr);
    expect(word(d, 0) == 12u && flt(d, 48) == 1.0f, "negative mode, large tier");
    // A callback pointer asserts but the body still runs.
    setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(d, 1, 2, 3, 4, 11, 22, 33, 44, 2.0f, 3.0f, 512.0f, 1, 0, 0, callback);
    expect(assert_calls == 1 && assert_line == 502u, "assert line");
    expect(std::strcmp(assert_file, "particle.cc") == 0 && std::strcmp(assert_text, "!terminateCallBackIn") == 0, "assert text");
    expect(word(d, 12) == static_cast<unsigned>(reinterpret_cast<unsigned long>(callback)) && word(d, 0) == 1u, "body ran");
    if (failures) return 1;
    std::printf("set-particle-def contract passed\n");
    return 0;
}
