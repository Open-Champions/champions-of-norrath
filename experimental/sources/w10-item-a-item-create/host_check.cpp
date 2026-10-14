// Host check for create__4itemPcPQ24item11descriptionii. Not an EE plan input.
// The table words are 32-bit in the image; the source maps them through a hook here.
#include <cstdio>
#include <cstring>
#include <string>
static const char* handleTable[16];
char* hostText(unsigned w) { return const_cast<char*>(handleTable[w]); }
#define ITEM_CREATE_HOST_POINTER hostText
#include "item_create.cpp"
alignas(8) unsigned char _4item$itemTable[64 * 6];
alignas(8) unsigned char _4item$namedItemTable[64 * 6];
int syncRandState;
static void word(unsigned char* p, int off, unsigned v) { memcpy(p + off, &v, 4); }
static short half(const unsigned char* p, int off) { short v; memcpy(&v, p + off, 2); return v; }
static long long quad(const unsigned char* p, int off) { long long v; memcpy(&v, p + off, 8); return v; }
static unsigned char desc[72];
static void run(const char* name, int mesh, int named) {
    char text[256];
    strcpy(text, name);
    memset(desc, 0x55, sizeof desc);
    create__4itemPcPQ24item11descriptionii(text, desc, mesh, named);
}
#define CHECK(c, n) if (!(c)) { printf("fail %d\n", n); return n; }
int main() {
    handleTable[1] = "dagger"; handleTable[2] = "sword"; handleTable[3] = "bow"; handleTable[4] = "named";
    // itemTable: entry 0 dagger, entry 1 sword, entry 2 bow (5-bit field = 3 at bits 11..15 of the +40 quad)
    for (int i = 0; i < 3; i++) {
        unsigned char* e = _4item$itemTable + 64 * i;
        word(e, 0, 1 + i);
        *reinterpret_cast<short*>(e + 12) = static_cast<short>(100 + i);
        long long q = (i == 2 ? 3LL : 0LL) << 11 | 0x234;
        memcpy(e + 40, &q, 8);
    }
    word(_4item$namedItemTable, 0, 4);
    word(_4item$namedItemTable + 64, 0, 2);
    *reinterpret_cast<short*>(_4item$namedItemTable + 64 + 12) = 777;
    // plain lookup: the entry, its halfword and quad are copied, ids/fields initialised
    run("sword", 0, 0);
    CHECK(half(desc, 4) == 101, 1);
    CHECK((quad(desc, 8) & 0x7ff) == 0x234, 2);
    CHECK(desc[32] == 0xff && desc[33] == 0xff && half(desc, 16) == 0 && half(desc, 30) == 0 && half(desc, 36) == -1 && half(desc, 60) == -1, 3);
    CHECK(*reinterpret_cast<unsigned*>(desc) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_4item$itemTable + 64)), 4);
    // unknown names select entry 0
    run("nosuchitem", 0, 0);
    CHECK(half(desc, 4) == 100, 5);
    // named table is searched with the whole name, including options
    run("sword", 0, 1);
    CHECK(half(desc, 4) == 777, 6);
    run("sword+armorbonus5", 0, 1);
    CHECK(half(desc, 4) == 0, 7);  // not found by the full name: entry 0 of the named table

    // options
    run("sword+armorbonus12+firedamage3-9+fireburst1,2-3+requiredlevel-4", 0, 0);
    CHECK(half(desc, 36) == 13 && half(desc, 38) == 12 && half(desc, 40) == 0 && half(desc, 42) == 0, 8);
    CHECK(half(desc, 44) == 21 && half(desc, 46) == 3 && half(desc, 48) == 9, 9);
    CHECK(half(desc, 52) == 31 && half(desc, 54) == 1 && half(desc, 56) == 2 && half(desc, 58) == 3, 10);
    CHECK(half(desc, 60) == 14 && half(desc, 62) == -4, 11);
    // the same id again reuses its record; a fifth distinct id is dropped
    run("sword+armorbonus1+armorbonus2+attackspeed7+critical8+stunresistance9+healthbonus4", 0, 0);
    CHECK(half(desc, 36) == 13 && half(desc, 38) == 2 && half(desc, 44) == 20 && half(desc, 52) == 29 && half(desc, 60) == 30 && half(desc, 62) == 9, 12);
    // mesh%d replaces the mesh argument; the value lands in bits 11..15 of the quad
    run("sword+mesh5", -1, 0);
    CHECK(((quad(desc, 8) >> 11) & 0x1f) == 5, 13);
    run("sword", 7, 0);
    CHECK(((quad(desc, 8) >> 11) & 0x1f) == 7 && (quad(desc, 8) & 0x7ff) == 0x234, 14);
    // mesh -1 with a 5-bit field of 3 picks (state >> 16) % 3 after advancing the generator
    syncRandState = 0;
    run("bow", -1, 0);
    unsigned next = 0u * 0x19660du + 0x3c6ef35fu;
    CHECK(syncRandState == static_cast<int>(next), 15);
    CHECK(((quad(desc, 8) >> 11) & 0x1f) == static_cast<long long>((next >> 16) % 3), 16);
    // a field of 0 gives 0 without touching the generator
    syncRandState = 99;
    run("sword", -1, 0);
    CHECK(syncRandState == 99 && ((quad(desc, 8) >> 11) & 0x1f) == 0, 17);
    // unparsable options are ignored; "mesh" without a number makes sscanf return EOF (nonzero), which
    // counts as a match and stores the zeroed slot
    run("sword+nonsense+mesh", 2, 0);
    CHECK(half(desc, 36) == -1 && ((quad(desc, 8) >> 11) & 0x1f) == 0, 18);
    return 0;
}
