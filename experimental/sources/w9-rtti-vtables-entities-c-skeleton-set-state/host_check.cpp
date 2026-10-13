// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "set_state.cpp"
#include "../w9-rtti-vtables-entities-c-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
unsigned char skeletonResMap[800];
static std::string log_text;
static int sp_result = 0, sp_new = -1000, roll_values[3], roll_i = 0;
static void* trans_header = nullptr;
static int trans_flags = 0, shot_flags = 0, drop_kind = 0, treasure_a = 0;
static void* shot_header = nullptr;
static unsigned trans_rate = 0, shot_rate = 0, shot_rate2 = 0;
static unsigned float_bits[6];
static char treasure_name[16];
static unsigned char treasure_point[12];
static unsigned char headers[200];
static unsigned char object[1300];
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
extern "C" int setStateProcessing__8CreaturePi(void*, int* state) { log_text += "p"; if (sp_new != -1000) *state = sp_new; return sp_result; }
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void* h, float rate, int flags) {
    log_text += "t"; trans_header = h; trans_rate = bits_of(rate); trans_flags = flags;
}
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a, float b, int flags) {
    log_text += "o"; shot_header = h; shot_rate = bits_of(a); shot_rate2 = bits_of(b); shot_flags = flags;
}
extern "C" void animGlobalSync__FP14AnimationState(void*) { log_text += "g"; }
extern "C" int eRand__10GameObject(void*) { log_text += "r"; return roll_values[roll_i++ % 3]; }
extern "C" void dropItem__8Skeletoni(void*, int kind) { log_text += "x"; drop_kind = kind; }
extern "C" void dropTreasure__8CreatureG6Point3iffffffPc(void*, const void* point, int a, float f12, float f13, float f14, float f15, float f16, float f17, char* name) {
    log_text += "y"; std::memcpy(treasure_point, point, 12); treasure_a = a;
    float_bits[0] = bits_of(f12); float_bits[1] = bits_of(f13); float_bits[2] = bits_of(f14); float_bits[3] = bits_of(f15); float_bits[4] = bits_of(f16); float_bits[5] = bits_of(f17);
    std::strncpy(treasure_name, name, 15);
}
extern "C" void objectAddToDeleteList__FP10GameObject(void*) { log_text += "e"; }
static void slot(void*) { log_text += "s"; }
static unsigned char table[256];
static rtti_c::Address a() { return rtti_c::address(object); }
static unsigned w(unsigned o) { return rtti_c::read_word(a() + o); }
static unsigned long long d(unsigned o) { return rtti_c::read_u64(a() + o); }
static unsigned half() { return rtti_c::read_u16(a() + 572u); }
static void* hdr(unsigned variant, unsigned off) { return headers + (104u * variant + off) / 4; }
static void reset(unsigned cur, unsigned variant) {
    std::memset(object, 0, sizeof object);
    std::memset(skeletonResMap, 0, sizeof skeletonResMap);
    for (unsigned v : {0u, 1u, 2u, 6u})
        for (unsigned off : {12u, 20u, 28u, 36u, 44u, 52u, 60u, 68u, 84u, 108u}) {
            rtti_c::host_note(hdr(v, off));
            rtti_c::write_word(rtti_c::address(skeletonResMap) + 104u * v + off, rtti_c::address_bits(hdr(v, off)));
        }
    rtti_c::write_u16(a() + 572u, cur);
    rtti_c::write_word(a() + 968u, variant);
    rtti_c::write_word(a() + 808u, 7000u);
    std::memset(table, 0, sizeof table);
    rtti_c::host_install_slot(object, table, 192u, 0, reinterpret_cast<void*>(slot));
    log_text.clear(); sp_result = 0; sp_new = -1000; roll_i = 0; roll_values[0] = 1; roll_values[1] = 45; roll_values[2] = 2;
    trans_header = shot_header = nullptr; trans_flags = shot_flags = 0; drop_kind = 0;
}
static int bad = 0;
static void expect(bool ok, const char* msg, int x = 0) { if (!ok) { std::printf("fail: %s (%d)\n", msg, x); ++bad; } }
int main() {
    const unsigned tenth = 0x3dccccccu;
    for (unsigned v : {0u, 2u, 6u}) {
        reset(5, v); norrath_skeleton_set_state(object, 0);
        expect(log_text == "pt" && trans_header == hdr(v, 68) && trans_rate == tenth && trans_flags == 3 && w(256) == 7180u && half() == 0u, "0", v);
        reset(5, v); norrath_skeleton_set_state(object, 1);
        expect(log_text == "ptg" && trans_header == hdr(v, 84) && trans_flags == 2051, "1", v);
        reset(5, v); norrath_skeleton_set_state(object, 3);
        expect(log_text == "ptg" && trans_header == hdr(v, 108) && trans_flags == 2051, "3", v);
        reset(5, v); norrath_skeleton_set_state(object, 5);
        expect(log_text == "pt" && trans_header == hdr(v, 44) && trans_flags == 3, "5", v);
        reset(5, v); rtti_c::write_u64(a() + 160u, 0x200ull); norrath_skeleton_set_state(object, 15);
        expect(log_text == "pt" && trans_header == hdr(v, 44) && w(256) == 7120u && d(160) == 0x200ull, "15 sets bit 0x200 again", v);
        reset(5, v); rtti_c::write_u64(a() + 160u, 0x300ull); norrath_skeleton_set_state(object, 5);
        expect(d(160) == 0x100ull, "bit 0x200 is cleared before dispatch", v);
        reset(5, v); norrath_skeleton_set_state(object, 9);
        expect(log_text == "pt" && trans_header == hdr(v, 68), "9", v);
        reset(5, v); norrath_skeleton_set_state(object, 13);
        expect(log_text == "ptor" && trans_header == hdr(v, 44) && shot_header == hdr(v, 28) && shot_flags == 2056 && shot_rate == tenth && shot_rate2 == tenth && w(256) == 1u % 20u + 20u, "13", v);
        reset(5, v); norrath_skeleton_set_state(object, 6);
        expect(log_text == "ptror" && shot_header == hdr(v, 20) && w(256) == 45u % 20u + 20u, "6 odd roll picks R+20 and then rolls again", v);
        reset(5, v); roll_values[0] = 4; norrath_skeleton_set_state(object, 6);
        expect(shot_header == hdr(v, 12), "6 even roll picks R+12", v);
        reset(5, v); norrath_skeleton_set_state(object, 14);
        expect(log_text == "ptro" && trans_header == hdr(v, 60) && shot_header == hdr(v, 20) && w(256) == 1u && shot_flags == 2056, "14", v);
    }
    reset(9, 1); norrath_skeleton_set_state(object, 8);
    expect(log_text == "p" && half() == 9u, "8 with current 9");
    reset(4, 1); norrath_skeleton_set_state(object, 8);
    expect(log_text == "po" && shot_header == hdr(1, 52) && shot_flags == 16 && half() == 8u && shot_rate == tenth, "8 otherwise");
    reset(4, 1); rtti_c::write_word(a() + 12u, 1u); norrath_skeleton_set_state(object, 10);
    expect(log_text == "pxyes" && drop_kind == 8 && w(12) == 0x4003u && d(160) == (1ull << 41) && treasure_a == 0 && std::strcmp(treasure_name, "bonechip") == 0 && half() == 10u, "10");
    expect(float_bits[0] == 0x42100000u && float_bits[1] == 0x40000000u && float_bits[2] == 0x3fcccccc && float_bits[3] == 0x3f199999u && float_bits[4] == 0x3fcccccc && float_bits[5] == 0x3e800000u, "10 floats");
    reset(4, 1); for (int i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0x20 + i);
    rtti_c::write_u16(a() + 56u, 0xfffeu); norrath_skeleton_set_state(object, 10);
    expect(std::memcmp(treasure_point, object + 16, 12) == 0 && treasure_a == -2, "10 point copy and signed halfword");
    reset(4, 1); rtti_c::write_u64(a() + 160u, 0x1000000ull); norrath_skeleton_set_state(object, 10);
    expect(log_text == "pxes" && d(160) == (0x1000000ull | (1ull << 41)), "10 skips the treasure drop when bit 0x1000000 is set");
    for (int s : {2, 4, 7, 11, 12, 16, 17, 100, -1}) {
        reset(4, 1); norrath_skeleton_set_state(object, s);
        expect(log_text == "p" && half() == (static_cast<unsigned>(s) & 0xffffu), "quiet states", s);
    }
    reset(4, 1); sp_result = 1; sp_new = 5; norrath_skeleton_set_state(object, 0);
    expect(log_text == "p" && half() == 4u && d(160) == 0ull, "nonzero processing result returns first");
    reset(4, 1); sp_new = 5; norrath_skeleton_set_state(object, 0);
    expect(log_text == "pt" && half() == 5u, "processing may replace the state");
    std::printf(bad ? "failed\n" : "norrath_skeleton_set_state passed\n");
    return bad != 0;
}
