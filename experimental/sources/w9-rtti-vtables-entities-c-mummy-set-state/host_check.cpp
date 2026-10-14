// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "set_state.cpp"
#include "../w9-rtti-vtables-entities-c-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>
struct Call { char kind; unsigned header; unsigned rate; int flags; };
static std::vector<Call> calls;
static std::string log_text;
static int sp_result = 0, sp_new = -1000, rolls[4], roll_n = 0, roll_i = 0, init_flag = -1;
static unsigned char headers[400];
static unsigned char object[1300];
static unsigned char vtable[256];
static unsigned char table[400];
static unsigned char type_info_object[4];
static unsigned weapon_at_init = 0, flags_at_init = 0;
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
static rtti_c::Address a() { return rtti_c::address(object); }
static unsigned w(unsigned o) { return rtti_c::read_word(a() + o); }
static unsigned long long d(unsigned o) { return rtti_c::read_u64(a() + o); }
static unsigned half(unsigned o = 572u) { return rtti_c::read_u16(a() + o); }
static unsigned offset_of(void* h) { return static_cast<unsigned>(static_cast<unsigned char*>(h) - headers); }
static unsigned treasure_bits[6];
static int treasure_a = 0, dust_a = 0;
static char treasure_name[16];
static unsigned dust_f[2];
static void* dust_model = nullptr;
static unsigned char point_copy[12];
static const void* terminate_type = nullptr;
extern "C" int setStateProcessing__8CreaturePi(void*, int* state) { log_text += "p"; if (sp_new != -1000) *state = sp_new; return sp_result; }
extern "C" const void* __tf15SpellConeOfFire() { log_text += "f"; return type_info_object; }
extern "C" void terminateSpell__8CreatureRC9type_info(void*, const void* t) { log_text += "k"; terminate_type = t; }
extern "C" void norrath_mummy_init_type(void*, int flag) { log_text += "i"; init_flag = flag; weapon_at_init = w(964); }
extern "C" int eRand__10GameObject(void*) { log_text += "r"; return rolls[roll_i++ % roll_n]; }
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void* h, float rate, int flags) { log_text += "t"; calls.push_back({'t', offset_of(h), bits_of(rate), flags}); }
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a_, float b_, int flags) {
    log_text += "o"; calls.push_back({'o', offset_of(h), bits_of(a_), flags}); if (bits_of(a_) != bits_of(b_)) log_text += "!";
}
extern "C" void animGlobalSync__FP14AnimationState(void*) { log_text += "g"; }
extern "C" void resetRouting__8Creature(void*) { log_text += "z"; }
extern "C" void getSmartRandomDestination__8Creaturei(void* r, void*, int range) {
    log_text += range == 200 ? "d" : "?";
    for (int i = 0; i < 12; ++i) static_cast<unsigned char*>(r)[i] = static_cast<unsigned char>(0x61 + i);
}
extern "C" void objectAddToRunList__FP10GameObject(void*) { log_text += "u"; flags_at_init = static_cast<unsigned>(rtti_c::read_u64(rtti_c::address(object) + 160u) >> 32); }
extern "C" void objectAddToDeleteList__FP10GameObject(void*) { log_text += "e"; }
extern "C" void fxCreatureToDust__FP12_modelHeaderP14AnimationStateG6Point3sff(void* model, void*, const void* point, short a_, float b_, float c_) {
    log_text += "x"; dust_model = model; std::memcpy(point_copy, point, 12); dust_a = a_; dust_f[0] = bits_of(b_); dust_f[1] = bits_of(c_);
}
extern "C" void dropTreasure__8CreatureG6Point3iffffffPc(void*, const void* point, int a_, float f12, float f13, float f14, float f15, float f16, float f17, char* name) {
    log_text += "y"; std::memcpy(point_copy, point, 12); treasure_a = a_; std::strncpy(treasure_name, name, 15);
    treasure_bits[0] = bits_of(f12); treasure_bits[1] = bits_of(f13); treasure_bits[2] = bits_of(f14); treasure_bits[3] = bits_of(f15); treasure_bits[4] = bits_of(f16); treasure_bits[5] = bits_of(f17);
}
extern "C" void dropWeapon__5Mummy(void*) { log_text += "w"; }
static void slot192(void*) { log_text += "s"; }
static void reset(unsigned cur, unsigned kind) {
    std::memset(object, 0, sizeof object);
    std::memset(vtable, 0, sizeof vtable);
    std::memset(table, 0, sizeof table);
    rtti_c::host_note(table);
    for (unsigned off : {12u, 28u, 36u, 60u, 68u, 76u, 84u, 92u, 100u, 108u, 116u, 124u, 140u, 148u, 156u, 164u, 180u, 188u, 196u, 204u, 236u, 244u, 252u, 260u, 268u, 276u, 300u, 308u, 316u, 324u, 332u, 340u, 348u}) {
        rtti_c::host_note(headers + off);
        rtti_c::write_word(rtti_c::address(table) + off, rtti_c::address_bits(headers + off));
    }
    rtti_c::write_word(a() + 36u, rtti_c::address_bits(table));
    rtti_c::host_install_slot(object, vtable, 192u, 0, reinterpret_cast<void*>(slot192));
    rtti_c::write_word(a() + 808u, 5000u);
    rtti_c::write_u16(a() + 572u, cur);
    rtti_c::write_word(a() + 960u, kind);
    rtti_c::write_word(a() + 1104u, 0xffffffffu);
    log_text.clear(); calls.clear(); sp_result = 0; sp_new = -1000; roll_n = 2; rolls[0] = 1000; rolls[1] = 61; roll_i = 0; init_flag = -1; terminate_type = nullptr;
}
static int bad = 0;
static void expect(bool ok, const char* msg, int x = 0, int y = 0) { if (!ok) { std::printf("fail: %s (%d, %d) log=%s\n", msg, x, y, log_text.c_str()); ++bad; } }
int main() {
    const unsigned fast = 0x3dccccccu, slow = 0x3e999999u;
    // kind-dependent transitions: {state, header A, header B, header C, flags, log}
    struct K { int state; unsigned a, b, c; int flags; const char* log; };
    const K by_kind_cases[] = {
        {1, 108, 252, 340, 2051, "pfkzdtg"}, {17, 92, 236, 332, 2051, "pfkztg"}, {3, 100, 244, 340, 2051, "pfkztg"},
        {5, 84, 164, 324, 3, "pfktgr"}};
    for (const K& c : by_kind_cases) {
        for (unsigned kind : {0u, 2u, 4u, 1u, 3u, 5u, 9u}) {
            reset(5, kind); norrath_mummy_set_state(object, c.state);
            const unsigned want = (kind == 0u || kind == 2u) ? c.a : kind == 4u ? c.b : c.c;
            expect(log_text == c.log && calls.size() == 1 && calls[0].header == want && calls[0].rate == fast && calls[0].flags == c.flags, "per-kind transition", c.state, kind);
            expect(terminate_type == type_info_object && w(1104) == 0u, "spell termination", c.state, kind);
        }
    }
    reset(5, 0); norrath_mummy_set_state(object, 1); expect(object[272] == 0x61 && object[283] == 0x6c, "destination copy");
    reset(5, 0); norrath_mummy_set_state(object, 5); expect(w(256) == 5000u + 1000u % 120u + 60u, "state 5 +256");
    reset(6, 0); norrath_mummy_set_state(object, 3); expect(log_text == "pfkz" && half() == 3u, "3 with current 6");
    reset(14, 0); norrath_mummy_set_state(object, 3); expect(log_text == "pfkz" && half() == 3u, "3 with current 14");
    // state 0 and 16
    for (unsigned kind : {0u, 2u, 4u, 7u}) {
        reset(5, kind); norrath_mummy_set_state(object, 0);
        const unsigned want = (kind == 0u || kind == 2u) ? 84 : kind == 4u ? 204 : 324;
        expect(log_text == "pfktr" && calls[0].header == want && calls[0].rate == fast && calls[0].flags == 3 && w(256) == 5000u + 1000u % 120u && half() == 0u, "0", kind);
    }
    const unsigned weapons[] = {44, 48, 44, 48, 46, 50};
    for (unsigned kind = 0; kind < 6; ++kind) {
        reset(5, kind); rtti_c::write_word(a() + 12u, 0xffffffffu); rtti_c::write_word(a() + 964u, 7u);
        norrath_mummy_set_state(object, 16);
        expect(log_text == "pfkiutr" && init_flag == 0 && weapon_at_init == weapons[kind] && w(12) == 0xfffffff6u, "16 sequence", kind);
        expect(flags_at_init == 0u && d(160) == (1ull << 32), "16 flag bit set after init", kind);
        expect(half(146) == 50u && half(150) == 128u && half(152) == 255u && half(154) == 150u && half(140) == 0u && half(142) == 128u && half(144) == 255u && half(148) == 0u, "16 halfwords", kind);
        expect(half() == 0u && w(256) == 5000u + 1000u % 120u, "16 ends in state 0", kind);
    }
    reset(5, 6); rtti_c::write_word(a() + 964u, 7u); norrath_mummy_set_state(object, 16); expect(w(964) == 7u, "16 with kind 6 keeps the weapon word");
    // one-shots
    reset(5, 0); norrath_mummy_set_state(object, 15);
    expect(log_text == "pfkto" && w(1112) == 5600u && calls[0].header == 84 && calls[1].header == 348 && calls[1].flags == 2056 && calls[0].flags == 3, "15");
    for (unsigned kind : {0u, 2u}) { reset(5, kind); norrath_mummy_set_state(object, 6); expect(log_text == "pfkro" && calls[0].header == 28 + 8 * (1000 & 1) && calls[0].flags == 2056, "6 kinds 0/2", kind); }
    for (unsigned kind : {4u, 1u}) { reset(5, kind); norrath_mummy_set_state(object, 6); expect(log_text == "pfkro" && calls[0].header == 116 + 8 * (1000 % 3) && calls[0].flags == 2056, "6 other kinds", kind); }
    reset(5, 0); norrath_mummy_set_state(object, 14); expect(log_text == "pfkro" && calls[0].header == 140 + 8 * (1000 % 3), "14");
    reset(5, 0); roll_n = 3; rolls[0] = 1001; rolls[1] = 65536; rolls[2] = 0; norrath_mummy_set_state(object, 13);
    expect(log_text == "pfktror" + std::string("") && calls[0].header == 84 && calls[1].header == 260 + 8 * (1001 % 3) && w(256) == 120u + 240u, "13 (second roll 65536 gives 1.0 * 120 + 240)");
    // state 8
    reset(9, 0); norrath_mummy_set_state(object, 8); expect(log_text == "pfk" && half() == 9u, "8 with current 9");
    reset(6, 0); rtti_c::write_u16(a() + 294u, 8u); norrath_mummy_set_state(object, 8); expect(log_text == "pfk" && half() == 6u, "8 with current 6 and bit 8");
    reset(14, 4); rtti_c::write_u16(a() + 294u, 8u); norrath_mummy_set_state(object, 8); expect(log_text == "pfk" && half() == 14u, "8 with current 14 and bit 8");
    for (unsigned kind : {0u, 2u, 4u, 8u}) {
        reset(6, kind); norrath_mummy_set_state(object, 8);
        const unsigned want = (kind == 0u || kind == 2u) ? 76 : kind == 4u ? 196 : 316;
        expect(log_text == "pfko" && calls[0].header == want && calls[0].rate == fast && calls[0].flags == 16 && half() == 8u, "8 one-shot", kind);
    }
    reset(5, 0); norrath_mummy_set_state(object, 9); expect(log_text == "pfkt" && calls[0].header == 84 && calls[0].rate == slow && calls[0].flags == 3, "9");
    // state 10
    for (unsigned kind : {0u, 4u, 5u}) {
        reset(5, kind); rtti_c::write_u64(a() + 160u, 0ull); for (int i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0x20 + i); rtti_c::write_u16(a() + 56u, 0xfffeu);
        norrath_mummy_set_state(object, 10);
        const unsigned want = (kind == 0u ? 60 : kind == 4u ? 180 : 300) + 8 * (1000 & 1);
        expect(log_text == "pfkyrtws" && calls[0].header == want && calls[0].rate == slow && calls[0].flags == 2054, "10 drop path", kind);
        expect(std::memcmp(point_copy, object + 16, 12) == 0 && treasure_a == -2 && std::strcmp(treasure_name, "mummytongue") == 0, "10 treasure arguments", kind);
        expect(treasure_bits[0] == 0x42100000u && treasure_bits[1] == 0x40000000u && treasure_bits[2] == 0x3fcccccc && treasure_bits[3] == 0x3f199999u && treasure_bits[4] == 0x3fb33333u && treasure_bits[5] == 0x3e800000u, "10 treasure floats", kind);
    }
    reset(5, 0); rtti_c::write_u64(a() + 160u, 1ull << 32); norrath_mummy_set_state(object, 10);
    expect(log_text == "pfkrtws", "10 skips the treasure drop when bit 32 is set");
    reset(5, 0); rtti_c::write_u64(a() + 160u, 0x1000000ull); rtti_c::write_word(a() + 224u, 0x3f800000u); rtti_c::write_word(a() + 228u, 0x40000000u); rtti_c::write_u16(a() + 56u, 7u);
    norrath_mummy_set_state(object, 10);
    expect(log_text == "pfkxews" && dust_a == 7 && dust_f[0] == 0x3f800000u && dust_f[1] == 0x40000000u && dust_model == headers + 12 && w(12) == 0x4002u && d(160) == (0x1000000ull | (1ull << 41)), "10 dust path");
    for (int s : {2, 4, 7, 11, 12, 18, 100, -1}) { reset(9, 1); norrath_mummy_set_state(object, s); expect(log_text == "pfk" && half() == (static_cast<unsigned>(s) & 0xffffu), "quiet states", s); }
    reset(9, 1); sp_result = 1; sp_new = 5; norrath_mummy_set_state(object, 0); expect(log_text == "p" && half() == 9u && w(1104) == 0xffffffffu, "processing result returns before the spell call");
    reset(9, 0); sp_new = 9; norrath_mummy_set_state(object, 0); expect(log_text == "pfkt" && calls[0].rate == slow && half() == 9u, "processing may replace the state");
    std::printf(bad ? "failed\n" : "norrath_mummy_set_state passed\n");
    return bad != 0;
}
