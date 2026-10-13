// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "set_state.cpp"

struct Call { char kind; unsigned header; float rate; float second; int flags; };
static std::vector<Call> calls;
static std::string log_text;
static int processing_result, processing_new;
static unsigned rolls[2], roll_index;
static unsigned char record[256];
static unsigned char object[1200];
static unsigned char table[256];
static unsigned stamp_960, stamp_256;
static void* slot_self;
static unsigned char* current;

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state) {
    log_text += self == object ? "p" : "p?";
    if (processing_new != -1000) *state = processing_new;
    return processing_result;
}
unsigned eRand__10GameObject(void*) { log_text += "r"; return rolls[roll_index++ % 2]; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags) {
    log_text += state == object + 288 ? "t" : "t?";
    calls.push_back({'t', static_cast<unsigned>(static_cast<unsigned char*>(header) - record), rate, rate, flags});
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags) {
    log_text += state == object + 288 ? "o" : "o?";
    calls.push_back({'o', static_cast<unsigned>(static_cast<unsigned char*>(header) - record), a, b, flags});
}
void animGlobalSync__FP14AnimationState(void* state) { log_text += state == object + 288 ? "g" : "g?"; }
void resetRouting__8Creature(void*) { log_text += "z"; }
void getSmartRandomDestination__8Creaturei(float* result, void* self, int range) {
    log_text += self == object && range == 200 ? "d" : "d?";
    const unsigned words[3] = {0x7fc00001u, 0x3f800000u, 0xdeadbeefu};
    std::memcpy(result, words, 12);
}
}
static void slot192(void* self) { log_text += "s"; slot_self = self; std::memcpy(&stamp_960, current + 960, 4); }

static unsigned word(unsigned off) { unsigned v; std::memcpy(&v, object + off, 4); return v; }
static unsigned half(unsigned off) { unsigned short v; std::memcpy(&v, object + off, 2); return v; }
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
static int failures;
static void expect(bool ok, const char* what, int a = 0, int b = 0) {
    if (!ok) { std::fprintf(stderr, "fail: %s (%d, %d) log=%s\n", what, a, b, log_text.c_str()); ++failures; }
}
static void reset(unsigned current_state) {
    std::memset(object, 0x3c, sizeof object);
    std::memset(record, 0, sizeof record);
    std::memset(table, 0, sizeof table);
    // The record at +36 is a table of header pointers: the word at offset k addresses record+k.
    for (unsigned off = 0; off < 200; off += 4) rtti_c::host_note(record + off);
    unsigned record_bits = rtti_c::address_bits(table);
    rtti_c::host_note(table);
    for (unsigned off = 0; off < 200; off += 4) {
        const unsigned bits = rtti_c::address_bits(record + off);
        std::memcpy(table + off, &bits, 4);
    }
    std::memcpy(object + 36, &record_bits, 4);
    unsigned short cs = static_cast<unsigned short>(current_state);
    std::memcpy(object + 572, &cs, 2);
    const unsigned base = 5000;
    std::memcpy(object + 808, &base, 4);
    static unsigned char vtable[256];
    std::memset(vtable, 0, sizeof vtable);
    rtti_c::host_note(vtable);
    const unsigned vt_bits = rtti_c::address_bits(vtable);
    std::memcpy(object + 72, &vt_bits, 4);
    const unsigned short adj = 0;
    std::memcpy(vtable + 192, &adj, 2);
    const unsigned fn = rtti_c::address_bits(reinterpret_cast<void*>(slot192));
    rtti_c::host_note(reinterpret_cast<void*>(slot192));
    std::memcpy(vtable + 196, &fn, 4);
    log_text.clear(); calls.clear();
    processing_result = 0; processing_new = -1000;
    rolls[0] = 1000; rolls[1] = 0xfffffff0u; roll_index = 0;
    current = object; slot_self = nullptr; stamp_960 = 0;
}

int main() {
    const unsigned slow = 0x3e999999u, fast = 0x3dccccccu, half_rate = 0x3f000000u;
    // Early return and untouched state.
    reset(7); processing_result = 1; processing_new = 3; setState__10Arenabeasti(object, 0);
    expect(log_text == "p" && half(572) == 7u && word(256) == 0x3c3c3c3cu, "nonzero processing result ends the call");
    // A new state chosen by setStateProcessing is the one handled.
    reset(7); processing_new = 5; setState__10Arenabeasti(object, 0);
    expect(log_text == "pt" && half(572) == 5u && bits_of(calls[0].rate) == fast, "processing can replace the state");
    // Plain stores.
    for (int state : {2, 4, 7, 11, 12, 24, 25, 1000, -1}) {
        reset(9); setState__10Arenabeasti(object, state);
        expect(log_text == "p" && half(572) == (static_cast<unsigned>(state) & 0xffffu) && word(256) == 0x3c3c3c3cu, "state only", state);
    }
    reset(0x1234); setState__10Arenabeasti(object, 8); expect(log_text == "p" && half(572) == 0x1234u, "state 8 keeps the current one");
    reset(0xfff0); setState__10Arenabeasti(object, 8); expect(log_text == "p" && half(572) == 0xfff0u, "state 8 with a negative current state");
    // State 0.
    reset(9); setState__10Arenabeasti(object, 0);
    expect(log_text == "prt" && word(256) == 5000u + 1000u % 120u && calls[0].header == 140 && bits_of(calls[0].rate) == slow && calls[0].flags == 3 && half(572) == 0u, "state 0");
    reset(9); rolls[0] = 0xfffffff0u; setState__10Arenabeasti(object, 0);
    expect(word(256) == 5000u + 0xfffffff0u % 120u, "state 0 uses an unsigned remainder");
    // State 1.
    reset(9); setState__10Arenabeasti(object, 1);
    expect(log_text == "pzdrtg" && word(272) == 0x7fc00001u && word(276) == 0x3f800000u && word(280) == 0xdeadbeefu, "state 1 destination bits");
    expect(word(256) == 5000u + 1000u % 120u + 300u && calls[0].header == 164 && bits_of(calls[0].rate) == slow && calls[0].flags == 2051, "state 1 transition");
    // State 3.
    for (unsigned cur : {3u, 6u, 18u, 14u, 15u, 0u, 5u, 0xffffu, 16u}) {
        reset(cur); setState__10Arenabeasti(object, 3);
        const bool skip = cur == 3 || cur == 6 || cur == 18 || cur == 14 || cur == 15;
        expect(log_text == (skip ? "pzg" : "pztg"), "state 3 sequence", cur);
        if (!skip) expect(calls[0].header == 164 && bits_of(calls[0].rate) == half_rate && calls[0].flags == 2051, "state 3 transition", cur);
        expect(half(572) == 3u, "state 3 result", cur);
    }
    for (int state : {5, 9}) { reset(0); setState__10Arenabeasti(object, state); expect(log_text == "pt" && calls[0].header == 140 && bits_of(calls[0].rate) == fast && calls[0].flags == 3 && half(572) == static_cast<unsigned>(state), "states 5/9", state); }
    // One-shot groups.
    struct OneShot { int state; unsigned header; unsigned rate; int flags; bool counter; bool tail; };
    const OneShot shots[] = {{15, 44, half_rate, 8, true, false}, {14, 68, half_rate, 8, true, false}, {18, 52, half_rate, 8, true, false}, {6, 60, half_rate, 8, true, false},
                             {16, 100, half_rate, 8, true, false}, {17, 84, half_rate, 8, true, false}, {13, 92, half_rate, 8, true, false},
                             {22, 28, half_rate, 2056, true, true}, {23, 36, half_rate, 2056, true, true}, {21, 76, half_rate, 2056, true, true},
                             {19, 148, fast, 16, false, false}, {20, 156, fast, 16, false, false}};
    for (const OneShot& s : shots) {
        reset(0); setState__10Arenabeasti(object, s.state);
        const bool is20 = s.state == 20;
        expect(log_text == std::string("po") + (s.counter || is20 ? "r" : ""), "one-shot log", s.state);
        expect(calls.size() == 1 && calls[0].header == s.header && bits_of(calls[0].rate) == s.rate && bits_of(calls[0].second) == s.rate && calls[0].flags == s.flags, "one-shot arguments", s.state);
        if (s.counter) {
            expect(word(960) == static_cast<unsigned>(s.state) && word(256) == 1000u % 20u, "counter and state copy", s.state);
            if (s.tail) expect(word(968) == 0u && word(972) == 0u, "tail zeros", s.state);
        }
        if (is20) expect(word(964) == 5000u + 1000u % 300u + 300u, "state 20 timer");
        expect(half(572) == static_cast<unsigned>(s.state), "result", s.state);
    }
    // State 10.
    reset(0); setState__10Arenabeasti(object, 10);
    expect(log_text == "pts" && calls[0].header == 108 && bits_of(calls[0].rate) == slow && calls[0].flags == 6 && slot_self == object && half(572) == 10u, "state 10");
    return failures ? 1 : 0;
}
