// Host contract for setState__8Froglocki. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "set_state.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static std::string log_text;
static unsigned rolls[4];
static unsigned roll_index;
static int processing_result;
static bool processing_change;
static int processing_set_word;
static int zone_result;
static bool alloc_result;
static unsigned char object[1300], group[512], table[256], block[600], first_point[12], second_point[12];
static void* death_adjusted;
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static void add(const std::string& t) { log_text += t; log_text += ';'; }
static std::string num(unsigned v) { return std::to_string(v); }
static std::string header_name(void* h) { return num(static_cast<unsigned>(static_cast<unsigned char*>(h) - group)); }
static std::string bits(float f) { unsigned b; std::memcpy(&b, &f, 4); char t[16]; std::snprintf(t, sizeof t, "%08x", b); return t; }
extern "C" int setStateProcessing__8CreaturePi(void*, int* state) { add("proc"); if (processing_change) *state = processing_set_word; return processing_result; }
extern "C" unsigned eRand__10GameObject(void*) { add("rand"); return rolls[roll_index++ % 4]; }
extern "C" void resetRouting__8Creature(void*) { add("reset"); }
extern "C" void getSmartRandomDestination__8Creaturei(void* out, void*, int range) {
    add("dest" + num(range)); const float v[3] = {7.5f, 8.5f, 9.5f}; std::memcpy(out, v, 12);
}
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float r, int flags) {
    expect(state == object + 288, "transition state"); add("T" + header_name(header) + "/" + bits(r) + "/" + num(flags));
}
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags) {
    expect(state == object + 288, "one-shot state"); add("O" + header_name(header) + "/" + bits(a) + "/" + bits(b) + "/" + num(flags));
}
extern "C" void animGlobalSync__FP14AnimationState(void* s) { expect(s == object + 288, "sync state"); add("sync"); }
extern "C" int enemyInZone__8Creaturefs(void*, float d, short n) { add("zone/" + bits(d) + "/" + num(n)); return zone_result; }
extern "C" void dropTreasure__8CreatureG6Point3iffffffPc(void*, const void* p, int angle, float f12, float f13, float f14,
                                                         float f15, float f16, float f17, char* name) {
    float pt[3]; std::memcpy(pt, p, 12);
    add("drop" + std::to_string(pt[0]) + "," + std::to_string(pt[2]) + "/" + num(angle & 0xffff) + "/" + bits(f12) + "/" + bits(f13) + "/" + bits(f14) +
        "/" + bits(f15) + "/" + bits(f16) + "/" + bits(f17) + "/" + (name ? name : "null"));
}
extern "C" void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, const void* position, short angle, void* state, int index, void* out, void* other, float scale) {
    float pt[3]; std::memcpy(pt, position, 12);
    add("mgcap/" + header_name(model) + "/" + std::to_string(pt[0]) + "/" + num(static_cast<unsigned short>(angle)) + "/" + (state == object + 288 ? "anim" : "bad") +
        "/" + num(index) + "/" + bits(scale));
    const float a[3] = {5.0f, 6.0f, 7.0f}, b[3] = {8.0f, 9.0f, 10.0f};
    std::memcpy(out, a, 12); std::memcpy(other, b, 12);
    std::memcpy(first_point, out, 12); (void)second_point;
}
extern "C" void* blockAlloc__Fi(int size) { add("alloc" + num(size)); return alloc_result ? block : nullptr; }
extern "C" void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, const void* position, char* first, char* second, char* third,
                                                                long a, int b, void* description, float scale) {
    float pt[3]; std::memcpy(pt, position, 12);
    add(std::string("phprop") + (self == block ? "/block" : "/bad") + "/" + std::to_string(pt[0]) + "/" + header_name(first) + "/" + header_name(second) + "/" +
        header_name(third) + "/" + std::to_string(a) + "/" + num(b) + "/" + (description ? "d" : "null") + "/" + bits(scale));
}
static void death_function(void* adjusted, int) { add("death"); death_adjusted = adjusted; }

static unsigned getw(unsigned off) { return vslots_b::read_word(vslots_b::address(object) + off); }
static unsigned getw16(unsigned off) { return vslots_b::read_u16(vslots_b::address(object) + off); }
static void setw(unsigned off, unsigned v) { vslots_b::write_word(vslots_b::address(object) + off, v); }
static void setup(int current, int p, int q) {
    std::memset(object, 0x41, sizeof object);
    std::memset(group, 0, sizeof group);
    setw(36, vslots_b::address_bits(group));
    // group word n/4 (for offsets below 160) holds the address of group + 256 + n so header names are the offsets + 256
    for (unsigned off = 0; off + 4 <= 160; off += 4)
        vslots_b::write_word(vslots_b::address(group) + off, vslots_b::address_bits(group + 256 + off));
    vslots_b::write_u16(vslots_b::address(object) + 572u, static_cast<unsigned>(current) & 0xffffu);
    object[568] = static_cast<unsigned char>(p); object[1105] = static_cast<unsigned char>(q);
    setw(808, 1000); setw(12, 0); setw(960, 0);
    setw(72, vslots_b::address_bits(table));
    vslots_b::write_u16(vslots_b::address(table) + 192u, 8u);
    vslots_b::write_word(vslots_b::address(table) + 196u, vslots_b::address_bits(reinterpret_cast<void*>(death_function)));
    const float pos[3] = {1.5f, 2.5f, 3.5f}; std::memcpy(object + 16, pos, 12);
    const float scale = 3.25f; std::memcpy(object + 224, &scale, 4);
    vslots_b::write_u16(vslots_b::address(object) + 56u, 0x1021u);
    vslots_b::write_u16(vslots_b::address(object) + 294u, 0u);
    log_text.clear(); roll_index = 0; processing_result = 0; processing_change = false; zone_result = 0; alloc_result = true;
}
static const char* S = "3dcccccc";
static const char* T = "3e999999";
static std::string Hn(unsigned off) { return num(256 + off); }
static std::string Tr(unsigned off, const char* r, int flags) { return "T" + Hn(off) + "/" + r + "/" + num(flags) + ";"; }
static std::string Os(unsigned off, int flags) { return "O" + Hn(off) + "/" + S + "/" + S + "/" + num(flags) + ";"; }
static void run(int current, int p, int q, int state, const std::string& want, const char* name, int want_state = -999) {
    setup(current, p, q);
    setw(256, 0xdeadbeef);
    setState__8Froglocki(object, state);
    if (log_text != "proc;" + want) { std::fprintf(stderr, "%s\n  got  %s\n  want %s\n", name, log_text.c_str(), ("proc;" + want).c_str()); ++failures; }
    if (want_state != -999) expect(static_cast<int>(getw16(572)) == (want_state & 0xffff), name);
}
int main() {
    using namespace vslots_b;
    host_note(group); host_note(table); host_note(object); host_note(block); host_note(reinterpret_cast<void*>(death_function));
    for (unsigned off = 0; off < 160; off += 4) host_note(group + 256 + off);
    rolls[0] = 245; rolls[1] = 61; rolls[2] = 5; rolls[3] = 1;
    setup(9, 0, 0); processing_result = 1; setState__8Froglocki(object, 5);
    expect(log_text == "proc;" && getw16(572) == 9, "processing result stops everything");
    setup(9, 0, 0); processing_change = true; processing_set_word = 6; setState__8Froglocki(object, 0);
    expect(log_text == "proc;" + Os(100, 8) && getw16(572) == 6, "changed word selects case 6");
    for (int s : {2, 4, 11, 12, 16, 17, 100, -1}) run(9, 1, 1, s, "", "store only", s);
    // case 0
    run(9, 1, 0, 0, "rand;" + Tr(84, T, 3), "case 0 P set", 0);
    expect(getw(256) == 1000 + 245 % 120, "case 0 due time");
    run(9, 0, 1, 0, "rand;" + Tr(60, T, 3), "case 0 P clear", 0);
    run(9, 1, 0, 9, Tr(84, T, 3), "case 9 P set", 9);
    run(9, 0, 1, 9, Tr(60, T, 3), "case 9 P clear", 9);
    // case 1
    run(9, 1, 1, 1, "reset;dest200;" + Tr(116, S, 2051) + "sync;", "case 1 both", 1);
    expect(getw(272) == 0x40f00000u, "case 1 destination stored");
    run(9, 1, 0, 1, "reset;dest200;" + Tr(124, S, 2051) + "sync;", "case 1 Q clear", 1);
    run(9, 0, 1, 1, "reset;dest200;" + Tr(124, S, 2051) + "sync;", "case 1 P clear", 1);
    // case 7
    run(9, 1, 1, 7, Tr(116, S, 2051) + "sync;", "case 7 both", 7);
    expect(getw(256) == 1180, "case 7 due time");
    run(9, 0, 0, 7, Tr(124, S, 2051) + "sync;", "case 7 neither", 7);
    // case 3
    const std::string zone = "zone/43c80000/6000;";
    setup(9, 1, 1); setw(960, 16); zone_result = 1; setState__8Froglocki(object, 3);
    expect(log_text == "proc;reset;" + zone + Tr(84, S, 2051) + "sync;", "case 3 both, zone");
    setup(9, 1, 1); setw(960, 16); zone_result = 0; setState__8Froglocki(object, 3);
    expect(log_text == "proc;reset;" + zone + Tr(116, S, 2051) + "sync;", "case 3 both, no zone");
    setup(9, 0, 1); setw(960, 16); zone_result = 1; setState__8Froglocki(object, 3);
    expect(log_text == "proc;reset;" + zone + Tr(76, S, 2051) + "sync;", "case 3 not both, zone");
    setup(9, 1, 0); setw(960, 16); zone_result = 0; setState__8Froglocki(object, 3);
    expect(log_text == "proc;reset;" + zone + Tr(124, S, 2051) + "sync;", "case 3 not both, no zone");
    setup(9, 1, 1); setw(960, 15); setState__8Froglocki(object, 3);
    expect(log_text == "proc;reset;" + Tr(116, S, 2051) + "sync;", "case 3 other w960, both");
    setup(9, 0, 1); setw(960, 15); setState__8Froglocki(object, 3);
    expect(log_text == "proc;reset;" + Tr(124, S, 2051) + "sync;", "case 3 other w960, not both");
    // case 5, 6, 13, 14, 15
    setup(9, 0, 0); setw(960, 16); setState__8Froglocki(object, 5); expect(log_text == "proc;" + Tr(68, S, 3), "case 5 w960 16");
    setup(9, 0, 0); setw(960, 3); setState__8Froglocki(object, 5); expect(log_text == "proc;" + Tr(76, S, 3), "case 5 other");
    run(9, 0, 0, 6, Os(100, 8), "case 6", 6);
    run(9, 0, 0, 13, Tr(76, S, 3) + Os(28, 8) + "rand;", "case 13", 13);
    expect(getw(256) == 245 % 15, "case 13 due time");
    run(9, 0, 0, 14, Tr(84, S, 3) + Os(108, 8) + "rand;", "case 14", 14);
    run(9, 1, 1, 15, Tr(116, S, 2051) + Os(92, 2064), "case 15 both", 15);
    run(9, 1, 0, 15, Tr(124, S, 2051) + Os(92, 2064), "case 15 Q clear", 15);
    // case 8
    run(9, 0, 0, 8, "", "case 8 from 9", 9);
    run(15, 0, 0, 8, "", "case 8 from 15", 15);
    setup(6, 0, 0); vslots_b::write_u16(vslots_b::address(object) + 294u, 8u); setState__8Froglocki(object, 8);
    expect(log_text == "proc;" && getw16(572) == 6, "case 8 from 6 with bit 3");
    run(6, 0, 0, 8, Os(52, 16), "case 8 from 6 without bit 3", 8);
    run(13, 0, 0, 8, Os(52, 16), "case 8 from 13", 8);
    // case 10
    const std::string drop = "drop1.500000,3.500000/4129/42100000/40000000/3fe66666/3f199999/40000000/3e800000/null;";
    rolls[0] = 2; run(9, 0, 0, 10, "rand;" + Tr(36 + 0, T, 6) + drop + "death;", "case 10 no spawn (w960 zero)", 10);
    rolls[0] = 3; run(9, 0, 0, 10, "rand;" + Tr(36 + 8, T, 6) + drop + "death;", "case 10 roll 3", 10);
    expect(death_adjusted == object + 8, "death adjusted");
    setup(9, 0, 0); rolls[0] = 2; setw(960, 2); setState__8Froglocki(object, 10);
    expect(log_text == "proc;rand;" + Tr(36, T, 6) + drop + "mgcap/" + Hn(12) + "/1.500000/" + num(static_cast<unsigned short>(0x1021 - 16384)) + "/anim/0/3f800000;alloc584;" +
                           "phprop/block/5.000000/" + Hn(0) + "/" + Hn(16) + "/" + Hn(32) + "/1/16385/null/40500000;death;", "case 10 spawn");
    expect(getw(960) == 0, "case 10 clears w960");
    setup(9, 0, 0); rolls[0] = 2; setw(960, 2); alloc_result = false; setState__8Froglocki(object, 10);
    expect(log_text.find("alloc584;death;") != std::string::npos && getw(960) == 0, "case 10 failed allocation still clears");
    setup(9, 0, 0); rolls[0] = 2; setw(960, 2); setw(12, 2); setState__8Froglocki(object, 10);
    expect(log_text == "proc;rand;" + Tr(36, T, 6) + drop + "death;" && getw(960) == 2, "case 10 bit 1 set skips the spawn");
    return failures ? 1 : 0;
}
