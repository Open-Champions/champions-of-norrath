// Host check only; not an EE build input. Callees are recorders over a byte
// array standing in for the object. The stores and call order were also compared
// with the original in a private interpreter run (5 argument sets, identical call
// logs including the light contents at the lightAdd call, identical changed bytes).
#include "sparks.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$6Sparks[4];
unsigned frameNm = 1000;
float norrath_w9_startup_crt_b_view_4816dc = 0.25f;
char norrath_w9_startup_crt_b_view_485380[8];
char norrath_w9_startup_crt_b_view_4708d0[8];
static unsigned char object[256];
static std::string calls;
static unsigned char light_at_add[48];
static int header_marker;

extern "C" {
void __10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap(void* self, Point3 p, void* owner, void* map) {
    calls += (self == object && p.x == 1.0f && p.y == 2.0f && p.z == 3.0f && owner == &header_marker && map == nullptr)
                 ? "proj;" : "proj?;";
}
void lightAdd__FP5Light(void* light) {
    calls += light == object + 100 ? "light;" : "light?;";
    std::memcpy(light_at_add, light, sizeof light_at_add);
}
unsigned objectAddToRunList__FP10GameObject(void* o) { calls += o == object ? "run;" : "run?;"; return 0; }
void* lumpFindResource__FPcT0(char* a, char* b) {
    calls += (a == norrath_w9_startup_crt_b_view_485380 && b == norrath_w9_startup_crt_b_view_4708d0) ? "lump;" : "lump?;";
    return &header_marker;
}
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    calls += (h == &header_marker && o == object && v == 1.0f) ? "sound;" : "sound?;";
}
}

static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
static unsigned word(const unsigned char* base, int off) { unsigned v; std::memcpy(&v, base + off, 4); return v; }
static unsigned half(const unsigned char* base, int off) { unsigned short v; std::memcpy(&v, base + off, 2); return v; }

int main() {
    std::memset(object, 0x5a, sizeof object);
    std::memset(light_at_add, 0x5a, sizeof light_at_add);
    int sound_a, sound_b;
    __6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii(object, Point3{1.0f, 2.0f, 3.0f}, Point3{4.0f, 5.0f, 6.0f},
                                                     &header_marker, &sound_a, &sound_b, 11, 22, 33, 44);
    expect(calls == "proj;light;run;lump;sound;", "call order and arguments");
    const float tint = 0.25f;
    unsigned tint_bits;
    std::memcpy(&tint_bits, &tint, 4);
    // The light as it was when lightAdd ran.
    expect(light_at_add[8] == 0 && light_at_add[9] == 0, "light bytes 8 and 9");
    expect(half(light_at_add, 10) == 318 && half(light_at_add, 12) == 193 && half(light_at_add, 16) == 0xffe0u, "light halves 10, 12, 16");
    expect(half(light_at_add, 14) == 0 && half(light_at_add, 22) == 0, "light halves 14 and 22");
    expect(half(light_at_add, 18) == 318 && half(light_at_add, 20) == 193 && half(light_at_add, 24) == 0xffe0u, "light halves 18, 20, 24");
    expect(word(light_at_add, 28) == 0x42f00000u && word(light_at_add, 36) == 0x42f00000u, "light 120.0f words");
    expect(word(light_at_add, 32) == tint_bits && word(light_at_add, 40) == tint_bits && word(light_at_add, 44) == 0, "light tint and zero");
    expect(word(object, 72) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$6Sparks)), "vtable word");
    expect(word(object, 148) == 0x3f800000u && word(object, 152) == 0x40000000u && word(object, 156) == 0x40400000u, "position copy at 148");
    expect(word(object, 88) == 0x40800000u && word(object, 92) == 0x40a00000u && word(object, 96) == 0x40c00000u, "second value at 88");
    expect(word(object, 176) == 33u && word(object, 180) == 44u, "third and fourth integers");
    expect(word(object, 84) == 1600u, "frameNm + 600");
    expect(word(object, 168) == 22u && word(object, 172) == 11u, "second and first integers");
    expect(word(object, 160) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(&sound_b)), "header pointer at 160");
    expect(object[0] == 0x5a && object[200] == 0x5a && object[164] == 0x5a, "other bytes untouched");
    std::printf("%s\n", failures ? "FAIL" : "ok");
    return failures != 0;
}
