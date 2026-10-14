// Host check for doEntityDispatch__FP8MTObjDefiP13delayedEntityPib. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -DNORRATH_MEGATILE_HOST host_check.cpp
// Pointer words in the records are small ids into a registry so that the 32-bit word model runs on a 64-bit host.
#include "dispatch.cpp"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <strings.h>
#include <vector>

extern "C" {
unsigned char mt_tiledata[64];
unsigned char pmlistptrs[64];
unsigned char pmlistptrsused[4];
unsigned char kittencount[4];
unsigned char userFilterFunction[4];
// upper-case flag (bit 0) for 'A' to 'Z' at index 1 + character, as the original table has it
const unsigned char _ctype_[260] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 
};
}

namespace {
struct Spawn { char** arguments; unsigned char point[12]; short type_word; bool filter; };
struct Created { char* name; unsigned char point[12]; int type_word; char** arguments; };
struct Parsed { char** arguments; int selector; int mode; char* name; };
std::vector<void*> registry;
std::vector<Spawn> spawns;
std::vector<Created> created;
std::vector<Parsed> parsed;
std::vector<std::string> pooled;
int cap_calls, filter_result = 1;
unsigned filter_seen;
char* cap_sentinel;
char** parse_result;
int failures;
#define EXPECT(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); ++failures; } } while (0)

void word(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
unsigned getword(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
float getfloat(const unsigned char* p) { float f; std::memcpy(&f, p, 4); return f; }
void setfloat(unsigned char* p, float f) { std::memcpy(p, &f, 4); }
unsigned enroll(void* pointer) { registry.push_back(pointer); return static_cast<unsigned>(registry.size() - 1u); }
void reset() {
    std::memset(mt_tiledata, 0, sizeof mt_tiledata); std::memset(pmlistptrs, 0, sizeof pmlistptrs);
    std::memset(pmlistptrsused, 0, 4); std::memset(kittencount, 0, 4); std::memset(userFilterFunction, 0, 4);
    registry.assign(1, nullptr); spawns.clear(); created.clear(); parsed.clear(); pooled.clear();
    cap_calls = 0; filter_result = 1; filter_seen = 0; parse_result = &cap_sentinel;
}
// A record: name word at +0, half at +6, floats at +8, +12, +16, and the inline argument words from +20.
struct Record { unsigned char bytes[64]; };
Record make(char* name, std::vector<char*> arguments, unsigned half, float x, float y, float z) {
    Record r{};
    word(r.bytes, enroll(name)); r.bytes[6] = half & 0xff; r.bytes[7] = half >> 8;
    setfloat(r.bytes + 8, x); setfloat(r.bytes + 12, y); setfloat(r.bytes + 16, z);
    unsigned offset = 20;
    for (char* a : arguments) { word(r.bytes + offset, enroll(a)); offset += 4; }
    word(r.bytes + offset, 0);
    return r;
}
void row(int index, signed char a, signed char b, signed char kind, signed char c, signed char d, signed char e, signed char f, signed char g) {
    unsigned char* p = mt_tiledata + 10 * index;
    p[0] = a; p[1] = b; p[2] = kind; p[3] = c; p[4] = d; p[7] = e; p[8] = f; p[9] = g;
}
}
extern "C" {
void* norrath_megatile_host_pointer(unsigned id) { return id < registry.size() ? registry[id] : nullptr; }
void norrath_megatile_host_note_word(const unsigned char*, unsigned) {}
void norrath_megatile_host_note_store(const unsigned char*, const void*) {}
int norrath_megatile_host_filter(char*, unsigned code_word) { filter_seen = code_word; return filter_result; }
int strncmp_(const char* l, const char* r, unsigned n) { return std::strncmp(l, r, n); }
int strcasecmp_(const char* l, const char* r) { return ::strcasecmp(l, r); }
char* strstr_(const char* t, const char* n) { return const_cast<char*>(std::strstr(t, n)); }
int sprintf_(char* out, const char* format, ...) { va_list a; va_start(a, format); int n = std::vsnprintf(out, 96, format, a); va_end(a); return n; }
char* addpmlist__FPc(char* text) { pooled.emplace_back(text); return const_cast<char*>(pooled.back().c_str()); }
char** addpmlistptrscap__Fv() { ++cap_calls; return &cap_sentinel; }
char** specialparse__FPPciiG6Point3Pc(char** arguments, int selector, int mode, const unsigned char*, char* name) {
    parsed.push_back({arguments, selector, mode, name}); return parse_result;
}
void monsterSpawn__FPPcG6Point3sb(char** arguments, const void* point, short type_word, bool filter) {
    Spawn s{}; s.arguments = arguments; std::memcpy(s.point, point, 12); s.type_word = type_word; s.filter = filter; spawns.push_back(s);
}
unsigned char* objectCreateByName__FPcG6Point3sPPc(char* name, const unsigned char* point, int type_word, char** arguments) {
    Created c{}; c.name = name; std::memcpy(c.point, point, 12); c.type_word = type_word; c.arguments = arguments; created.push_back(c); return nullptr;
}
}

int main() {
    char mspawn[] = "MSpawn7", plain[] = "Barrel", trig[] = "TRIGGER", push[] = "pushtrigger", kitten[] = "Kitten2", bang[] = "!!!x", other[] = "other";
    // MSpawn: the monster spawn gets the inline list, the transformed point and the combined type word
    reset(); row(1, 1, 2, 0, 3, 4, -1, 2, 3);
    Record r = make(mspawn, {other}, 0x1234, 1000.0f, 2000.0f, 5.0f);
    unsigned char delayed[28 * 4] = {}; int count = 0;
    doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(r.bytes, 1, delayed, &count, true);
    EXPECT(spawns.size() == 1 && created.empty() && parsed.empty());
    EXPECT(spawns[0].arguments == reinterpret_cast<char**>(r.bytes + 20) && spawns[0].filter && spawns[0].type_word == 0x1234);
    EXPECT(getfloat(spawns[0].point) == -440.0f && getfloat(spawns[0].point + 4) == 2000.0f && getfloat(spawns[0].point + 8) == 185.0f);
    // the four region kinds
    const float want[3][2] = {{1600.0f, 1720.0f}, {1880.0f, 3040.0f}, {-160.0f, 3320.0f}};
    for (int kind = 1; kind <= 3; ++kind) {
        reset(); row(1, 1, 2, static_cast<signed char>(kind), 3, 4, -1, 2, 3);
        r = make(plain, {}, 0x1234, 1000.0f, 2000.0f, 5.0f);
        doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(r.bytes, 1, delayed, &count, false);
        EXPECT(created.size() == 1);
        if (!created.empty()) {
            EXPECT(getfloat(created[0].point) == want[kind - 1][0] && getfloat(created[0].point + 4) == want[kind - 1][1] && getfloat(created[0].point + 8) == 185.0f);
            EXPECT(created[0].type_word == static_cast<short>(0x1234 + (kind << 14)));
        }
    }
    // plain object: nothing parsed, created with the inline list
    reset(); r = make(plain, {other}, 7, 0.0f, 0.0f, 0.0f);
    doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(r.bytes, 0, delayed, &count, false);
    EXPECT(created.size() == 1 && created[0].arguments == reinterpret_cast<char**>(r.bytes + 20) && parsed.empty() && created[0].name == plain);
    // "trigger" (any case) and a "!!!" argument both go through specialparse with selector = region kind, mode 0; its result is used
    reset(); row(0, 0, 0, 2, 0, 0, 0, 0, 0); r = make(trig, {}, 7, 0.0f, 0.0f, 0.0f);
    char* result_list[2] = {other, nullptr}; parse_result = result_list;
    doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(r.bytes, 0, delayed, &count, false);
    EXPECT(parsed.size() == 1 && parsed[0].selector == 2 && parsed[0].mode == 0 && created.size() == 1 && created[0].arguments == result_list);
    reset(); r = make(plain, {other, bang}, 7, 0.0f, 0.0f, 0.0f);
    doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(r.bytes, 0, delayed, &count, false);
    EXPECT(parsed.size() == 1);
    // "pushtrigger" clears the type half of the record
    reset(); r = make(push, {}, 0x2222, 0.0f, 0.0f, 0.0f);
    doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(r.bytes, 0, delayed, &count, false);
    EXPECT(r.bytes[6] == 0 && r.bytes[7] == 0 && created.size() == 1 && created[0].type_word == 0);
    // kitten prefix (case-insensitive): counter read then incremented, "name=kitten<N>" and the arguments are published, then the cap
    reset(); word(kittencount, 5); word(pmlistptrsused, 2); r = make(kitten, {other, plain}, 7, 0.0f, 0.0f, 0.0f);
    doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(r.bytes, 0, delayed, &count, false);
    EXPECT(getword(kittencount) == 6 && getword(pmlistptrsused) == 5 && cap_calls == 1 && pooled.size() == 1 && pooled[0] == "name=kitten5");
    EXPECT(created.size() == 1 && created[0].arguments == &cap_sentinel);
    // parse result null: a delayed 28-byte record is written (point, name word, type half, argument list word) and the count grows
    reset(); parse_result = nullptr; row(0, 0, 0, 3, 0, 0, 0, 0, 0); r = make(trig, {}, 0x0101, 1.0f, 2.0f, 3.0f);
    count = 1; std::memset(delayed, 0xee, sizeof delayed);
    doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(r.bytes, 0, delayed, &count, false);
    EXPECT(count == 2 && created.empty());
    EXPECT(delayed[28 + 16] == 3 && delayed[28 + 17] == 0 && delayed[28 + 24] == 0xee);
    // filter: with a nonzero filter flag a zero function word stops creation; a function word calls it (nonzero lets it through)
    reset(); r = make(plain, {}, 7, 0.0f, 0.0f, 0.0f);
    doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(r.bytes, 0, delayed, &count, true);
    EXPECT(created.empty());
    word(userFilterFunction, 0x4000); filter_result = 0;
    doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(r.bytes, 0, delayed, &count, true);
    EXPECT(created.empty() && filter_seen == 0x4000);
    filter_result = 1;
    doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(r.bytes, 0, delayed, &count, true);
    EXPECT(created.size() == 1);
    if (failures == 0) std::puts("ok");
    return failures == 0 ? 0 : 1;
}
