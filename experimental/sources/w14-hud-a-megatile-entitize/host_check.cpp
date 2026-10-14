// Build as x86_64 with statics below 4 GiB (the block uses 32-bit pointer words):
//   c++ -arch x86_64 -std=c++17 -Wl,-no_pie -Wl,-pagezero_size,0x1000 host_check.cpp
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include <cstdarg>
// The target passes the two doubles of the point format as 64-bit integers; the host
// reinterprets them, so route sprintf through a local routine.
extern "C" int host_sprintf(char*, const char*, ...);
#define sprintf host_sprintf
#include "megatile_entitize.cpp"
#undef sprintf
extern "C" int host_sprintf(char* out, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int n;
    if (std::strcmp(fmt, "pcount=-%d") == 0) {
        n = std::snprintf(out, 64, fmt, va_arg(ap, int));
    } else {
        Double a = va_arg(ap, Double), b = va_arg(ap, Double);
        double x, y; std::memcpy(&x, &a, 8); std::memcpy(&y, &b, 8);
        n = std::snprintf(out, 64, fmt, x, y);
    }
    va_end(ap);
    return n;
}

extern "C" {
int kittencount, levsuffix, pmlistptrsused, pmlistcharsused, dynlevelcount;
int lightStart[9];
signed char mt_tiledata[320];
float mt_ebx[256];
float mt_eby[256];
int instmap_bounds[6];
char* pmlistptrs[256];
int (*userFilterFunction)(char*);
unsigned char* mgrg;
}

static std::vector<std::string> events;
static int pmCounter;
static char pmPool[64][64];
static char* argsMarker = reinterpret_cast<char*>(0x5000);
static int filterCalls;

extern "C" {
Double fptodp(float v) { double d = v; Double r; std::memcpy(&r, &d, 8); return r; }
char* addpmlist__FPc(char* s) {
    events.push_back(std::string("pm:") + s);
    std::strcpy(pmPool[pmCounter], s);
    return pmPool[pmCounter++];
}
char** addpmlistptrscap__Fv() { events.push_back("cap"); return &argsMarker; }
char** specialparse__FPPciiG6Point3Pc(char**, int selector, int mode, const Point3*, char* name) {
    events.push_back("spec:" + std::string(name) + ":" + std::to_string(selector) + ":" + std::to_string(mode));
    return &argsMarker;
}
void doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(unsigned char* record, int index, unsigned char* delayed, int* count, bool filter) {
    unsigned name; std::memcpy(&name, record, 4);
    events.push_back("disp:" + std::string(reinterpret_cast<char*>(static_cast<unsigned long>(name))) + ":" + std::to_string(index) + ":" + (filter ? "f" : "n"));
    unsigned char* d = delayed + 28 * *count;
    std::memset(d, 0, 28);
    std::memcpy(d + 12, &name, 4);
    d[24] = 3;
    *count += 1;
}
void* objectCreateByName__FPcG6Point3sPPc(char* name, const Point3* point, short type, char** args) {
    float p[3]; std::memcpy(p, point, 12);
    events.push_back("create:" + std::string(name) + ":" + std::to_string((int)p[0]) + ":" + std::to_string(type) + ":" + (args == &argsMarker ? "L" : "a"));
    return nullptr;
}
void objectProcessDeleteList__Fv() { events.push_back("del"); }
}

static int filterFn(char* name) { filterCalls++; return std::strcmp(name, "Directional_Light_x") != 0; }

static unsigned char block[512];
static char names[8][32];
static int bad;
#define CHECK(c) do { if (!(c)) { std::printf("fail line %d\n", __LINE__); bad++; } } while (0)

static void put16(unsigned char* p, int v) { short s = (short)v; std::memcpy(p, &s, 2); }
static void put32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void putf(unsigned char* p, float v) { std::memcpy(p, &v, 4); }

// Builds `n` records of 24 bytes (no arguments) with pre-relocated names; returns the block.
static void build(const char* const* list, const float* xs, int n, unsigned flags) {
    std::memset(block, 0, sizeof block);
    put16(block, n);
    put16(block + 2, flags);
    put32(block + 4, 0);
    int off = 8;
    for (int i = 0; i < n; ++i) {
        std::strcpy(names[i], list[i]);
        unsigned char* rec = block + off;
        unsigned ptr = (unsigned)(unsigned long)names[i];
        put32(rec, (flags & 2u) ? ptr : ptr - (unsigned)(unsigned long)block);
        put16(rec + 4, 24);
        put16(rec + 6, 7 + i);
        putf(rec + 8, xs[i]);
        putf(rec + 12, 100.0f);
        putf(rec + 16, 5.0f);
        put32(rec + 20, 0);
        off += 24;
    }
}

int main() {
    const char* const list[] = {"Directional_Light_x", "zzTileType_1", "Orc", "EdgeMarker2", "customVars"};
    const float xs[] = {1.0f, 2.0f, 30.0f, 4.0f, 50.0f};
    // tile rows: row 0 covers x 0..1, y 0..1; row 1 covers x 2..3? (unused: 30/720 -> 0)
    std::memset(mt_tiledata, 0, sizeof mt_tiledata);
    mt_tiledata[2] = 0; mt_tiledata[5] = 2; mt_tiledata[6] = 2;       // row 0: x 0..1, y 0..1
    mt_tiledata[12] = 5; mt_tiledata[10 + 2] = 0;                       // row 1: x 5.., y 0.., 5-wide 0 -> never matches
    mt_tiledata[10 + 0] = 5; mt_tiledata[10 + 5] = 0; mt_tiledata[10 + 6] = 0;
    mt_tiledata[20 + 2] = -1;                                           // row 2 terminates the scan
    static unsigned char mg[64];
    put16(mg + 50, 0);
    mgrg = mg;
    userFilterFunction = nullptr;
    kittencount = levsuffix = pmlistptrsused = pmlistcharsused = dynlevelcount = 9;
    build(list, xs, 5, 2u);   // already relocated
    megatileEntitize__FPci(block, 0);
    CHECK(kittencount == 0 && levsuffix == 0 && pmlistptrsused == 0 && dynlevelcount == 0);
    CHECK(lightStart[0] == 32 && lightStart[3] == 68 && lightStart[6] == 0);
    CHECK(lightStart[7] == (int)0xbf800000u && lightStart[8] == 0x3f800000);
    std::vector<std::string> want = {"create:Directional_Light_x:1:7:a", "disp:Orc:0:n", "create:customVars:50:11:a", "spec:Orc:3:1", "create:Orc:0:0:L", "del"};
    // customVars is created at once (record 4); Orc is dispatched while walking, so its delayed creation comes last.
    if (events != want) {
        for (auto& e : events) std::printf("  %s\n", e.c_str());
        bad++;
    }
    unsigned short flags; std::memcpy(&flags, block + 2, 2);
    CHECK(flags == 2);

    // filter set, function present: "Directional_Light_x" is refused
    events.clear(); filterCalls = 0;
    userFilterFunction = filterFn;
    build(list, xs, 5, 2u);
    megatileEntitize__FPci(block, 1);
    CHECK(filterCalls == 3);   // Directional_Light_x, customVars, then the delayed Orc
    bool sawLight = false;
    for (auto& e : events) if (e.rfind("create:Directional_Light_x", 0) == 0) sawLight = true;
    CHECK(!sawLight);
    // filter set, no function: nothing is created
    events.clear(); userFilterFunction = nullptr;
    build(list, xs, 5, 2u);
    megatileEntitize__FPci(block, 1);
    for (auto& e : events) CHECK(e.rfind("create:", 0) != 0);

    // relocation: with the flag clear the name words are rebased onto the block
    events.clear();
    build(list, xs, 5, 0u);
    megatileEntitize__FPci(block, 0);
    CHECK(events.size() == 6);

    // Line groups
    events.clear(); pmCounter = 0;
    put16(mg + 50, 1);
    instmap_bounds[0] = 1; instmap_bounds[1] = 2;
    const float ebx[] = {10.0f, 20.0f, -1.0f, 5.0f, -1.0f, -2.0f};
    for (int i = 0; i < 6; ++i) { mt_ebx[i] = ebx[i]; mt_eby[i] = ebx[i] + 1.0f; }
    build(list, xs, 0, 2u);
    megatileEntitize__FPci(block, 0);
    CHECK(events.size() == 9 + 8 + 1);   // second group has one point; then the delete list
    CHECK(events[0] == "pm:Line" && events[1] == "pm:name=barrier" && events[4] == "pm:pcount=-2");
    // first point: ((10 - 12) + 6 * 1) * 30 * 4 = 480, ((11 - 12) + 6 * 2) * 30 * 4 = 1320
    CHECK(events[5] == "pm:p=480.000000,1320.000000,0");
    CHECK(events[6] == "pm:p=1680.000000,2520.000000,0");
    CHECK(events[7] == "cap" && events[8] == "create:Line:0:0:L");
    CHECK(events[9] == "pm:Line" && events[13] == "pm:pcount=-1");
    if (!bad) std::puts("megatileEntitize passed");
    return bad;
}
