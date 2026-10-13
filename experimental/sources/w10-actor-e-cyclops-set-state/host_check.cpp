// Host contract for setState__7Cyclopsi. Includes the source; not an EE input.
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
static int sp_result = 0, sp_new = -1000, rolls[4], roll_n = 0, roll_i = 0, heading_value = 0, range_seen = 0;
static unsigned char headers[400], object[1300], vtable[256], table[400], target[32], inner[48];
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
static rtti_c::Address a() { return rtti_c::address(object); }
static unsigned w(unsigned o) { return rtti_c::read_word(a() + o); }
static unsigned half(unsigned o = 572u) { return rtti_c::read_u16(a() + o); }
static unsigned offset_of(void* h) { return static_cast<unsigned>(static_cast<unsigned char*>(h) - headers); }
static unsigned treasure_bits[6], atan_bits[2], treasure_w256 = 0; static int treasure_a = 0; static char* treasure_name = reinterpret_cast<char*>(1);
static unsigned char point_copy[12]; static unsigned byte40_at_treasure = 0, w256_at_call = 0;
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
extern "C" int iatan2__Fff(float y, float x) { log_text += "a"; atan_bits[0] = bits_of(y); atan_bits[1] = bits_of(x); return heading_value; }
extern "C" void dropTreasure__8CreatureG6Point3iffffffPc(void*, const void* point, int a_, float f12, float f13, float f14, float f15, float f16, float f17, char* name) {
    log_text += "y"; std::memcpy(point_copy, point, 12); treasure_a = a_; treasure_name = name; byte40_at_treasure = object[40];
    treasure_bits[0] = bits_of(f12); treasure_bits[1] = bits_of(f13); treasure_bits[2] = bits_of(f14); treasure_bits[3] = bits_of(f15); treasure_bits[4] = bits_of(f16); treasure_bits[5] = bits_of(f17);
}
static void* slot_self = nullptr;
static void slot192(void* self) { log_text += "s"; slot_self = self; }
static void reset(unsigned cur) {
    std::memset(object, 0, sizeof object); std::memset(vtable, 0, sizeof vtable); std::memset(table, 0, sizeof table);
    std::memset(target, 0, sizeof target); std::memset(inner, 0, sizeof inner);
    rtti_c::host_note(table);
    for (unsigned off = 0; off < 160; off += 4) {
        rtti_c::host_note(headers + off);
        rtti_c::write_word(rtti_c::address(table) + off, rtti_c::address_bits(headers + off));
    }
    rtti_c::write_word(a() + 36u, rtti_c::address_bits(table));
    rtti_c::host_install_slot(object, vtable, 192u, -8, reinterpret_cast<void*>(slot192));
    rtti_c::write_word(a() + 808u, 5000u);
    rtti_c::write_u16(a() + 572u, cur);
    log_text.clear(); calls.clear(); sp_result = 0; sp_new = -1000; roll_n = 2; rolls[0] = 1000; rolls[1] = 61; roll_i = 0; slot_self = nullptr;
}
static int bad = 0;
static void expect(bool ok, const char* msg, int x = 0, int y = 0) { if (!ok) { std::printf("fail: %s (%d, %d) log=%s\n", msg, x, y, log_text.c_str()); ++bad; } }
int main() {
    const unsigned fast = 0x3dccccccu, slow = 0x3e999999u;
    struct K { int state; const char* log; unsigned header; unsigned rate; int flags; bool shot; };
    const K simple[] = {
        {5, "pt", 100, fast, 3, false}, {6, "po", 28, fast, 8, true}, {9, "pt", 108, slow, 3, false}, {14, "po", 68, fast, 8, true},
        {15, "po", 76, fast, 8, true}, {2, "pztg", 132, slow, 2051, false}, {1, "pzdtg", 132, slow, 2051, false},
        {7, "pztg", 132, slow, 2051, false}};
    for (const K& c : simple) {
        reset(5); sp_result = 0; setState__7Cyclopsi(object, c.state);
        expect(log_text == c.log && calls.size() == 1 && calls[0].header == c.header && calls[0].rate == c.rate && calls[0].flags == c.flags, "simple state", c.state);
        expect(half() == unsigned(c.state), "state stored", c.state);
    }
    reset(5); setState__7Cyclopsi(object, 1);
    expect(range_seen == 300 && object[272] == 0x61 && object[283] == 0x6c && w(176) == 0u, "state 1 destination, range and +176");
    reset(5); rtti_c::write_word(a() + 176u, 77u); setState__7Cyclopsi(object, 2); expect(w(176) == 0u && w(256) == 0u, "state 2 clears +176 only");
    reset(5); rtti_c::write_word(a() + 176u, 77u); setState__7Cyclopsi(object, 7);
    expect(w(176) == 0u && w(256) == 5060u, "state 7 clears +176 and sets +256");
    reset(5); setState__7Cyclopsi(object, 0);
    expect(log_text == "prt" && w(256) == 5000u + 1000u % 60u + 120u && w256_at_call == w(256) && calls[0].header == 108 && calls[0].rate == slow && calls[0].flags == 3, "state 0");
    for (unsigned cur : {3u, 6u, 13u, 14u, 15u, 16u}) { reset(cur); setState__7Cyclopsi(object, 3); expect(log_text == "pz" && half() == 3u, "3 blocked", cur); }
    for (unsigned cur : {0u, 5u, 9u, 12u, 17u}) { reset(cur); setState__7Cyclopsi(object, 3); expect(log_text == "pztg" && calls[0].header == 132 && calls[0].flags == 2051, "3 allowed", cur); }
    reset(9); setState__7Cyclopsi(object, 8); expect(log_text == "p" && half() == 9u, "8 keeps 9");
    reset(6); rtti_c::write_u16(a() + 294u, 8u); setState__7Cyclopsi(object, 8); expect(log_text == "p" && half() == 6u, "8 keeps 6 with bit 3");
    reset(6); rtti_c::write_u16(a() + 294u, 7u); setState__7Cyclopsi(object, 8); expect(log_text == "po" && calls[0].header == 92 && calls[0].flags == 16 && half() == 8u, "8 six without bit 3");
    reset(14); rtti_c::write_u16(a() + 294u, 8u); setState__7Cyclopsi(object, 8); expect(log_text == "po" && half() == 8u, "8 does not keep 14");
    reset(5); setState__7Cyclopsi(object, 16); expect(log_text == "po" && calls[0].header == 36 && w(960) == 5600u && half() == 16u, "16");
    reset(5); for (unsigned i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0xa0 + i); rtti_c::write_u16(a() + 56u, 0xfff0u);
    setState__7Cyclopsi(object, 10);
    expect(log_text == "ptys" && object[40] == 120 && byte40_at_treasure == 120 && calls[0].header == 148 && calls[0].rate == slow && calls[0].flags == 6, "10 order");
    expect(treasure_a == -16 && treasure_name == nullptr && std::memcmp(point_copy, object + 16, 12) == 0, "10 treasure args");
    expect(treasure_bits[0] == 0x42100000u && treasure_bits[1] == 0x40000000u && treasure_bits[2] == 0x3fcccccc && treasure_bits[3] == 0x3f199999u &&
           treasure_bits[4] == 0x3fcccccc && treasure_bits[5] == 0x3e800000u && slot_self == object - 8, "10 floats and slot");
    for (int st : {4, 11, 12, 17, 18, 100, -1}) { reset(5); setState__7Cyclopsi(object, st); expect(log_text == "p" && half() == (unsigned(st) & 0xffffu), "store only", st); }
    reset(5); sp_result = 1; sp_new = 9; setState__7Cyclopsi(object, 5); expect(log_text == "p" && half() == 5u, "processing result stops everything");
    reset(5); sp_new = 9; setState__7Cyclopsi(object, 3); expect(log_text == "pt" && calls[0].header == 108 && half() == 9u, "processing can replace the state");
    // 13: target chain and heading comparison
    rtti_c::host_note(target); rtti_c::host_note(inner);
    for (int h : {0x1234 + 1, 0x1234, 0x1234 - 1, 0x1234 + 0x7fff, 0x1234 + 0x8000, 0x1234 - 0x8000}) {
        reset(5); rtti_c::write_u16(a() + 56u, 0x1234u);
        rtti_c::write_word(a() + 576u, rtti_c::address_bits(target)); rtti_c::write_word(rtti_c::address(target) + 12u, rtti_c::address_bits(inner));
        rtti_c::write_single(rtti_c::address(inner) + 16u, 30.0f); rtti_c::write_single(rtti_c::address(inner) + 20u, 40.0f);
        rtti_c::write_single(a() + 16u, 5.0f); rtti_c::write_single(a() + 20u, 8.0f);
        heading_value = h; setState__7Cyclopsi(object, 13);
        const bool positive = static_cast<short>(h - 0x1234) > 0;
        expect(log_text == "pao" && atan_bits[0] == bits_of(32.0f) && atan_bits[1] == bits_of(25.0f) && calls[0].header == (positive ? 44u : 52u) && calls[0].flags == 8 && calls[0].rate == fast, "13", h);
    }
    reset(5); setState__7Cyclopsi(object, 13); expect(log_text == "p" && half() == 13u, "13 without target");
    return bad ? 1 : 0;
}
