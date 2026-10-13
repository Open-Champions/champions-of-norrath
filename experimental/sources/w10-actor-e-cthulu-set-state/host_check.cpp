// Host contract for setState__6Cthului. Includes the source; not an EE input.
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
static int sp_result = 0, sp_new = -1000, rolls[4], roll_n = 0, roll_i = 0, range_seen = 0;
static unsigned char headers[400], object[1300], vtable[256], table[400];
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
static rtti_c::Address a() { return rtti_c::address(object); }
static unsigned w(unsigned o) { return rtti_c::read_word(a() + o); }
static unsigned half(unsigned o = 572u) { return rtti_c::read_u16(a() + o); }
static unsigned offset_of(void* h) { return static_cast<unsigned>(static_cast<unsigned char*>(h) - headers); }
static unsigned treasure_bits[6]; static int treasure_a = 0; static char* treasure_name = reinterpret_cast<char*>(1);
static unsigned char point_copy[12]; static unsigned w256_at_call = 0;
extern "C" int setStateProcessing__8CreaturePi(void*, int* state) { log_text += "p"; if (sp_new != -1000) *state = sp_new; return sp_result; }
extern "C" int eRand__10GameObject(void*) { log_text += "r"; return rolls[roll_i++ % roll_n]; }
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void* h, float rate, int flags) {
    log_text += "t"; w256_at_call = w(256); calls.push_back({'t', offset_of(h), bits_of(rate), flags});
}
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a_, float b_, int flags) {
    log_text += "o"; calls.push_back({'o', offset_of(h), bits_of(a_), flags}); if (bits_of(a_) != bits_of(b_)) log_text += "!";
}
extern "C" void animGlobalSync__FP14AnimationState(void*) { log_text += "g"; }
extern "C" void resetRouting__8Creature(void*) { log_text += "z"; }
extern "C" void getSmartRandomDestination__8Creaturei(void* r, void*, int range) {
    log_text += "d"; range_seen = range;
    for (int i = 0; i < 12; ++i) static_cast<unsigned char*>(r)[i] = static_cast<unsigned char>(0x61 + i);
}
extern "C" void dropTreasure__8CreatureG6Point3iffffffPc(void*, const void* point, int a_, float f12, float f13, float f14, float f15, float f16, float f17, char* name) {
    log_text += "y"; std::memcpy(point_copy, point, 12); treasure_a = a_; treasure_name = name; 
    treasure_bits[0] = bits_of(f12); treasure_bits[1] = bits_of(f13); treasure_bits[2] = bits_of(f14); treasure_bits[3] = bits_of(f15); treasure_bits[4] = bits_of(f16); treasure_bits[5] = bits_of(f17);
}
static void* slot_self = nullptr;
static void slot192(void* self) { log_text += "s"; slot_self = self; }
static void reset(unsigned cur) {
    std::memset(object, 0, sizeof object); std::memset(vtable, 0, sizeof vtable); std::memset(table, 0, sizeof table);
    rtti_c::host_note(table);
    for (unsigned off : {28u, 36u, 44u, 52u, 60u, 68u, 76u, 84u, 92u, 108u, 116u, 124u, 148u, 156u}) {
        rtti_c::host_note(headers + off);
        rtti_c::write_word(rtti_c::address(table) + off, rtti_c::address_bits(headers + off));
    }
    rtti_c::write_word(a() + 36u, rtti_c::address_bits(table));
    rtti_c::host_install_slot(object, vtable, 192u, -8, reinterpret_cast<void*>(slot192));
    rtti_c::write_word(a() + 808u, 5000u); rtti_c::write_word(a() + 960u, 0u);
    rtti_c::write_u16(a() + 572u, cur);
    log_text.clear(); calls.clear(); sp_result = 0; sp_new = -1000; roll_n = 2; rolls[0] = 1000; rolls[1] = 61; roll_i = 0; slot_self = nullptr;
}
static int bad = 0;
static void expect(bool ok, const char* msg, int x = 0, int y = 0) { if (!ok) { std::printf("fail: %s (%d, %d) log=%s\n", msg, x, y, log_text.c_str()); ++bad; } }
static void flag(unsigned v) { rtti_c::write_word(a() + 964u, v); }
int main() {
    const unsigned fast = 0x3dccccccu, slow = 0x3e999999u;
    for (unsigned f : {0u, 1u, 0x80000000u}) {
        const bool set = f != 0u;
        reset(5); flag(f); setState__6Cthului(object, 0);
        expect(log_text == "prt" && calls[0].header == (set ? 156u : 76u) && calls[0].rate == fast && calls[0].flags == 3 && w(256) == 5000u + 1000u % 120u && w256_at_call == w(256), "0", f);
        reset(5); flag(f); setState__6Cthului(object, 1);
        expect(log_text == "pzdrtg" && range_seen == 200 && object[272] == 0x61 && object[283] == 0x6c && calls[0].header == (set ? 84u : 68u) && calls[0].flags == 2051 &&
               w(256) == 5000u + 1000u % 120u + 300u && w256_at_call == w(256), "1", f);
        reset(5); flag(f); setState__6Cthului(object, 5); expect(log_text == "pt" && calls[0].header == (set ? 156u : 76u) && calls[0].rate == fast && calls[0].flags == 3, "5", f);
        reset(5); flag(f); rtti_c::write_word(a() + 972u, 9u); setState__6Cthului(object, 6);
        expect(log_text == "po" && calls[0].header == (set ? 148u : 28u) && calls[0].flags == 2056 && w(972) == 0u, "6", f);
        reset(5); flag(f); setState__6Cthului(object, 9); expect(log_text == "pt" && calls[0].header == (set ? 76u : 156u) && calls[0].rate == slow && calls[0].flags == 3, "9", f);
        reset(5); flag(f); rtti_c::write_u16(a() + 572u, 5); setState__6Cthului(object, 8);
        expect(set ? (log_text == "p" && half() == 5u) : (log_text == "po" && calls[0].header == 124 && calls[0].flags == 16 && half() == 8u), "8 flag", f);
    }
    // state 3: the +960 / +808 comparison
    reset(5); rtti_c::write_word(a() + 960u, 4999u); setState__6Cthului(object, 3);
    expect(log_text == "pztorg" && calls[0].header == 84 && calls[0].flags == 2051 && calls[1].header == 108 && calls[1].flags == 2056 && w(964) == 1u &&
           w(960) == 5000u + 1000u % 300u + 300u, "3 below, flag clear");
    reset(5); rtti_c::write_word(a() + 960u, 4999u); flag(1); setState__6Cthului(object, 3);
    expect(log_text == "pztrg" && calls[0].header == 60 && calls[0].flags == 2051 && w(964) == 0u, "3 below, flag set");
    reset(5); rtti_c::write_word(a() + 960u, 0xfffffff0u); setState__6Cthului(object, 3); expect(log_text == "pztorg", "3 negative +960 is below");
    reset(5); rtti_c::write_word(a() + 960u, 0x7fffffffu); rtti_c::write_word(a() + 808u, 0x80000000u); setState__6Cthului(object, 3); expect(log_text == "pztg", "3 signed comparison, +960 above");
    for (unsigned cur : {6u, 13u, 14u, 15u, 16u}) { reset(cur); rtti_c::write_word(a() + 960u, 5000u); setState__6Cthului(object, 3); expect(log_text == "pz" && half() == 3u, "3 blocked", cur); }
    for (unsigned cur : {0u, 3u, 5u, 9u, 12u, 17u}) {
        reset(cur); rtti_c::write_word(a() + 960u, 5000u); setState__6Cthului(object, 3);
        expect(log_text == "pztg" && calls[0].header == 60 && calls[0].flags == 2051, "3 allowed, flag clear", cur);
        reset(cur); rtti_c::write_word(a() + 960u, 5000u); flag(1); setState__6Cthului(object, 3);
        expect(log_text == "pztg" && calls[0].header == 84, "3 allowed, flag set", cur);
    }
    reset(9); setState__6Cthului(object, 8); expect(log_text == "p" && half() == 9u, "8 keeps 9");
    reset(6); rtti_c::write_u16(a() + 294u, 8u); setState__6Cthului(object, 8); expect(log_text == "p" && half() == 6u, "8 keeps 6 with bit 3");
    reset(6); rtti_c::write_u16(a() + 294u, 7u); setState__6Cthului(object, 8); expect(log_text == "po" && half() == 8u, "8 six without bit 3");
    reset(14); rtti_c::write_u16(a() + 294u, 8u); setState__6Cthului(object, 8); expect(log_text == "po", "8 does not keep 14");
    const struct { int state; unsigned header; int flags; unsigned w972; } shots[] = {{13, 36, 8, 0}, {14, 44, 8, 0}, {15, 52, 2056, 1}};
    for (const auto& c : shots) {
        reset(5); rtti_c::write_word(a() + 972u, 7u); setState__6Cthului(object, c.state);
        expect(log_text == "po" && calls[0].header == c.header && calls[0].rate == fast && calls[0].flags == c.flags && w(972) == c.w972, "one-shot state", c.state);
    }
    reset(5); rtti_c::write_word(a() + 972u, 7u); setState__6Cthului(object, 16);
    expect(log_text == "por" && calls[0].header == 92 && calls[0].flags == 2056 && w(972) == 0u && w(968) == 5000u + 1000u % 300u + 120u, "16");
    reset(5); for (unsigned i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0xa0 + i); rtti_c::write_u16(a() + 56u, 0xfff0u);
    setState__6Cthului(object, 10);
    expect(log_text == "pyts" && calls[0].header == 116 && calls[0].rate == slow && calls[0].flags == 6 && treasure_a == -16 && treasure_name == nullptr &&
           std::memcmp(point_copy, object + 16, 12) == 0 && slot_self == object - 8, "10");
    expect(treasure_bits[0] == 0x42100000u && treasure_bits[1] == 0x40000000u && treasure_bits[2] == 0x3fe66666u && treasure_bits[3] == 0x3ecccccc &&
           treasure_bits[4] == 0x40000000u && treasure_bits[5] == 0x3e800000u, "10 floats");
    for (int st : {2, 4, 7, 11, 12, 17, 100, -1}) { reset(5); setState__6Cthului(object, st); expect(log_text == "p" && half() == (unsigned(st) & 0xffffu), "store only", st); }
    reset(5); sp_result = 1; sp_new = 9; setState__6Cthului(object, 5); expect(log_text == "p" && half() == 5u, "processing result stops everything");
    reset(5); sp_new = 9; setState__6Cthului(object, 3); expect(log_text == "pt" && calls[0].header == 156 && calls[0].rate == slow && half() == 9u, "processing can replace the state");
    return bad ? 1 : 0;
}
