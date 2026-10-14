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
static int sp_result = 0, sp_new = -1000, rolls[4], roll_n = 0, roll_i = 0, edge = 0, valid = 0, drop_kind = 0;
static unsigned char headers[400];
static unsigned char object[1300];
static unsigned char vtable[256];
static unsigned char table[400];
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
static rtti_c::Address a() { return rtti_c::address(object); }
static unsigned w(unsigned o) { return rtti_c::read_word(a() + o); }
static unsigned half() { return rtti_c::read_u16(a() + 572u); }
static unsigned offset_of(void* h) { return static_cast<unsigned>(static_cast<unsigned char*>(h) - headers); }
static unsigned bits_treasure[6];
static int treasure_a = 0;
static char* treasure_name = reinterpret_cast<char*>(1);
static unsigned char treasure_point[12];
extern "C" int setStateProcessing__8CreaturePi(void*, int* state) { log_text += "p"; if (sp_new != -1000) *state = sp_new; return sp_result; }
extern "C" int eRand__10GameObject(void*) { log_text += "r"; return rolls[roll_i++ % roll_n]; }
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void* h, float rate, int flags) {
    log_text += "t"; calls.push_back({'t', offset_of(h), bits_of(rate), flags});
}
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a_, float b_, int flags) {
    log_text += "o"; calls.push_back({'o', offset_of(h), bits_of(a_), flags}); if (bits_of(a_) != bits_of(b_)) log_text += "!";
}
extern "C" void animGlobalSync__FP14AnimationState(void*) { log_text += "g"; }
extern "C" void resetRouting__8Creature(void*) { log_text += "z"; }
extern "C" void getSmartRandomDestination__8Creaturei(void* r, void*, int range) {
    log_text += range == 300 ? "d" : "?";
    for (int i = 0; i < 12; ++i) static_cast<unsigned char*>(r)[i] = static_cast<unsigned char>(0x61 + i);
}
extern "C" int bNextToEdge__14WoodElfSoldier(void*) { log_text += "b"; return edge; }
extern "C" int commanderValid__14WoodElfSoldier(void*) { log_text += "v"; return valid; }
extern "C" void leaveCommander__14WoodElfSoldier(void*) { log_text += "l"; }
extern "C" void dropItem__14WoodElfSoldieri(void*, int kind) { log_text += "x"; drop_kind = kind; }
extern "C" void dropTreasure__8CreatureG6Point3iffffffPc(void*, const void* point, int a_, float f12, float f13, float f14, float f15, float f16, float f17, char* name) {
    log_text += "y"; std::memcpy(treasure_point, point, 12); treasure_a = a_; treasure_name = name;
    bits_treasure[0] = bits_of(f12); bits_treasure[1] = bits_of(f13); bits_treasure[2] = bits_of(f14); bits_treasure[3] = bits_of(f15); bits_treasure[4] = bits_of(f16); bits_treasure[5] = bits_of(f17);
}
static void slot192(void*) { log_text += "s"; }
static void reset(unsigned cur, unsigned type) {
    std::memset(object, 0, sizeof object);
    std::memset(vtable, 0, sizeof vtable);
    std::memset(table, 0, sizeof table);
    rtti_c::host_note(table);
    for (unsigned off : {140u, 148u, 156u, 164u, 172u, 188u, 196u, 204u, 212u, 220u, 228u, 236u, 244u, 252u, 260u, 268u, 284u, 300u, 308u}) {
        rtti_c::host_note(headers + off);
        rtti_c::write_word(rtti_c::address(table) + off, rtti_c::address_bits(headers + off));
    }
    rtti_c::write_word(a() + 36u, rtti_c::address_bits(table));
    rtti_c::host_install_slot(object, vtable, 192u, 0, reinterpret_cast<void*>(slot192));
    rtti_c::write_word(a() + 808u, 5000u);
    rtti_c::write_u16(a() + 572u, cur);
    rtti_c::write_word(a() + 964u, type);
    log_text.clear(); calls.clear(); sp_result = 0; sp_new = -1000; roll_n = 2; rolls[0] = 1000; rolls[1] = 61; roll_i = 0; edge = 0; valid = 0; drop_kind = 0;
}
static int bad = 0;
static void expect(bool ok, const char* msg, int x = 0, int y = 0) { if (!ok) { std::printf("fail: %s (%d, %d) log=%s\n", msg, x, y, log_text.c_str()); ++bad; } }
static const unsigned kSlow = 0x3e999999u, kFast = 0x3dccccccu;
int main() {
    // table of per-type transitions: {state, type, header offset, rate, expected +176 bits (0 = untouched), expected log}
    struct T { int state; unsigned type; unsigned header; unsigned rate; unsigned s176; const char* log; };
    const T cases[] = {
        {9, 0, 148, kSlow, 0, "pt"}, {9, 1, 172, kSlow, 0, "pt"}, {9, 2, 244, kSlow, 0, "pt"}, {9, 3, 252, kSlow, 0, "pt"}, {9, 4, 0, 0, 0, "p"},
        {1, 0, 164, kSlow, 0x42c80000u, "pzdtgr"}, {1, 1, 212, kSlow, 0x42c80000u, "pzdtgr"}, {1, 2, 164, kSlow, 0x42c80000u, "pzdtgr"}, {1, 3, 268, kSlow, 0x43160000u, "pzdtgr"},
        {2, 0, 164, kSlow, 0x42c80000u, "pztg"}, {2, 1, 212, kSlow, 0x42c80000u, "pztg"}, {2, 2, 164, kSlow, 0x42c80000u, "pztg"}, {2, 3, 300, kSlow, 0x42c80000u, "pztg"},
        {7, 0, 156, kSlow, 0x43160000u, "pztg"}, {7, 1, 188, kSlow, 0x43160000u, "pztg"}, {7, 2, 156, kSlow, 0x43160000u, "pztg"}, {7, 3, 268, kSlow, 0x43160000u, "pztg"},
        {3, 0, 156, kSlow, 0x43160000u, "pztg"}, {3, 1, 212, kSlow, 0x42c80000u, "pztg"}, {3, 2, 156, kSlow, 0x43160000u, "pztg"}, {3, 3, 268, kSlow, 0x43160000u, "pztg"},
        {5, 0, 148, kFast, 0, "pt"}, {5, 1, 172, kFast, 0, "pt"}, {5, 2, 244, kFast, 0, "pt"}, {5, 3, 260, kFast, 0, "pt"},
        {13, 0, 236, kSlow, 0, "pt"}};
    for (const T& c : cases) {
        reset(5, c.type); norrath_wood_elf_soldier_set_state(object, c.state);
        const bool has = c.rate != 0;
        expect(log_text == c.log, "call order", c.state, c.type);
        expect(!has || (calls.size() == 1 && calls[0].header == c.header && calls[0].rate == c.rate && calls[0].flags == 3), "transition", c.state, c.type);
        expect(w(176) == c.s176, "+176", c.state, c.type);
        expect(half() == static_cast<unsigned>(c.state), "halfword", c.state, c.type);
    }
    reset(5, 0); norrath_wood_elf_soldier_set_state(object, 1); expect(w(256) == 5000u + 1000u % 120u + 300u && object[272] == 0x61 && object[283] == 0x6c, "state 1 +256 and destination");
    reset(5, 0); norrath_wood_elf_soldier_set_state(object, 7); expect(w(256) == 5120u, "state 7 +256");
    reset(5, 0); norrath_wood_elf_soldier_set_state(object, 13); expect(w(256) == 5060u && (rtti_c::read_u64(a() + 160u) & 0x200ull) != 0ull, "state 13 +256 and bit 0x200");
    reset(5, 0); rtti_c::write_u64(a() + 160u, ~0ull); norrath_wood_elf_soldier_set_state(object, 5); expect((rtti_c::read_u64(a() + 160u) & 0x200ull) == 0ull, "other states clear bit 0x200");
    // state 0
    reset(5, 0); norrath_wood_elf_soldier_set_state(object, 0);
    expect(log_text == "prt" && w(256) == 5000u + 1000u % 120u + 120u && calls[0].header == 148, "0 type 0");
    reset(5, 1); edge = 0; norrath_wood_elf_soldier_set_state(object, 0);
    expect(log_text == "prbt" && w(256) == 5000u + 1000u % 120u + 120u && calls[0].header == 172, "0 type 1 away from an edge");
    reset(5, 1); edge = 1; norrath_wood_elf_soldier_set_state(object, 0);
    expect(log_text == "prbrt" && w(256) == 5000u + 61u % 60u && calls[0].header == 172, "0 type 1 next to an edge");
    reset(5, 2); edge = 1; norrath_wood_elf_soldier_set_state(object, 0);
    expect(log_text == "prt" && calls[0].header == 244, "0 type 2 never asks about edges");
    reset(5, 4); norrath_wood_elf_soldier_set_state(object, 0); expect(log_text == "pr" && w(256) == 5000u + 1000u % 120u + 120u, "0 type 4");
    // states 6, 15, 14
    struct S { int state; unsigned type; unsigned t_header, o_header; unsigned rate; const char* log; unsigned s256; };
    const S shots[] = {
        {6, 0, 148, 140, kFast, "ptor", 1000u % 10u}, {6, 1, 0, 0, 0, "pr", 1000u % 10u}, {6, 2, 244, 220, kFast, "ptor", 1000u % 10u}, {6, 3, 260, 284, kFast, "ptor", 1000u % 10u},
        {15, 1, 172, 196, kFast, "ptor", 1000u % 10u + 10u}, {15, 3, 260, 196, kFast, "ptor", 1000u % 10u + 10u}, {15, 0, 0, 0, 0, "pr", 1000u % 10u + 10u},
        {14, 1, 172, 204, kFast, "ptor", 1000u % 60u + 60u}, {14, 0, 0, 0, 0, "pr", 1000u % 60u + 60u}};
    for (const S& s : shots) {
        reset(5, s.type); norrath_wood_elf_soldier_set_state(object, s.state);
        expect(log_text == s.log, "shot order", s.state, s.type);
        expect(s.rate == 0 || (calls.size() == 2 && calls[0].header == s.t_header && calls[0].rate == s.rate && calls[0].flags == 3 && calls[1].header == s.o_header && calls[1].rate == s.rate && calls[1].flags == 8), "shot calls", s.state, s.type);
        expect(w(256) == s.s256, "shot +256", s.state, s.type);
    }
    // state 8
    reset(9, 0); norrath_wood_elf_soldier_set_state(object, 8); expect(log_text == "p" && half() == 9u, "8 with current 9");
    reset(6, 0); rtti_c::write_u16(a() + 294u, 8u); norrath_wood_elf_soldier_set_state(object, 8); expect(log_text == "p" && half() == 6u, "8 with current 6 and bit 8");
    reset(6, 2); norrath_wood_elf_soldier_set_state(object, 8); expect(log_text == "po" && calls[0].header == 228 && calls[0].rate == kFast && calls[0].flags == 16 && half() == 8u, "8 with current 6 without bit 8, type 2");
    reset(5, 2); norrath_wood_elf_soldier_set_state(object, 8); expect(log_text == "po" && half() == 8u, "8 with type 2");
    reset(5, 1); norrath_wood_elf_soldier_set_state(object, 8); expect(log_text == "p" && half() == 5u, "8 otherwise keeps the current halfword");
    // state 10
    reset(5, 0); valid = 1; norrath_wood_elf_soldier_set_state(object, 10);
    expect(log_text == "pvlxtys" && drop_kind == 1 && calls[0].header == 308 && calls[0].rate == kSlow && calls[0].flags == 6, "10 with commander and no item bit");
    expect(treasure_name == nullptr && bits_treasure[0] == 0x42100000u && bits_treasure[1] == 0x40000000u && bits_treasure[2] == 0x3fcccccc && bits_treasure[3] == 0x3f199999u && bits_treasure[4] == 0x3fcccccc && bits_treasure[5] == 0x3e800000u, "10 treasure arguments");
    reset(5, 0); rtti_c::write_word(a() + 12u, 2u); for (int i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0x20 + i); rtti_c::write_u16(a() + 56u, 0xfffeu);
    norrath_wood_elf_soldier_set_state(object, 10);
    expect(log_text == "pvtys" && std::memcmp(treasure_point, object + 16, 12) == 0 && treasure_a == -2, "10 without a commander and with the item bit");
    // quiet states and early returns
    for (int s : {4, 11, 12, 16, 17, 99, -1}) { reset(9, 1); norrath_wood_elf_soldier_set_state(object, s); expect(log_text == "p" && half() == (static_cast<unsigned>(s) & 0xffffu), "quiet states", s); }
    reset(9, 1); sp_result = 1; sp_new = 5; norrath_wood_elf_soldier_set_state(object, 0); expect(log_text == "p" && half() == 9u, "processing result returns");
    reset(9, 1); sp_new = 5; norrath_wood_elf_soldier_set_state(object, 0); expect(log_text == "pt" && half() == 5u && calls[0].header == 172, "processing may replace the state");
    std::printf(bad ? "failed\n" : "norrath_wood_elf_soldier_set_state passed\n");
    return bad != 0;
}
