// Host contract for P_DrawParticles__FP8Matrix34. Not an EE plan input. Build with -DNORRATH_HOST_CONTRACT.
// Display-list addresses are 32-bit words, so the list lives in a static arena mapped by the host pointer hook.
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" {
signed char objectDrawEnable = 1;
unsigned char pushLevel[4] = {0};
unsigned frameNm = 2;
unsigned glistp = 0, gstart = 0, GLISTSIZE = 100000, nmSingle = 7;
unsigned char descListMark[128];
unsigned char descList[2048];
unsigned char PS[16 * 312];
unsigned char globalSystem[256];
unsigned char particletex[16];
unsigned short P_FrameNum = 5;
float worldScrz = 1024.0f;
void P_DrawParticles__FP8Matrix34(void* camera);
}
alignas(64) static unsigned char arena[0x4000];
extern "C" void* norrath_host_pointer(unsigned address) {
    const unsigned long base = reinterpret_cast<unsigned long>(arena);
    return reinterpret_cast<void*>((base & ~0xffffffffUL) | address);
}
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); }
static unsigned word(unsigned a) { unsigned v; std::memcpy(&v, norrath_host_pointer(a), 4); return v; }
static unsigned long long dword(unsigned a) { return word(a) | (static_cast<unsigned long long>(word(a + 4)) << 32); }
static void set_word(void* base, unsigned offset, unsigned v) { std::memcpy(static_cast<unsigned char*>(base) + offset, &v, 4); }
static unsigned rword(const unsigned char* base, unsigned offset) { unsigned v; std::memcpy(&v, base + offset, 4); return v; }

static std::vector<const char*> calls;
static int paused = 0;
static int ps_calls = 0, mat44_calls = 0, mat34_calls = 0, add_calls = 0;
static unsigned add_list = 0;
static int add_kind = -1;
static void* add_texture = nullptr;
static float view_args[7];
static unsigned packet_cursor_at_ps[32], packet_tag_at_ps[32];
extern "C" {
void drawTrails__Fv() { calls.push_back("trails"); }
int gameIsPaused__Fv() { return paused; }
void drawSingleParticles__Fi(int pass) { calls.push_back(pass ? "single1" : "single0"); }
void matMakeViewMatrix__FP8Matrix44fffffff(float* m, float a, float b, float c, float d, float e, float f, float g) {
    calls.push_back("view");
    const float args[7] = {a, b, c, d, e, f, g};
    std::memcpy(view_args, args, sizeof args);
    for (int i = 0; i < 16; ++i) m[i] = 1.0f + i;
}
void matMul__FP8Matrix44P8Matrix34T0(float* out, const void*, const float* view) {
    ++mat44_calls;
    for (int i = 0; i < 16; ++i) out[i] = 100.0f * mat44_calls + i;
    (void)view;
}
void matMul__FP8Matrix34N20(float* out, const void*, const void*) {
    ++mat34_calls;
    for (int i = 0; i < 12; ++i) out[i] = 0.0f;
}
void drawPS__FR6PacketP14ParticleSystem(void* packet, void* system) {
    unsigned* p = static_cast<unsigned*>(packet);
    if (ps_calls < 32) { packet_cursor_at_ps[ps_calls] = p[0]; packet_tag_at_ps[ps_calls] = p[1]; }
    ++ps_calls;
    calls.push_back(system == globalSystem ? "ps_global" : "ps_record");
    p[0] += 32;      // a particle system adds two qwords
}
void* norrath_player_resource_map_get(void* map) { return map == particletex ? static_cast<void*>(&nmSingle) : nullptr; }
void* drawAddToList__FPviP8_textureP11_drawRecordi(void* list, int kind, void* texture, void*, int) {
    ++add_calls; add_list = low(list); add_kind = kind; add_texture = texture; return nullptr;
}
void drawLines__Fv() { calls.push_back("lines"); }
void drawDistortionParticles__Fv() { calls.push_back("distortion"); }
}
static int failures = 0;
static void expect(bool ok, const char* what) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; }
}
static void reset() {
    calls.clear(); ps_calls = mat44_calls = mat34_calls = add_calls = 0;
    std::memset(arena, 0xcc, sizeof arena);
}

int main() {
    alignas(16) unsigned char camera[48] = {0};
    // Disabled: nothing happens.
    objectDrawEnable = 0;
    glistp = low(arena) + 0x100; gstart = low(arena);
    reset();
    P_DrawParticles__FP8Matrix34(camera);
    expect(calls.empty() && glistp == low(arena) + 0x100, "disabled returns at once");

    // Enabled, even frame. Records: 0 kind 1 (scrolls on an even frame), 1 kind 4 with a wrapping word,
    // 2 kind 3 (never scrolls), 3 has the unused mark.
    objectDrawEnable = 1; frameNm = 2; paused = 0;
    std::memset(descListMark, 0xff, sizeof descListMark);
    std::memset(descList, 0, sizeof descList);
    for (int i = 0; i < 3; ++i) set_word(descListMark, 4 * i, 1u);
    set_word(descList, 0 * 64 + 4, 1u << 9); set_word(descList, 0 * 64 + 0, 1000u); set_word(descList, 0 * 64 + 16, 2000u);
    set_word(descList, 1 * 64 + 4, 4u << 9); set_word(descList, 1 * 64 + 0, 7700u); set_word(descList, 1 * 64 + 16, 7800u);
    set_word(descList, 2 * 64 + 4, 3u << 9); set_word(descList, 2 * 64 + 0, 5u); set_word(descList, 2 * 64 + 16, 6u);
    set_word(descList, 3 * 64 + 4, 4u << 9); set_word(descList, 3 * 64 + 0, 5u);
    std::memset(PS, 0, sizeof PS);
    set_word(PS + 2 * 312, 304, 1u); set_word(PS + 9 * 312, 304, 3u);
    const unsigned g = low(arena) + 0x400;     // 16-byte aligned display-list start
    gstart = low(arena); glistp = g; GLISTSIZE = 100000; nmSingle = 7;
    reset();
    P_DrawParticles__FP8Matrix34(camera);

    expect(rword(descList, 0) == 1512u && rword(descList, 16) == 2512u, "kind 1 scrolls by 512 on an even frame");
    expect(rword(descList, 64) == 7700u - 7680u && rword(descList, 80) == 7800u - 7680u, "a word reaching 8192 is replaced by its old value minus 7680");
    expect(rword(descList, 128) == 5u && rword(descList, 144) == 6u, "kind 3 stays");
    expect(rword(descList, 192) == 5u, "unused mark stays");
    const char* want[] = {"trails", "single1", "view", "ps_global", "ps_record", "ps_record", "single0", "lines", "distortion"};
    expect(calls.size() == 9, "call count");
    for (size_t i = 0; i < calls.size() && i < 9; ++i) expect(std::strcmp(calls[i], want[i]) == 0, "call order");
    expect(view_args[2] == 1024.0f && view_args[3] == 1.0f && view_args[5] == 18.0f && view_args[6] == 1800.0f, "view arguments");
    expect(mat44_calls == 1 + 2 + 1 && mat34_calls == 2, "matrix calls");
    expect(word(g + 8) == 0x11000000u, "first VIF word");
    expect(word(g + 12) == 0x50000006u, "DIRECT word");
    const unsigned q = g + 16;
    expect(dword(q) == 0x1000000000008005ull && dword(q + 8) == 14u, "GIF tag with NLOOP 5");
    expect(dword(q + 24) == 63u && dword(q + 40) == 6u && dword(q + 56) == 20u && dword(q + 72) == 71u && dword(q + 88) == 68u, "GIF data words");
    expect(dword(q + 64) == 0x71001ull && dword(q + 80) == 0x1212303012123030ull, "GIF register words");
    expect(word(q + 96) == 0x13000000u && word(q + 100) == 0x6c8ac000u, "GIF closing words");
    // First camera x view result is the first matMul call: 100 + i.
    for (int i = 0; i < 16; ++i) {
        float f; std::memcpy(&f, norrath_host_pointer(q + 104 + 4 * i), 4);
        expect(f == 100.0f + i, "camera x view floats");
    }
    expect(word(q + 168) == 32u && word(q + 172) == 0u && word(q + 176) == 0u, "integer 32 then zeros");
    float frame; std::memcpy(&frame, norrath_host_pointer(q + 180), 4);
    expect(frame == 5.0f && word(q + 184) == 0x44480000u, "frame number as float, then 800.0");
    expect(dword(q + 256) == ((static_cast<unsigned long long>(low(descList)) << 32) | 0x30000080ull), "REF tag to descList");
    expect(word(q + 280) == 0x14000014u && word(q + 284) == 0x10000000u, "MSCAL words");
    // Tag of the first segment: ((end - start) / 8 - 2) / 2 with end = q + 256.
    expect(dword(g) == (0x10000000ull | (((q + 256 - g) / 8 - 2) / 2)), "first segment tag");
    expect(ps_calls == 3, "three drawPS calls");
    expect(packet_cursor_at_ps[0] == q + 288, "cursor handed to the first drawPS");
    expect(add_calls == 1 && add_list == g && add_kind == 4 && add_texture == static_cast<void*>(&nmSingle), "registered at the old glistp");
    expect(glistp > g && (glistp & 15u) == 0u, "glistp advanced to an aligned cursor");
    expect(nmSingle == 0u, "nmSingle cleared");
    // The last tag is RET (ID 6).
    bool found_ret = false;
    for (unsigned a = g; a < glistp; a += 16) if ((dword(a) >> 28 & 7u) == 6u && (dword(a) & 0x60000000ull)) found_ret = true;
    expect(found_ret, "a RET tag closes the list");

    // Not enough room before the closing segment: returns with glistp unchanged.
    objectDrawEnable = 1;
    glistp = g; GLISTSIZE = (g - low(arena)) / 8 + 2999;
    reset();
    P_DrawParticles__FP8Matrix34(camera);
    expect(glistp == g && add_calls == 0, "low room before the closing segment returns early");
    // Room for the closing segment but not for the extras after drawAddToList.
    glistp = g; GLISTSIZE = (g - low(arena)) / 8 + 3050;
    reset();
    P_DrawParticles__FP8Matrix34(camera);
    expect(add_calls == 1, "closing segment registered");
    expect(nmSingle == 0u, "nmSingle is still cleared on that path");
    bool lines = false;
    for (auto* c : calls) if (std::strcmp(c, "lines") == 0) lines = true;
    expect(!lines, "no drawLines when room ran out after the segment");
    if (failures) return 1;
    std::printf("draw-particles contract passed\n");
    return 0;
}
