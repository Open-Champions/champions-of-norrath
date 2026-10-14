// Host check for objectPutSaveData__FPci. Not an EE plan input.
// Build as x86_64 with statics below 4 GiB (pointers in the data are 32-bit words):
//   c++ -arch x86_64 -std=c++17 -Wall -Wextra -Werror -Wl,-no_pie -Wl,-pagezero_size,0x1000 host_check.cpp
// Builds a buffer of records, runs the routine with recording stubs for the callbacks and the list
// iterator, and checks the call order and arguments, the in-place option conversion, the header
// flags, the lightStart defaults and the returned size.
#include "object_put_save_data.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
int lightStart[9];
char* poolOffset;
int objectIgnoreFlags;
unsigned char deleteList[16];
unsigned norrath_create_by_name_function_word;
unsigned userFilterFunction;
}

alignas(16) static unsigned char buffer[0x400];
alignas(16) static unsigned char objectA[0x100], objectB[0x100], objectC[0x100];
alignas(16) static unsigned char vtableA[0x100], vtableB[0x100];
alignas(16) static unsigned char nodes[4][16];
static std::string trace;
static unsigned firstNode;
static int createCount;
static unsigned char* createResults[8];

static std::string str(const void* p) { return p ? std::string(static_cast<const char*>(p)) : std::string("null"); }
static unsigned word(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }

static int filterFn(char* name) { trace += "filter(" + str(name) + ");"; return (name && name[0] == 'k') ? 1 : 0; }
static void* createFn(char* name, Point3 position, short type, unsigned* options) {
    trace += "create(" + str(name) + "," + std::to_string(static_cast<int>(position.x)) + "," +
             std::to_string(static_cast<int>(position.y)) + "," + std::to_string(static_cast<int>(position.z)) + "," +
             std::to_string(type) + ",opts";
    for (int i = 0; options[i]; ++i) trace += ":" + str(reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(options[i])));
    trace += ");";
    return createResults[createCount++];
}
static std::string where(void* p) {
    unsigned char* q = static_cast<unsigned char*>(p);
    if (q >= objectA && q < objectA + 0x100) return "A+" + std::to_string(q - objectA);
    if (q >= objectB && q < objectB + 0x100) return "B+" + std::to_string(q - objectB);
    if (q >= objectC && q < objectC + 0x100) return "C+" + std::to_string(q - objectC);
    return "?";
}
static void slot8(void* self, void* data) {
    trace += "load(" + where(self) + "," + std::to_string(static_cast<unsigned char*>(data) - buffer) + ");";
}
static void slot1(void* self, void* arg) {
    trace += "msg(" + where(self) + "," + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(arg)) + ");";
}
extern "C" {
void __16ListSafeIteratorP8ListHeadi(unsigned* it, unsigned char* head, int mask) {
    trace += "begin(" + std::to_string(head == deleteList) + "," + std::to_string(mask) + ");";
    it[1] = firstNode;
}
void next__16ListSafeIterator(unsigned* it) { it[1] = *reinterpret_cast<unsigned*>(static_cast<__UINTPTR_TYPE__>(it[1])); }
void _$_16ListSafeIterator(unsigned* it, int flags) { (void)it; trace += "end(" + std::to_string(flags) + ");"; }
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    trace: %s\n", __LINE__, #c, trace.c_str()); return 1; } } while (0)

static void put16(int off, int v) { *reinterpret_cast<short*>(buffer + off) = static_cast<short>(v); }
static void put32(int off, int v) { std::memcpy(buffer + off, &v, 4); }
static void putf(int off, float v) { std::memcpy(buffer + off, &v, 4); }
// A record at off: name offset, type, position x, then n option offsets (terminated), padded to size.
static void record(int off, int nameOffset, int type, float x, int n, const int* optionOffsets, int size) {
    put32(off, nameOffset); put16(off + 4, size); put16(off + 6, type);
    putf(off + 8, x); putf(off + 12, x + 1); putf(off + 16, x + 2);
    for (int i = 0; i < n; ++i) put32(off + 20 + 4 * i, optionOffsets[i]);
    put32(off + 20 + 4 * n, 0);
}

static void reset(int flags) {
    std::memset(buffer, 0xee, sizeof buffer);
    std::memset(objectA, 0, sizeof objectA); std::memset(objectB, 0, sizeof objectB); std::memset(objectC, 0, sizeof objectC);
    // pool at 0x200: "keep" at 0x200, "drop" at 0x205, "extra" at 0x20a
    std::memcpy(buffer + 0x200, "keep\0drop\0extra\0", 16);
    put16(0, 3); put16(2, flags); put32(4, 0x200);
    const int opt1[2] = {10, -1};
    record(8, 0, 11, 100.f, 2, opt1, 0x30);          // name "keep", options "extra", null
    record(0x38, 5, 22, 200.f, 0, nullptr, 0x20);     // name "drop"
    record(0x58, -1, 33, 300.f, 1, opt1, 0x28);       // no name
    trace.clear(); createCount = 0; firstNode = 0;
    createResults[0] = objectA; createResults[1] = objectB; createResults[2] = objectC;
    for (unsigned char* v : {vtableA, vtableB}) std::memset(v, 0, 0x100);
    *reinterpret_cast<unsigned*>(objectA + 72) = word(vtableA);
    *reinterpret_cast<unsigned*>(objectB + 72) = word(vtableB);
    *reinterpret_cast<unsigned*>(objectC + 72) = word(vtableA);
    *reinterpret_cast<short*>(vtableA + 8) = 4; *reinterpret_cast<unsigned*>(vtableA + 12) = word(reinterpret_cast<void*>(slot1));
    *reinterpret_cast<short*>(vtableB + 8) = 8; *reinterpret_cast<unsigned*>(vtableB + 12) = word(reinterpret_cast<void*>(slot1));
    *reinterpret_cast<short*>(vtableA + 64) = 0; *reinterpret_cast<unsigned*>(vtableA + 68) = word(reinterpret_cast<void*>(slot8));
    *reinterpret_cast<short*>(vtableB + 64) = 0; *reinterpret_cast<unsigned*>(vtableB + 68) = word(reinterpret_cast<void*>(slot8));
    norrath_create_by_name_function_word = word(reinterpret_cast<void*>(createFn));
    userFilterFunction = word(reinterpret_cast<void*>(filterFn));
    std::memset(lightStart, 0x55, sizeof lightStart);
    objectIgnoreFlags = 9;
}

static void setNodes(bool withList) {
    firstNode = 0;
    if (!withList) return;
    // node0 -> objectA, node1 -> no object, node2 -> objectB
    unsigned char* objects[3] = {objectA, nullptr, objectB};
    for (int i = 0; i < 3; ++i) {
        *reinterpret_cast<unsigned*>(nodes[i]) = i < 2 ? word(nodes[i + 1]) : 0;
        *reinterpret_cast<unsigned*>(nodes[i] + 12) = word(objects[i]);
    }
    firstNode = word(nodes[0]);
}
static unsigned wordAt(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }

int main() {
    // flags 1: create and load data, no filter, a deleteList of two objects and a hole
    reset(1);
    setNodes(true);
    int size = objectPutSaveData__FPci(reinterpret_cast<char*>(buffer), 0);
    CHECK(size == 8 + 0x30 + 0x20 + 0x28);
    CHECK(trace ==
          "create(keep,100,101,102,11,opts:extra);load(A+0,40);"
          "create(drop,200,201,202,22,opts);load(B+0,80);"
          "create(null,300,301,302,33,opts:extra);load(C+0,120);"
          "begin(1,9);msg(A+4,3);msg(B+8,3);end(2);");
    CHECK(wordAt(objectA + 52) == word(buffer + 0x200) && wordAt(objectB + 52) == word(buffer + 0x205) && wordAt(objectC + 52) == 0);
    CHECK(wordAt(buffer + 28) == word(buffer + 0x20a) && wordAt(buffer + 32) == 0 && wordAt(buffer + 0x58 + 20) == word(buffer + 0x20a));
    CHECK(*reinterpret_cast<unsigned short*>(buffer + 2) == 3);
    CHECK(lightStart[0] == 32 && lightStart[1] == 32 && lightStart[2] == 32 && lightStart[3] == 68 && lightStart[4] == 68 &&
          lightStart[5] == 68 && lightStart[6] == 0);
    float f7, f8;
    std::memcpy(&f7, lightStart + 7, 4); std::memcpy(&f8, lightStart + 8, 4);
    CHECK(f7 == -1.0f && f8 == 1.0f);
    CHECK(poolOffset == reinterpret_cast<char*>(buffer + 0x200));

    // flags 2 (already relocated): the option words are left as they are; no data call (bit 1 clear)
    reset(2);
    put32(28, static_cast<int>(word(buffer + 0x20a)));
    put32(32, 0);
    put32(0x58 + 20, static_cast<int>(word(buffer + 0x20a)));
    put32(0x58 + 24, 0);
    size = objectPutSaveData__FPci(reinterpret_cast<char*>(buffer), 0);
    CHECK(size == 8 + 0x30 + 0x20 + 0x28);
    CHECK(trace ==
          "create(keep,100,101,102,11,opts:extra);create(drop,200,201,202,22,opts);create(null,300,301,302,33,opts:extra);begin(1,9);end(2);");
    CHECK(*reinterpret_cast<unsigned short*>(buffer + 2) == 2);

    // filter on: only names the filter accepts ('k...'); a null name is asked about as well
    reset(1);
    size = objectPutSaveData__FPci(reinterpret_cast<char*>(buffer), 1);
    CHECK(size == 8 + 0x30 + 0x20 + 0x28);
    CHECK(trace ==
          "filter(keep);create(keep,100,101,102,11,opts:extra);load(A+0,40);filter(drop);filter(null);begin(1,9);end(2);");
    // filter on but no filter function: every record is skipped
    reset(1);
    userFilterFunction = 0;
    size = objectPutSaveData__FPci(reinterpret_cast<char*>(buffer), 1);
    CHECK(size == 8 + 0x30 + 0x20 + 0x28 && trace == "begin(1,9);end(2);");
    // no create function: nothing is created, no data call, the options are still converted
    reset(1);
    norrath_create_by_name_function_word = 0;
    size = objectPutSaveData__FPci(reinterpret_cast<char*>(buffer), 0);
    CHECK(trace == "begin(1,9);end(2);" && wordAt(buffer + 28) == word(buffer + 0x20a) && size == 8 + 0x30 + 0x20 + 0x28);
    // the create function returns no object: no name store and no data call
    reset(1);
    createResults[0] = createResults[1] = createResults[2] = nullptr;
    size = objectPutSaveData__FPci(reinterpret_cast<char*>(buffer), 0);
    CHECK(trace.find("load") == std::string::npos && createCount == 3);
    // zero or negative count: only the header work, the list pass and the base size
    reset(1);
    put16(0, 0);
    setNodes(true);
    size = objectPutSaveData__FPci(reinterpret_cast<char*>(buffer), 0);
    CHECK(size == 8 && trace == "begin(1,9);msg(A+4,3);msg(B+8,3);end(2);" && *reinterpret_cast<unsigned short*>(buffer + 2) == 3);
    reset(1);
    put16(0, -4);
    size = objectPutSaveData__FPci(reinterpret_cast<char*>(buffer), 0);
    CHECK(size == 8 && trace == "begin(1,9);end(2);");
    std::puts("ok");
    return 0;
}
