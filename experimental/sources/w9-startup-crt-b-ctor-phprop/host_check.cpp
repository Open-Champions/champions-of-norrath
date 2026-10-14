// Host check only; not an EE build input. Callees are recorders over a byte array
// standing in for the object. The same stores and calls were compared with the
// original in a private interpreter run (48 argument sets covering description
// present/absent, four values left in byte 568 by initStuff, and six flag values;
// identical call logs and changed bytes).
#include "phprop.cpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$6PhProp[4];
char norrath_w9_startup_crt_b_view_462260[4];
char norrath_w9_startup_crt_b_view_483b70[4];
static unsigned char object[640];
static std::string calls;
static int init_sets_568;
static int sprintf_arg;
static int header_marker;

extern "C" {
void __4PropG6Point3PcN22li(void* self, Point3 p, char* a, char* b, char* c, long n) {
    char text[96];
    std::snprintf(text, sizeof text, "prop(%d,%g,%g,%g,%d,%d,%d,%ld);", self == object, p.x, p.y, p.z, a != nullptr, b != nullptr && c != nullptr, 0, n);
    calls += text;
}
void initStuff__6PhPropi(void* self, int flags) {
    calls += "init(" + std::to_string(flags) + ");";
    if (init_sets_568 >= 0) object[568] = static_cast<unsigned char>(init_sets_568);
    (void)self;
}
int sprintf(char* buffer, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    sprintf_arg = va_arg(ap, int);
    va_end(ap);
    calls += format == norrath_w9_startup_crt_b_view_462260 ? "fmt;" : "fmt?;";
    std::strcpy(buffer, "buf");
    return 3;
}
void* lumpFindResource__FPcT0(char* lump, char* name) {
    calls += (lump == norrath_w9_startup_crt_b_view_483b70 && std::strcmp(name, "buf") == 0) ? "lump;" : "lump?;";
    return &header_marker;
}
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    calls += (h == &header_marker && o == object && v == 1.0f) ? "sound;" : "sound?;";
}
}

static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
static unsigned word(int off) { unsigned v; std::memcpy(&v, object + off, 4); return v; }
static unsigned half(int off) { unsigned short v; std::memcpy(&v, object + off, 2); return v; }
static void reset() {
    std::memset(object, 0x5a, sizeof object);
    calls.clear();
}

int main() {
    unsigned char description[72];
    for (int i = 0; i < 72; ++i) description[i] = static_cast<unsigned char>(i + 1);

    // No description, nothing sets byte 568: no sound; the fixed stores are made.
    reset();
    init_sets_568 = -1;
    object[568] = 0;
    __6PhPropG6Point3PcN22liPQ24item11descriptionf(object, Point3{1, 2, 3}, (char*)1, (char*)2, (char*)3, 7, 0x19, nullptr, 2.5f);
    expect(calls == "prop(1,1,2,3,1,1,0,7);init(25);", "no sound without byte 568");
    expect(word(72) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$6PhProp)), "vtable word");
    expect(half(388) == 0xffffu && half(380) == 0xffffu && half(372) == 0xffffu && half(364) == 0xffffu, "four halves of 0xffff");
    expect(object[360] == 0xff && object[361] == 0xff, "bytes 360 and 361");
    bool zeros = true;
    for (int i = 344; i < 360; ++i) zeros = zeros && object[i] == 0;
    expect(zeros, "eight zero halves");
    expect(word(576) == 0x40200000u, "scale at 576");
    expect(word(12) == (0x5a5a5a5au | 0x4000u), "flag word with 0x4000");
    expect(object[570] == 0 && word(564) == 0, "byte 570 and word 564");
    expect(object[362] == 0x5a && object[363] == 0x5a && object[300] == 0x5a, "other bytes untouched");

    // Description present: the copy replaces 328..399 and the flag word changes; sound plays.
    reset();
    init_sets_568 = -1;
    object[68] = 1; object[69] = 0; object[70] = 0; object[71] = 0;
    __6PhPropG6Point3PcN22liPQ24item11descriptionf(object, Point3{1, 2, 3}, (char*)1, (char*)2, (char*)3, 7, 0x1, description, 0.0f);
    expect(std::memcmp(object + 328, description, 72) == 0, "description copy");
    expect(object[568] == 1, "byte 568 set");
    expect(word(12) == (((0x5a5a5a5au | 0x4000u) | 0x100000u) & ~0x800u), "flag word with description");
    const unsigned seed = 1u * 0x19660du + 0x3c6ef35fu;
    expect(word(68) == seed, "seed stepped");
    expect(sprintf_arg == static_cast<int>((seed >> 16) % 3u) + 1, "formatted value");
    expect(calls == "prop(1,1,2,3,1,1,0,7);init(1);fmt;lump;sound;", "sound sequence order");

    // Description present but flags select no sound.
    reset();
    __6PhPropG6Point3PcN22liPQ24item11descriptionf(object, Point3{1, 2, 3}, (char*)1, (char*)2, (char*)3, 7, 0x6, description, 0.0f);
    expect(calls == "prop(1,1,2,3,1,1,0,7);init(6);", "flags 0x6 select no sound");

    // initStuff leaves a negative byte in 568: still counts as nonzero.
    reset();
    init_sets_568 = 0x81;
    __6PhPropG6Point3PcN22liPQ24item11descriptionf(object, Point3{1, 2, 3}, (char*)1, (char*)2, (char*)3, 7, 0x8, nullptr, 0.0f);
    expect(calls.find("sound;") != std::string::npos, "byte 568 = 0x81 plays sound");
    std::printf("%s\n", failures ? "FAIL" : "ok");
    return failures != 0;
}
