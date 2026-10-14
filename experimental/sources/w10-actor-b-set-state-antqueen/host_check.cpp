// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "set_state.cpp"

struct Call { char kind; unsigned header; unsigned a, b, c, d, e; int flags; };
static std::vector<Call> calls;
static std::string log_text;
static int processing_result, processing_new;
static unsigned rolls[2], roll_index;
static unsigned char record[256];
static unsigned char table[256];
static unsigned char object[1200];
static unsigned char enemy[64];
static int has_enemy, comp_result, heading_result;
static float heading_args[2], enemy_args[2];
static short enemy_angle, enemy_range;
static void* slot_self;
static unsigned char* current;
static unsigned point_at_treasure[3], treasure_ints[2];
static unsigned treasure_floats[6];
static void* treasure_name = reinterpret_cast<void*>(1);
static unsigned stamp_at_slot;

static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
static unsigned word(unsigned off) { unsigned v; std::memcpy(&v, object + off, 4); return v; }
static unsigned half(unsigned off) { unsigned short v; std::memcpy(&v, object + off, 2); return v; }
static unsigned header_offset(void* h) { return static_cast<unsigned>(static_cast<unsigned char*>(h) - record); }

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state) {
    log_text += self == object ? "p" : "p?";
    if (processing_new != -1000) *state = processing_new;
    return processing_result;
}
unsigned eRand__10GameObject(void*) { log_text += "r"; return rolls[roll_index++ % 2]; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags) {
    log_text += state == object + 288 ? "t" : "t?";
    calls.push_back({'t', header_offset(header), bits_of(rate), 0, 0, 0, 0, flags});
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags) {
    log_text += state == object + 288 ? "o" : "o?";
    calls.push_back({'o', header_offset(header), bits_of(a), bits_of(b), 0, 0, 0, flags});
}
void animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(void* state, void* header, float a, float b, float c, float d, float e, int flags) {
    log_text += state == object + 288 ? "h" : "h?";
    calls.push_back({'h', header_offset(header), bits_of(a), bits_of(b), bits_of(c), bits_of(d), bits_of(e), flags});
}
void* animGetAnimComponent__FP14AnimationStateP15AnimationHeader(void* state, void* header) {
    log_text += state == object + 288 && header_offset(header) == 60 ? "c" : "c?";
    return comp_result ? record : nullptr;
}
void resetRouting__8Creature(void*) { log_text += "z"; }
void getSmartRandomDestination__8Creaturei(float* result, void* self, int range) {
    log_text += self == object && range == 200 ? "d" : "d?";
    const unsigned words[3] = {0x7fc00001u, 0x3f800000u, 0xdeadbeefu};
    std::memcpy(result, words, 12);
}
void findEvadeDestination__8AntQueen(float* result, void* self) {
    log_text += self == object ? "e" : "e?";
    const unsigned words[3] = {0x11223344u, 0x7fc00002u, 0x99887766u};
    std::memcpy(result, words, 12);
}
void* alternateEnemyInZone__8Creatureffss(void* self, float a, float b, short angle, short range) {
    log_text += self == object ? "a" : "a?";
    enemy_args[0] = a; enemy_args[1] = b; enemy_angle = angle; enemy_range = range;
    return has_enemy ? enemy : nullptr;
}
int iatan2__Fff(float y, float x) { log_text += "i"; heading_args[0] = y; heading_args[1] = x; return heading_result; }
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const float* point, int angle, float a, float b, float c, float d, float e, float f, char* name) {
    log_text += self == object ? "y" : "y?";
    std::memcpy(point_at_treasure, point, 12);
    treasure_ints[0] = static_cast<unsigned>(angle);
    treasure_floats[0] = bits_of(a); treasure_floats[1] = bits_of(b); treasure_floats[2] = bits_of(c);
    treasure_floats[3] = bits_of(d); treasure_floats[4] = bits_of(e); treasure_floats[5] = bits_of(f);
    treasure_name = name;
}
}
static void slot192(void* self) { log_text += "s"; slot_self = self; }

static int failures;
static void expect(bool ok, const char* what, int a = 0, int b = 0) {
    if (!ok) { std::fprintf(stderr, "fail: %s (%d, %d) log=%s\n", what, a, b, log_text.c_str()); ++failures; }
}
static void put32(unsigned off, unsigned v) { std::memcpy(object + off, &v, 4); }
static void put16(unsigned off, unsigned v) { unsigned short s = static_cast<unsigned short>(v); std::memcpy(object + off, &s, 2); }
static void reset(unsigned current_state) {
    std::memset(object, 0x3c, sizeof object);
    std::memset(record, 0, sizeof record);
    std::memset(table, 0, sizeof table);
    for (unsigned off = 0; off < 200; off += 4) rtti_c::host_note(record + off);
    for (unsigned off = 0; off < 200; off += 4) {
        const unsigned bits = rtti_c::address_bits(record + off);
        std::memcpy(table + off, &bits, 4);
    }
    rtti_c::host_note(table);
    put32(36, rtti_c::address_bits(table));
    put16(572, current_state);
    put32(808, 5000);
    static unsigned char vtable[256];
    std::memset(vtable, 0, sizeof vtable);
    rtti_c::host_note(vtable);
    put32(72, rtti_c::address_bits(vtable));
    std::memset(vtable + 192, 0, 2);
    const unsigned fn = rtti_c::address_bits(reinterpret_cast<void*>(slot192));
    rtti_c::host_note(reinterpret_cast<void*>(slot192));
    std::memcpy(vtable + 196, &fn, 4);
    rtti_c::host_note(enemy);
    log_text.clear(); calls.clear();
    processing_result = 0; processing_new = -1000;
    rolls[0] = 1000; rolls[1] = 0xfffffff0u; roll_index = 0;
    has_enemy = 0; comp_result = 0; heading_result = 0;
    current = object; slot_self = nullptr; treasure_name = reinterpret_cast<void*>(1);
}

int main() {
    const unsigned s3 = 0x3e999999u, s1 = 0x3dccccccu, third = 0x3eaaaaabu, f_one = 0x3f800000u, f_zero = 0;
    // Early return, replaced state, plain stores.
    reset(7); processing_result = 1; processing_new = 3; setState__8AntQueeni(object, 0);
    expect(log_text == "p" && half(572) == 7u && word(256) == 0x3c3c3c3cu, "nonzero processing result ends the call");
    reset(7); processing_new = 5; setState__8AntQueeni(object, 0);
    expect(log_text == "pt" && half(572) == 5u && calls[0].header == 132 && calls[0].a == s1, "processing can replace the state");
    for (int state : {2, 4, 7, 11, 12, 20, 21, 1000, -1}) {
        reset(9); setState__8AntQueeni(object, state);
        expect(log_text == "p" && half(572) == (static_cast<unsigned>(state) & 0xffffu) && word(256) == 0x3c3c3c3cu, "state only", state);
    }
    // 0
    reset(9); setState__8AntQueeni(object, 0);
    expect(log_text == "prt" && word(256) == 5000u + 1000u % 120u && calls[0].header == 124 && calls[0].a == s3 && calls[0].flags == 3 && half(572) == 0u, "state 0");
    reset(9); rolls[0] = 0xfffffff0u; setState__8AntQueeni(object, 0);
    expect(word(256) == 5000u + 0xfffffff0u % 120u, "state 0 remainder is unsigned");
    // 1
    reset(9); setState__8AntQueeni(object, 1);
    expect(log_text == "pzdt" && word(272) == 0x7fc00001u && word(276) == 0x3f800000u && word(280) == 0xdeadbeefu && word(176) == bits_of(140.0f), "state 1 stores");
    expect(calls[0].header == 172 && calls[0].a == s3 && calls[0].flags == 3, "state 1 transition");
    // 3
    for (unsigned cur : {8u, 6u, 19u, 0u, 5u, 0xffffu, 7u}) {
        reset(cur); put32(160, 0x00000001u); put32(164, 0xa5a5a5a5u);
        setState__8AntQueeni(object, 3);
        const bool skip = cur == 8 || cur == 6 || cur == 19;
        expect(log_text == (skip ? "p" : "pzhhh"), "state 3 sequence", cur);
        expect(word(160) == 0x10000001u && word(164) == 0xa5a5a5a5u, "state 3 flag", cur);
        if (!skip) {
            expect(word(176) == bits_of(140.0f), "state 3 +176", cur);
            expect(calls[0].header == 172 && calls[0].a == f_zero && calls[0].b == f_zero && calls[0].c == third && calls[0].d == f_one && calls[0].e == f_zero && calls[0].flags == 515, "state 3 first hairy", cur);
            expect(calls[1].header == 180 && calls[1].c == f_zero && calls[1].d == f_one && calls[1].flags == 513 && calls[2].header == 188 && calls[2].c == f_zero && calls[2].flags == 513, "state 3 other hairy", cur);
        } else expect(word(176) == 0x3c3c3c3cu, "state 3 untouched +176", cur);
    }
    reset(0); setState__8AntQueeni(object, 5); expect(log_text == "pt" && calls[0].header == 132 && calls[0].a == s1 && calls[0].flags == 3, "state 5");
    reset(0); setState__8AntQueeni(object, 9); expect(log_text == "pt" && calls[0].header == 124 && calls[0].a == s3 && calls[0].flags == 3, "state 9");
    for (int state : {6, 15, 16}) {
        reset(0); setState__8AntQueeni(object, state);
        expect(log_text == "po" && calls[0].header == (state == 6 ? 28u : 156u) && calls[0].a == s1 && calls[0].b == s1 && calls[0].flags == 8 && word(964) == 0u && half(572) == static_cast<unsigned>(state), "plain one-shots", state);
    }
    // 8
    for (unsigned cur : {13u, 14u, 15u, 9u}) { reset(cur); setState__8AntQueeni(object, 8); expect(log_text == "p" && half(572) == cur, "state 8 keeps", cur); }
    for (unsigned bit : {0u, 8u, 0xfff7u, 0xffffu}) {
        reset(6); put16(294, bit); put16(748, 0);
        setState__8AntQueeni(object, 8);
        if (bit & 8u) expect(log_text == "p" && half(572) == 6u, "state 8 keeps 6 with the bit", bit);
        else expect(log_text == "po" && calls[0].header == 84 && half(572) == 8u, "state 8 falls through from 6", bit);
    }
    struct Range { unsigned angle; unsigned header; };
    const Range ranges[] = {{0, 84}, {8191, 84}, {0xe001, 84}, {8192, 92}, {8193, 108}, {24575, 108}, {24576, 92}, {0x8001, 92}, {0xa000, 92}, {0xdfff, 100}, {0xe000, 92}, {0xa001, 100}, {0x9fff, 92}, {0xffff, 84}};
    for (const Range& r : ranges) {
        reset(0); put16(748, r.angle); setState__8AntQueeni(object, 8);
        expect(log_text == "po" && calls[0].header == r.header && calls[0].a == s1 && calls[0].b == s1 && calls[0].flags == 16 && half(572) == 8u, "state 8 by angle", r.angle);
    }
    // 10
    reset(0); put32(16, 0x11111111u); put32(20, 0x22222222u); put32(24, 0x33333333u); put16(56, 0xfff0u);
    setState__8AntQueeni(object, 10);
    expect(log_text == "pytcos" && point_at_treasure[0] == 0x11111111u && point_at_treasure[1] == 0x22222222u && point_at_treasure[2] == 0x33333333u &&
           static_cast<int>(treasure_ints[0]) == -16 && treasure_name == nullptr, "state 10 treasure");
    expect(treasure_floats[0] == bits_of(36.0f) && treasure_floats[1] == bits_of(2.0f) && treasure_floats[2] == bits_of(5.0f) && treasure_floats[3] == bits_of(1.0f) && treasure_floats[4] == bits_of(5.0f) && treasure_floats[5] == bits_of(1.0f), "state 10 floats");
    expect(calls[0].header == 196 && calls[0].a == 0x3ecccccc && calls[0].flags == 22 && calls[1].header == 164 && calls[1].a == s1 && calls[1].flags == 8 && slot_self == object, "state 10 animations");
    reset(0); comp_result = 1; setState__8AntQueeni(object, 10); expect(log_text == "pytcs" && half(572) == 10u, "state 10 with the component present");
    // 13, 14
    for (unsigned cur : {14u, 15u, 0u, 13u, 16u}) {
        reset(cur); setState__8AntQueeni(object, 13);
        const bool skip = cur == 14 || cur == 15;
        expect(log_text == std::string("prze") + (skip ? "" : "hhho"), "state 13 sequence", cur);
        expect(word(176) == bits_of(240.0f) && word(972) == 5000u + 1000u % 120u + 180u && word(272) == 0x11223344u && word(276) == 0x7fc00002u && word(280) == 0x99887766u, "state 13 stores", cur);
        if (!skip) expect(calls[0].header == 60 && calls[0].c == third && calls[0].flags == 515 && calls[1].header == 68 && calls[2].header == 76 && calls[3].header == 164 && calls[3].a == s1 && calls[3].flags == 8, "state 13 animations", cur);
    }
    for (unsigned cur : {13u, 15u, 0u, 14u, 16u}) {
        reset(cur); setState__8AntQueeni(object, 14);
        const bool skip = cur == 13 || cur == 15;
        expect(log_text == std::string("pz") + (skip ? "" : "rhhho"), "state 14 sequence", cur);
        expect(word(176) == bits_of(240.0f), "state 14 +176", cur);
        if (!skip) expect(word(972) == 5000u + 1000u % 180u + 300u && calls[0].header == 60 && calls[3].header == 164, "state 14 animations", cur);
        else expect(word(972) == 0x3c3c3c3cu, "state 14 leaves +972", cur);
    }
    reset(0); setState__8AntQueeni(object, 17);
    expect(log_text == "prto" && word(972) == 5000u + 1000u % 180u + 180u && calls[0].header == 132 && calls[0].a == s1 && calls[1].header == 148 && calls[1].flags == 8, "state 17");
    reset(0); setState__8AntQueeni(object, 18);
    expect(log_text == "prtor" && word(976) == 5000u + 1000u % 180u + 180u && word(256) == 5000u + 0xfffffff0u % 60u + 120u && calls[0].header == 132 && calls[1].header == 140, "state 18");
    // 19
    reset(0); put32(576, 0); setState__8AntQueeni(object, 19); expect(log_text == "p" && half(572) == 19u, "state 19 needs the member");
    struct Heading { int offset; unsigned header; };
    const Heading headings[] = {{2000, 36}, {2001, 52}, {-2000, 36}, {-2001, 44}, {0, 36}, {5000, 52}, {-5000, 44}};
    for (const Heading& h : headings) {
        reset(0); put32(576, 1); put16(56, 0x4000u); put32(28, bits_of(10.0f));
        float v = 3.0f; std::memcpy(object + 16, &v, 4); v = 4.0f; std::memcpy(object + 20, &v, 4);
        v = 13.0f; std::memcpy(enemy + 16, &v, 4); v = 24.0f; std::memcpy(enemy + 20, &v, 4);
        has_enemy = 1; heading_result = 0x4000 + h.offset - 32767;
        setState__8AntQueeni(object, 19);
        expect(log_text == "paio" && calls[0].header == h.header && calls[0].a == s1 && calls[0].flags == 8, "state 19 headings", h.offset);
        expect(enemy_args[0] == 0.0f && enemy_args[1] == 50.0f && enemy_angle == static_cast<short>(0x4000 - 32767) && enemy_range == 6000, "enemy query", h.offset);
        expect(heading_args[0] == 20.0f && heading_args[1] == 10.0f, "heading arguments", h.offset);
    }
    reset(0); put32(576, 1); setState__8AntQueeni(object, 19); expect(log_text == "pao" && calls[0].header == 36, "state 19 without an enemy");
    return failures ? 1 : 0;
}
