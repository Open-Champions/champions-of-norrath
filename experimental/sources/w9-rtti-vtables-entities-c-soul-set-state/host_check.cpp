// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "set_state.cpp"
#include "../w9-rtti-vtables-entities-c-bytes/host_vcall.hpp"
#include <cctype>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
char levelName[128];
unsigned char soulParticle[16];
static std::string log_text;
static bool set_minion_in_shadow = false;
static int sp_result = 0, sp_new = -1000, nm_count = 0, slot_arg = -1;
static int rolls[16], roll_n = 0, roll_i = 0;
static void* trans_header = nullptr;
static void* shot_header = nullptr;
static unsigned trans_rate = 0, shot_rate = 0;
static int trans_flags = 0, shot_flags = 0;
static int particles = 0;
static float last_velocity[3];
static float velocities[8][3];
static unsigned char headers[200];
static unsigned char object[1300];
static unsigned char vtable[256];
static unsigned char minion_block[64];
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
extern "C" int strcasecmp(const char* a, const char* b) {
    for (;; ++a, ++b) {
        const int x = std::tolower(static_cast<unsigned char>(*a)), y = std::tolower(static_cast<unsigned char>(*b));
        if (x != y) return x < y ? -1 : 1;
        if (x == 0) return 0;
    }
}
extern "C" int setStateProcessing__8CreaturePi(void*, int* state) { log_text += "p"; if (sp_new != -1000) *state = sp_new; return sp_result; }
extern "C" int eRand__10GameObject(void*) { log_text += "r"; return rolls[roll_i++ % roll_n]; }
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void* h, float rate, int flags) {
    log_text += "t"; trans_header = h; trans_rate = bits_of(rate); trans_flags = flags;
}
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a, float b, int flags) {
    log_text += "o"; shot_header = h; shot_rate = bits_of(a); if (bits_of(a) != bits_of(b)) log_text += "!"; shot_flags = flags;
}
extern "C" void animGlobalSync__FP14AnimationState(void*) { log_text += "g"; }
extern "C" void resetRouting__8Creature(void*) { log_text += "z"; }
extern "C" void getSmartRandomDestination__8Creaturei(void* r, void*, int range) {
    log_text += range == 200 ? "d" : "?";
    for (int i = 0; i < 12; ++i) static_cast<unsigned char*>(r)[i] = static_cast<unsigned char>(0x61 + i);
}
extern "C" void listDestroy__FP8ListHead(void*) { log_text += "l"; }
extern "C" void objectAddToDeleteList__FP10GameObject(void*) { log_text += "e"; }
extern "C" int modelGetNmShadowPoints__FP12_modelHeader(void*) { log_text += "n"; return nm_count; }
extern "C" void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void*, void*, const void* point, short a, int index, void* out, float*, void* matrix, float scale) {
    log_text += "w";
    const float* p = static_cast<const float*>(point);
    if (p[0] != 0.0f || p[1] != 0.0f || p[2] != 0.0f || a != -16384 || matrix != nullptr || bits_of(scale) != 0x42280000u) log_text += "!";
    if (set_minion_in_shadow) rtti_c::write_word(rtti_c::address(object) + 960u, rtti_c::address_bits(minion_block));
    static_cast<float*>(out)[0] = static_cast<float>(index); static_cast<float*>(out)[1] = 2.0f; static_cast<float*>(out)[2] = 3.0f;
}
extern "C" void getSoulAttachMatrix__6MinionP8Matrix34(void*, void*) { log_text += "A"; }
extern "C" void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(void*, void*, int, void*, float) { log_text += "B"; }
extern "C" void matInvert__FP8Matrix34T0(void*, void*) { log_text += "I"; }
extern "C" void matMul__FP8Matrix34N20(void*, void*, void*) { log_text += "M"; }
extern "C" void matTransform__FP6Point3P8Matrix34T0(void*, void*, void*) { log_text += "T"; }
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* def, void* pos, void* vel, float value) {
    log_text += "P";
    if (def != soulParticle || value != 0.0f) log_text += "!";
    std::memcpy(velocities[particles & 7], vel, 12);
    std::memcpy(last_velocity, vel, 12);
    (void)pos;
    ++particles;
}
static void slot184(void*, int arg) { log_text += "u"; slot_arg = arg; }
static void slot192(void*) { log_text += "v"; }
static void* slot128(void*) { log_text += "m"; return headers; }
static rtti_c::Address a() { return rtti_c::address(object); }
static unsigned w(unsigned o) { return rtti_c::read_word(a() + o); }
static unsigned half() { return rtti_c::read_u16(a() + 572u); }
static void* hdr(unsigned off) { return headers + off; }
static void reset(unsigned cur, unsigned minion) {
    std::memset(object, 0, sizeof object);
    std::memset(vtable, 0, sizeof vtable);
    static unsigned char table[160];
    std::memset(table, 0, sizeof table);
    rtti_c::host_note(table);
    rtti_c::host_note(minion_block);
    for (unsigned off : {28u, 44u, 52u, 60u, 68u, 76u, 84u, 92u, 108u, 116u, 124u, 132u, 140u, 148u}) {
        rtti_c::host_note(hdr(off));
        rtti_c::write_word(rtti_c::address(table) + off, rtti_c::address_bits(hdr(off)));
    }
    rtti_c::write_word(a() + 36u, rtti_c::address_bits(table));
    rtti_c::write_word(a() + 72u, rtti_c::address_bits(vtable));
    rtti_c::host_note(vtable);
    rtti_c::host_note(reinterpret_cast<void*>(slot128)); rtti_c::host_note(reinterpret_cast<void*>(slot184)); rtti_c::host_note(reinterpret_cast<void*>(slot192));
    rtti_c::write_word(rtti_c::address(vtable) + 132u, rtti_c::address_bits(reinterpret_cast<void*>(slot128)));
    rtti_c::write_word(rtti_c::address(vtable) + 188u, rtti_c::address_bits(reinterpret_cast<void*>(slot184)));
    rtti_c::write_word(rtti_c::address(vtable) + 196u, rtti_c::address_bits(reinterpret_cast<void*>(slot192)));
    rtti_c::write_word(a() + 808u, 7000u);
    rtti_c::write_word(a() + 224u, 0x42280000u);
    rtti_c::write_u16(a() + 572u, cur);
    rtti_c::write_word(a() + 960u, minion ? rtti_c::address_bits(minion_block) : 0u);
    rtti_c::write_word(rtti_c::address(minion_block) + 12u, 0x1234u);
    std::strcpy(levelName + 0x51, "other");
    log_text.clear(); sp_result = 0; sp_new = -1000; nm_count = 0; slot_arg = -1; roll_n = 2; rolls[0] = 1000; rolls[1] = 61; roll_i = 0; particles = 0;
    trans_header = shot_header = nullptr; trans_flags = shot_flags = 0;
}
static int bad = 0;
static void expect(bool ok, const char* msg, int x = 0) { if (!ok) { std::printf("fail: %s (%d) log=%s\n", msg, x, log_text.c_str()); ++bad; } }
int main() {
    const unsigned fast = 0x3dccccccu, slow = 0x3e999999u;
    reset(5, 0); norrath_soul_set_state(object, 0);
    expect(log_text == "prt" && trans_header == hdr(60) && trans_rate == slow && trans_flags == 3 && w(256) == 7000u + 1000u % 120u + 60u && half() == 0u, "0");
    reset(5, 0); norrath_soul_set_state(object, 1);
    expect(log_text == "pzdtgr" && trans_header == hdr(76) && trans_rate == slow && trans_flags == 2051 && object[272] == 0x61 && object[283] == 0x6c && w(256) == 7000u + 1000u % 120u + 300u, "1");
    for (int s : {5, 13}) { reset(5, 0); norrath_soul_set_state(object, s); expect(log_text == "pt" && trans_header == hdr(52) && trans_flags == 3, "5/13", s); }
    reset(14, 0); norrath_soul_set_state(object, 8); expect(log_text == "pu" && slot_arg == 15 && half() == 8u, "8 with current 14");
    reset(4, 0); norrath_soul_set_state(object, 8); expect(log_text == "pu" && slot_arg == 14, "8 otherwise");
    reset(77, 0); norrath_soul_set_state(object, 9); expect(log_text == "p" && half() == 77u, "9 keeps the current halfword");
    reset(5, 0); norrath_soul_set_state(object, 10);
    expect(log_text == "ptv" && trans_header == hdr(140) && trans_rate == fast && trans_flags == 6 && w(256) == 7180u && w(12) == 0x4000u, "10");
    reset(5, 0); norrath_soul_set_state(object, 14);
    expect(log_text == "ptr" && trans_header == hdr(68) && trans_rate == slow && w(256) == 7000u + 1000u % 120u, "14");
    reset(14, 0); norrath_soul_set_state(object, 15); expect(log_text == "po" && shot_header == hdr(44) && shot_rate == fast && shot_flags == 16 && half() == 15u, "15 with current 14");
    reset(3, 0); norrath_soul_set_state(object, 15); expect(log_text == "p" && half() == 3u, "15 otherwise keeps current");
    const struct { int state; unsigned off, rate, flags; } shots[] = {{16, 84, fast, 8}, {18, 116, fast, 12}, {19, 92, 0x3f000000u, 8}, {22, 132, fast, 8}};
    for (const auto& s : shots) { reset(5, 0); norrath_soul_set_state(object, s.state); expect(log_text == "po" && shot_header == hdr(s.off) && shot_rate == s.rate && shot_flags == s.flags, "one-shot state", s.state); }
    reset(5, 0); norrath_soul_set_state(object, 20); expect(log_text == "pt" && trans_header == hdr(108) && trans_rate == fast && trans_flags == 3, "20");
    reset(5, 0); norrath_soul_set_state(object, 21); expect(log_text == "pt" && trans_header == hdr(124) && trans_rate == slow && trans_flags == 3, "21");
    reset(5, 0); rtti_c::write_word(a() + 960u, 0u); norrath_soul_set_state(object, 17);
    expect(log_text == "pl" && w(960) == 0u && w(256) == 7240u, "17");
    reset(5, 0); norrath_soul_set_state(object, 24);
    expect(log_text == "pt" && trans_header == hdr(148) && trans_rate == fast && trans_flags == 6 && w(256) == 7300u && w(12) == 9u, "24");
    reset(5, 0); std::strcpy(levelName + 0x51, "AirHub"); norrath_soul_set_state(object, 0);
    expect(trans_header == hdr(148) && half() == 24u, "airhub forces 24");
    for (int s : {2, 3, 4, 6, 7, 11, 12, 25, 26, 100, -1}) { reset(9, 0); norrath_soul_set_state(object, s); expect(log_text == "p" && half() == (static_cast<unsigned>(s) & 0xffffu), "quiet states", s); }
    // early returns
    reset(5, 0); sp_result = 1; sp_new = 20; norrath_soul_set_state(object, 0); expect(log_text == "p" && half() == 5u, "processing result returns");
    reset(5, 1); norrath_soul_set_state(object, 0); expect(log_text == "p" && half() == 5u && w(256) == 0u, "minion present: state outside 16..22 returns");
    reset(5, 1); norrath_soul_set_state(object, 16); expect(log_text == "po" && half() == 16u, "minion present: 16 passes");
    reset(5, 1); norrath_soul_set_state(object, 22); expect(log_text == "po", "minion present: 22 passes");
    reset(5, 1); norrath_soul_set_state(object, 23); expect(log_text == "p", "minion present: 23 returns");
    reset(17, 0); norrath_soul_set_state(object, 0); expect(log_text == "p" && half() == 17u && w(256) == 0u, "current 17 returns");
    // state 23: shadow particles
    reset(5, 0); nm_count = 0; norrath_soul_set_state(object, 23);
    expect(log_text == "pmne" && w(12) == 0x4002u && half() == 23u, "23 without shadow points");
    reset(5, 0); nm_count = 2; roll_n = 3; rolls[0] = 40000; rolls[1] = 32768; rolls[2] = 20000;
    norrath_soul_set_state(object, 23);
    expect(log_text == "pmnmwrrrPrrrPmwrrrPrrrPe", "23 order");
    expect(particles == 4, "23 particle count", particles);
    expect(last_velocity[2] == 0.0f, "velocity z is zero");
    // x = 2 * (40000 * 2^-16) - 1 = 0.220703125, y = 0, z rejected input: 15 * x
    expect(velocities[0][0] == 15.0f * (2.0f * (40000.0f / 65536.0f) - 1.0f) && velocities[0][1] == 15.0f * (2.0f * (32768.0f / 65536.0f) - 1.0f), "velocity scaling");
    reset(5, 0); nm_count = 1; roll_n = 5; rolls[0] = 65536; rolls[1] = 65536; rolls[2] = 65536; rolls[3] = 32768; rolls[4] = 32768;
    norrath_soul_set_state(object, 23);
    expect(log_text.find("rrrrrrP") != std::string::npos, "rejection resamples all three components");
    // With a minion the state filter returns before state 23, so the attach chain can only run when the minion
    // word becomes nonzero during the shadow-position call.
    reset(5, 0); nm_count = 1; roll_n = 3; rolls[0] = 32768; rolls[1] = 32768; rolls[2] = 32768; set_minion_in_shadow = true;
    norrath_soul_set_state(object, 23);
    expect(log_text.find("wABIMTrrrP") != std::string::npos, "minion attach chain runs between the shadow call and the particles");
    set_minion_in_shadow = false;
    std::printf(bad ? "failed\n" : "norrath_soul_set_state passed\n");
    return bad != 0;
}
