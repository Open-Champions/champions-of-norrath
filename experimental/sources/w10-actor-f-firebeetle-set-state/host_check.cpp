// Host contract for setState__10FireBeetlei. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "set_state.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

static std::string log_text;
static unsigned rolls[4];
static unsigned roll_index;
static int processing_result;
static int processing_set_word;      // when >= 0 the processing call changes the word to this
static bool processing_change;
static unsigned long long u64_at_call;
static unsigned char object[1300], group[256], table[256];
static void* death_adjusted;
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static void add(const char* t) { log_text += t; log_text += ';'; }
static std::string num(unsigned v) { char b[16]; std::snprintf(b, sizeof b, "%u", v); return b; }
static std::string header_name(void* h) {
    const unsigned char* p = static_cast<unsigned char*>(h);
    return num(static_cast<unsigned>(p - group));
}
static std::string rate(float f) { unsigned b; std::memcpy(&b, &f, 4); char t[16]; std::snprintf(t, sizeof t, "%08x", b); return t; }
extern "C" int setStateProcessing__8CreaturePi(void* self, int* state) {
    add("proc");
    expect(self == object, "processing object");
    if (processing_change) *state = processing_set_word;
    return processing_result;
}
extern "C" unsigned eRand__10GameObject(void*) { add("rand"); return rolls[roll_index++ % 4]; }
extern "C" void resetRouting__8Creature(void*) { add("reset"); }
extern "C" void getSmartRandomDestination__8Creaturei(void* out, void* self, int range) {
    add(("dest" + num(range)).c_str()); expect(self == object, "dest self");
    const float v[3] = {7.5f, 8.5f, 9.5f}; std::memcpy(out, v, 12);
}
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float r, int flags) {
    expect(state == object + 288, "transition state");
    u64_at_call = vslots_b::read_u64(vslots_b::address(object) + 160u);
    add(("T" + header_name(header) + "/" + rate(r) + "/" + num(flags)).c_str());
}
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags) {
    expect(state == object + 288, "one-shot state");
    add(("O" + header_name(header) + "/" + rate(a) + "/" + rate(b) + "/" + num(flags)).c_str());
}
extern "C" void animSetToEnd__FP14AnimationState(void* s) { expect(s == object + 288, "toend state"); add("end"); }
extern "C" void loseBodyPart__10FireBeetlei(void*, int part) { add(("lose" + num(part)).c_str()); }
extern "C" void dropTreasure__8CreatureG6Point3iffffffPc(void*, const void* p, int angle, float f12, float f13, float f14,
                                                         float f15, float f16, float f17, char* name) {
    float pt[3]; std::memcpy(pt, p, 12);
    add(("drop" + std::to_string(pt[0]) + "," + std::to_string(pt[2]) + "/" + num(angle & 0xffff) + "/" + rate(f12) + "/" + rate(f13) + "/" + rate(f14) +
         "/" + rate(f15) + "/" + rate(f16) + "/" + rate(f17) + "/" + name).c_str());
}
static void death_function(void* adjusted, int) { add("death"); death_adjusted = adjusted; }

static unsigned getw(unsigned off) { return vslots_b::read_word(vslots_b::address(object) + off); }
static unsigned getw16(unsigned off) { return vslots_b::read_u16(vslots_b::address(object) + off); }
static void setw(unsigned off, unsigned v) { vslots_b::write_word(vslots_b::address(object) + off, v); }
static void setup(int current) {
    std::memset(object, 0x41, sizeof object);
    for (unsigned i = 0; i < sizeof group; ++i) group[i] = 0;
    setw(36, vslots_b::address_bits(group));
    // each header slot of the group holds the address of group + (offset * 2 + 1) so names are unique
    for (unsigned off = 0; off + 4 <= 160; off += 4) vslots_b::write_word(vslots_b::address(group) + off, vslots_b::address_bits(group + 128 + off / 4));
    vslots_b::write_u16(vslots_b::address(object) + 572u, static_cast<unsigned>(current) & 0xffffu);
    setw(808, 1000);
    setw(12, 0);
    vslots_b::write_u64(vslots_b::address(object) + 160u, 0xffffffffffffffffull);
    setw(72, vslots_b::address_bits(table));
    vslots_b::write_u16(vslots_b::address(table) + 192u, 8u);
    setw(0, 0);
    vslots_b::write_word(vslots_b::address(table) + 196u, vslots_b::address_bits(reinterpret_cast<void*>(death_function)));
    const float pos[3] = {1.5f, 2.5f, 3.5f}; std::memcpy(object + 16, pos, 12);
    vslots_b::write_u16(vslots_b::address(object) + 56u, 0x21u);
    log_text.clear(); roll_index = 0; processing_result = 0; processing_change = false;
    vslots_b::write_u16(vslots_b::address(object) + 294u, 0u);
}
// the header name for offset n is group + 128 + n/4 -> its index relative to the group base
static std::string H(unsigned off) { return num(128 + off / 4); }
static const char* A = "3f000000";   // 0.5
static const char* B = "3f800000";   // 1.0
static const char* C = "3c23d70a";   // 0.01
static const char* D = "3dcccccc";   // 0.1
static const char* E = "40000000";   // 2.0
static std::string T(unsigned off, const char* r, int flags) { return "T" + H(off) + "/" + r + "/" + num(flags) + ";"; }
static std::string O(unsigned off, const char* a, const char* b, int flags) { return "O" + H(off) + "/" + a + "/" + b + "/" + num(flags) + ";"; }
static void run(int current, int state, const std::string& want, const char* name, int want_state = -999) {
    setup(current);
    setw(256, 0xdeadbeef);
    setState__10FireBeetlei(object, state);
    if (log_text != "proc;" + want) { std::fprintf(stderr, "%s\n  got  %s\n  want %s\n", name, log_text.c_str(), ("proc;" + want).c_str()); ++failures; }
    if (want_state != -999) expect(static_cast<int>(getw16(572)) == (want_state & 0xffff), name);
}
int main() {
    using namespace vslots_b;
    host_note(group); host_note(table); host_note(object); host_note(reinterpret_cast<void*>(death_function));
    for (unsigned off = 0; off < 40; ++off) host_note(group + 128 + off);
    rolls[0] = 7; rolls[1] = 61; rolls[2] = 5; rolls[3] = 1;
    // processing result non-zero: nothing stored
    setup(9); processing_result = 1; setState__10FireBeetlei(object, 5);
    expect(log_text == "proc;" && getw16(572) == 9, "processing result stops everything");
    // the processing call may change the word
    setup(9); processing_change = true; processing_set_word = 5; setState__10FireBeetlei(object, 0);
    expect(log_text == "proc;" + T(100, D, 3) && getw16(572) == 5, "changed word selects case 5");
    // states without work and out-of-range values only store
    for (int s : {2, 4, 7, 11, 12, 15, 16, 100, -1}) { run(9, s, "", "store only", s); }
    run(9, 5, T(100, D, 3), "case 5", 5);
    run(9, 9, T(84, A, 3), "case 9", 9);
    // case 0: w256 stored before the call
    run(9, 0, "rand;" + T(84, A, 3), "case 0", 0);
    expect(getw(256) == 1000 + 7 % 60 + 120, "case 0 due time");
    // case 6, 14
    run(9, 6, T(100, D, 3) + O(28, D, D, 8) + "rand;", "case 6", 6);
    expect(getw(256) == 7 % 20 + 20, "case 6 due time");
    run(9, 14, T(100, D, 3) + O(28, D, D, 8) + "rand;", "case 14", 14);
    expect(getw(256) == 7 % 20 + 20 && getw(964) == 1300, "case 14 words");
    // case 13
    run(9, 13, T(68, E, 3) + O(116, C, D, 8), "case 13", 13);
    expect(getw(960) == 1120 && getw(176) == 0x43340000u && (u64_at_call & 0x100000ull) != 0, "case 13 words");
    // case 1: other current state and current 13
    run(9, 1, "reset;dest300;" + T(124, D, 3), "case 1 other", 1);
    expect(getw(176) == 0x43200000u && getw(272) == 0x40f00000u, "case 1 words");
    run(13, 1, "reset;dest300;" + T(124, B, 3) + O(108, C, D, 8), "case 1 from 13", 1);
    expect((u64_at_call & 0x100000ull) == 0 && (u64_at_call | 0x100000ull) == 0xffffffffffffffffull, "case 1 clears bit 20 before the call");
    // case 3
    run(9, 3, T(124, D, 3), "case 3 other", 3);
    expect(getw(176) == 0x43200000u, "case 3 float");
    run(3, 3, "", "case 3 from 3", 3);
    run(13, 3, T(124, B, 3) + O(108, C, D, 8), "case 3 from 13", 3);
    // case 8
    run(9, 8, "", "case 8 from 9", 9);
    setup(6); vslots_b::write_u16(vslots_b::address(object) + 294u, 8u); setState__10FireBeetlei(object, 8);
    expect(log_text == "proc;" && getw16(572) == 6, "case 8 from 6 with bit 3");
    run(6, 8, O(76, D, D, 16), "case 8 from 6 without bit 3", 8);
    run(13, 8, "rand;" + T(84, E, 3) + O(108, C, D, 8), "case 8 from 13", 0);
    expect(getw(256) == 1000 + 7 % 60 + 120, "case 8 from 13 due time");
    run(8, 8, "", "case 8 from 8", 8);
    run(0, 8, O(76, D, D, 16), "case 8 from 0", 8);
    // case 10: first roll not a multiple of 3 -> no part loss
    rolls[0] = 7; rolls[1] = 2; rolls[2] = 3; rolls[3] = 9;
    run(9, 10, "rand;drop1.500000,3.500000/33/42100000/40000000/40333333/00000000/3f800000/3e800000/fire_beetle_eye;rand;" + T(36 + 8 * (2 & 3) , D, 6) + "death;", "case 10 no loss", 10);
    // first roll multiple of 3: part from the second roll
    for (unsigned q = 0; q < 6; ++q) {
        rolls[0] = 9; rolls[1] = q; rolls[2] = 1; rolls[3] = 0;
        int part = static_cast<int>(q) + 2; if (part == 5) part = 4; else if (part == 7) part = 6;
        run(9, 10, "rand;rand;lose" + num(part) + ";drop1.500000,3.500000/33/42100000/40000000/40333333/00000000/3f800000/3e800000/fire_beetle_eye;rand;" +
                       T(36 + 8 * 1, D, 6) + "death;", "case 10 part loss", 10);
        expect(death_adjusted == object + 8, "death call is adjusted");
    }
    // case 10 with bit 1 of +12: animSetToEnd before the virtual call
    rolls[0] = 7; rolls[1] = 2; rolls[2] = 3; rolls[3] = 9;
    setup(9); setw(12, 2); setState__10FireBeetlei(object, 10);
    expect(log_text.find("end;death;") != std::string::npos, "animSetToEnd");
    return failures ? 1 : 0;
}
