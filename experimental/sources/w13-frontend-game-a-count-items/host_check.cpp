// Host check for countItems__FP10GameObject. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -DW13_FRONT_HOST -ffp-contract=off -fno-builtin host_check.cpp
// Pointer words in the objects are small handles that hostWordAddress maps to host addresses.
#include "count_items.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
float maxDistance;
int fullCount;
void* itemToDestroy;
alignas(8) unsigned char player[2 * 23984];
}
static std::string trace;
static int tokenItem, tokenOther;
static void* typeAnswer;
extern "C" void __throw_bad_typeid() { trace += "throw;"; }
extern "C" void* __tf8ItemProp() { trace += "itemprop;"; return &tokenItem; }
extern "C" int __eq__C9type_infoRC9type_info(const void* a, const void* b) { trace += "eq;"; return a == b; }

alignas(8) static unsigned char vtable[16];
alignas(8) static unsigned char worldA[64];
alignas(8) static unsigned char worldB[64];
static void* objectSeen;
static void* typeFn(void* self) { objectSeen = self; trace += "type;"; return typeAnswer; }
__UINTPTR_TYPE__ hostWordAddress(unsigned word) {
    switch (word) {
        case 0x200: return reinterpret_cast<__UINTPTR_TYPE__>(vtable);
        case 0x301: return reinterpret_cast<__UINTPTR_TYPE__>(&typeFn);
        case 0x401: return reinterpret_cast<__UINTPTR_TYPE__>(worldA);
        case 0x402: return reinterpret_cast<__UINTPTR_TYPE__>(worldB);
    }
    return 0;
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    trace: %s\n", __LINE__, #c, trace.c_str()); return 1; } } while (0)

static void setf(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static void setu(unsigned char* o, int off, unsigned v) { std::memcpy(o + off, &v, 4); }

static void makeObject(unsigned char (&o)[400], float x, float y, float z, unsigned long long flags) {
    std::memset(o, 0, sizeof o);
    setf(o, 16, x); setf(o, 20, y); setf(o, 24, z);
    setu(o, 72, 0x200);
    std::memcpy(o + 336, &flags, 8);
}
static void reset() {
    trace.clear();
    maxDistance = 5.0f; fullCount = 7; itemToDestroy = nullptr; objectSeen = nullptr;
    std::memset(player, 0, sizeof player);
    std::memset(worldA, 0, sizeof worldA); std::memset(worldB, 0, sizeof worldB);
    std::memset(vtable, 0, sizeof vtable);
    setu(vtable, 4, 0x301);
    setu(player, 21872, 0x401);
    setu(player, 45856, 0x402);
    setf(worldA, 16, 100); setf(worldA, 20, 200); setf(worldA, 24, 300);
    setf(worldB, 16, 0); setf(worldB, 20, 0); setf(worldB, 24, 0);
    typeAnswer = &tokenItem;
}

int main() {
    unsigned char o[400];
    // farther than the stored maximum: recorded; distance from player 0's world object
    reset(); makeObject(o, 103, 204, 300, 0);   // distance 5 exactly: not above 5
    countItems__FP10GameObject(o);
    CHECK(trace == "type;itemprop;eq;" && objectSeen == o);
    CHECK(fullCount == 8 && itemToDestroy == nullptr && maxDistance == 5.0f);
    reset(); makeObject(o, 106, 204, 300, 0);   // dx=-6 dy=-4: sqrt(52)
    countItems__FP10GameObject(o);
    CHECK(fullCount == 8 && itemToDestroy == o && maxDistance == std::sqrt(52.0f));
    // all three axes count, with the reference minus the object (sign does not matter, square does)
    reset(); makeObject(o, 100, 200, 310, 0);
    countItems__FP10GameObject(o);
    CHECK(itemToDestroy == o && maxDistance == 10.0f);
    reset(); makeObject(o, 100, 190, 300, 0);
    countItems__FP10GameObject(o);
    CHECK(itemToDestroy == o && maxDistance == 10.0f);
    reset(); makeObject(o, 90, 200, 300, 0);
    countItems__FP10GameObject(o);
    CHECK(itemToDestroy == o && maxDistance == 10.0f);
    // wrong type: nothing else happens
    reset(); makeObject(o, 500, 500, 500, 0); typeAnswer = &tokenOther;
    countItems__FP10GameObject(o);
    CHECK(trace == "type;itemprop;eq;" && fullCount == 7 && itemToDestroy == nullptr && maxDistance == 5.0f);
    // flag bit 39 set: skipped before the count; other bits do not skip
    reset(); makeObject(o, 500, 500, 500, 0x8000000000ull);
    countItems__FP10GameObject(o);
    CHECK(fullCount == 7 && itemToDestroy == nullptr);
    reset(); makeObject(o, 500, 500, 500, 0x7fffffffffull | 0x7fffff0000000000ull);
    countItems__FP10GameObject(o);
    CHECK(fullCount == 8 && itemToDestroy == o);
    // a null object throws (and the comparison then sees no type)
    reset();
    countItems__FP10GameObject(nullptr);
    CHECK(trace.substr(0, 6) == "throw;");
    // player 0 without a world object: player 1's is used; with neither, only the count changes
    reset(); makeObject(o, 3, 4, 0, 0); setu(player, 21872, 0);
    countItems__FP10GameObject(o);
    CHECK(fullCount == 8 && itemToDestroy == nullptr && maxDistance == 5.0f);  // distance 5 from the origin
    reset(); makeObject(o, 6, 8, 0, 0); setu(player, 21872, 0);
    countItems__FP10GameObject(o);
    CHECK(itemToDestroy == o && maxDistance == 10.0f);
    reset(); makeObject(o, 6, 8, 0, 0); setu(player, 21872, 0); setu(player, 45856, 0);
    countItems__FP10GameObject(o);
    CHECK(fullCount == 8 && itemToDestroy == nullptr && maxDistance == 5.0f);
    // a smaller distance than the stored maximum leaves everything alone
    reset(); makeObject(o, 106, 204, 300, 0); maxDistance = 100.0f; itemToDestroy = &tokenOther;
    countItems__FP10GameObject(o);
    CHECK(itemToDestroy == &tokenOther && maxDistance == 100.0f && fullCount == 8);
    std::puts("count items passed");
    return 0;
}
