// Host contract for setState__5Demoni. Includes the source; not an EE input.
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
static int rolls[4], roll_n = 0, roll_i = 0, range_seen = 0;
static unsigned char headers[400], object[1300], vtable[256], table[400];
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
static rtti_c::Address a() { return rtti_c::address(object); }
static unsigned w(unsigned o) { return rtti_c::read_word(a() + o); }
static unsigned long long d(unsigned o) { return rtti_c::read_u64(a() + o); }
static unsigned half(unsigned o = 572u) { return rtti_c::read_u16(a() + o); }
static unsigned offset_of(void* h) { return static_cast<unsigned>(static_cast<unsigned char*>(h) - headers); }
static unsigned treasure_bits[6]; static int treasure_a = 0; static char* treasure_name = reinterpret_cast<char*>(1);
static unsigned char point_copy[12]; static unsigned w256_at_treasure = 0, w12_at_treasure = 0, w256_at_call = 0;
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
    log_text += "y"; std::memcpy(point_copy, point, 12); treasure_a = a_; treasure_name = name; w256_at_treasure = w(256); w12_at_treasure = w(12);
    treasure_bits[0] = bits_of(f12); treasure_bits[1] = bits_of(f13); treasure_bits[2] = bits_of(f14); treasure_bits[3] = bits_of(f15); treasure_bits[4] = bits_of(f16); treasure_bits[5] = bits_of(f17);
}
static void* slot_self = nullptr;
static void slot192(void* self) { log_text += "s"; slot_self = self; }
static void reset(unsigned cur) {
    std::memset(object, 0, sizeof object); std::memset(vtable, 0, sizeof vtable); std::memset(table, 0, sizeof table);
    rtti_c::host_note(table);
    for (unsigned off = 0; off < 160; off += 4) {
        rtti_c::host_note(headers + off);
        rtti_c::write_word(rtti_c::address(table) + off, rtti_c::address_bits(headers + off));
    }
    rtti_c::write_word(a() + 36u, rtti_c::address_bits(table));
    rtti_c::host_install_slot(object, vtable, 192u, -8, reinterpret_cast<void*>(slot192));
    rtti_c::write_word(a() + 808u, 5000u);
    rtti_c::write_u16(a() + 572u, cur);
    log_text.clear(); calls.clear(); roll_n = 2; rolls[0] = 1000; rolls[1] = 61; roll_i = 0; slot_self = nullptr;
}
static int bad = 0;
static void expect(bool ok, const char* msg, int x = 0, int y = 0) { if (!ok) { std::printf("fail: %s (%d, %d) log=%s\n", msg, x, y, log_text.c_str()); ++bad; } }
int main() {
    const unsigned fast = 0x3dccccccu, slow = 0x3e999999u;
    struct K { int state; const char* log; unsigned header; unsigned rate; int flags; };
    const K simple[] = {
        {5, "t", 52, fast, 3}, {6, "o", 28, fast, 8}, {9, "t", 92, fast, 3}, {16, "o", 36, fast, 8}, {17, "o", 44, fast, 8},
        {3, "ztg", 100, fast, 2051}};
    for (const K& c : simple) {
        reset(5); setState__5Demoni(object, c.state);
        expect(log_text == c.log && calls.size() == 1 && calls[0].header == c.header && calls[0].rate == c.rate && calls[0].flags == c.flags, "simple state", c.state);
        expect(half() == unsigned(c.state), "state stored", c.state);
    }
    reset(5); setState__5Demoni(object, 0);
    expect(log_text == "rt" && w(256) == 5000u + 1000u % 120u && w256_at_call == w(256) && calls[0].header == 68 && calls[0].rate == slow && calls[0].flags == 3, "state 0");
    reset(5); setState__5Demoni(object, 1);
    expect(log_text == "zdrtg" && range_seen == 200 && object[272] == 0x61 && object[283] == 0x6c && w(256) == 5000u + 1000u % 120u + 300u && w256_at_call == w(256) &&
           calls[0].header == 108 && calls[0].rate == slow && calls[0].flags == 2051, "state 1");
    for (unsigned cur : {6u, 8u}) { reset(cur); setState__5Demoni(object, 3); expect(log_text == "z" && half() == 3u, "3 blocked", cur); }
    for (unsigned cur : {0u, 3u, 5u, 9u, 13u}) { reset(cur); setState__5Demoni(object, 3); expect(log_text == "ztg", "3 allowed", cur); }
    reset(9); setState__5Demoni(object, 8); expect(log_text.empty() && half() == 9u, "8 keeps 9");
    reset(6); rtti_c::write_u16(a() + 294u, 8u); setState__5Demoni(object, 8); expect(log_text.empty() && half() == 6u, "8 keeps 6 with bit 3");
    reset(6); rtti_c::write_u16(a() + 294u, 7u); setState__5Demoni(object, 8); expect(log_text == "o" && calls[0].header == 60 && calls[0].flags == 16 && half() == 8u, "8 six without bit 3");
    reset(14); rtti_c::write_u16(a() + 294u, 8u); setState__5Demoni(object, 8); expect(log_text == "o" && half() == 8u, "8 does not keep 14");
    reset(5); setState__5Demoni(object, 13); expect(log_text == "to" && calls[0].header == 124 && calls[1].header == 116 && calls[0].rate == fast && calls[1].flags == 8, "13");
    reset(5); setState__5Demoni(object, 14);
    expect(log_text == "tr" && calls[0].header == 52 && w(256) == 5000u + 1000u % 60u + 60u && (d(160) & 0x200ull) != 0ull, "14");
    for (unsigned long long start : {0ull, 0x200ull, ~0ull}) {
        reset(5); rtti_c::write_u64(a() + 160u, start); setState__5Demoni(object, 5);
        expect(d(160) == (start & ~0x200ull), "bit 9 cleared on entry", int(start & 0xff));
        reset(5); rtti_c::write_u64(a() + 160u, start); setState__5Demoni(object, 99);
        expect(d(160) == (start & ~0x200ull), "bit 9 cleared even when only storing", int(start & 0xff));
    }
    reset(5); rolls[0] = 65536; setState__5Demoni(object, 15);
    expect(log_text == "tor" && calls[0].header == 84 && calls[1].header == 28 && w(256) == 5000u + 180u + 240u, "15");
    reset(5); rolls[0] = int(0xffff0000u); setState__5Demoni(object, 15); expect(w(256) == 5000u - 180u + 240u, "15 negative roll");
    reset(5); rtti_c::write_word(a() + 12u, 1u); for (unsigned i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0xa0 + i);
    rtti_c::write_u16(a() + 56u, 0xfff0u);
    setState__5Demoni(object, 10);
    expect(log_text == "yts" && w(12) == 0x4001u && w(256) == 5000u && w256_at_treasure == 5000u && w12_at_treasure == 0x4001u && calls[0].header == 92 && calls[0].rate == slow && calls[0].flags == 6, "10 order");
    expect(treasure_a == -16 && treasure_name == nullptr && std::memcmp(point_copy, object + 16, 12) == 0, "10 treasure args");
    expect(treasure_bits[0] == 0x42100000u && treasure_bits[1] == 0x40000000u && treasure_bits[2] == 0x3fe66666u && treasure_bits[3] == 0x3f199999u &&
           treasure_bits[4] == 0x40000000u && treasure_bits[5] == 0x3e800000u && slot_self == object - 8, "10 floats and slot");
    for (int st : {2, 4, 7, 11, 12, 18, 100, -1}) { reset(5); setState__5Demoni(object, st); expect(log_text.empty() && half() == (unsigned(st) & 0xffffu), "store only", st); }
    return bad ? 1 : 0;
}
