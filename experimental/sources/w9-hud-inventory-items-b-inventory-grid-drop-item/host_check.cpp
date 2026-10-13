// Host check: build the unit and this file with -DNORRATH_INVENTORY_HOST=1 -fno-builtin
// (the stubs replace sprintf and strstr) (record words
// are 32-bit tokens, resolved through host_resolve).
#include "../inventory-items-record/record.hpp"
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
extern const unsigned char norrath_inventory_ability_sound_lump_view[];
unsigned char player[23984 * 2];
unsigned invGrid;
int invMsgHandle = 77;
unsigned char animState[264 * 2];
const unsigned char norrath_inventory_ability_sound_lump_view[8] = {'f', 'x', '.', 'l', 'm', 'p', 0, 0};
void inventoryGridDropItem__Fiiciiii(int, int, char, int, int, int, int);
struct DropPoint { float x, y, z; };
}

using namespace inventory_items;

static const void* table[256];
static unsigned put(const void* o) { for (unsigned i = 0; i < 256; ++i) if (!table[i]) { table[i] = o; return i + 1; } return 0; }
namespace inventory_items { const void* host_resolve(unsigned w) { return table[w - 1]; } }

static std::string ev;
static void add(const std::string& s) { ev += s; ev += ";"; }
static int local;
static std::string soundName, lumpName;
static int compareHits;
static unsigned char* hitCell;
static const void* lastItem;
static int allocFail;
static unsigned char propBuf[512];
static float spotX;

extern "C" int sprintf(char* d, const char* f, ...) { char* o = d; for (const char* q = f; (*o++ = *q++);) {} soundName = f; add(std::string("SPRINTF:") + f); return 0; }
extern "C" char* strstr(const char* h, const char* n) {
    for (; *h; ++h) { const char *a = h, *b = n; while (*a && *b && *a == *b) ++a, ++b; if (!*b) return (char*)h; }
    return n[0] ? nullptr : (char*)h;
}
extern "C" int netPlayerIsLocal__Fi(int p) { add("LOCAL" + std::to_string(p)); return local; }
extern "C" void* lumpFindResource__FPcT0(char* l, char* n) { lumpName = l; add(std::string("FIND:") + l + ":" + n); return (void*)0x55; }
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float r) { add("PLAY" + std::to_string(h == (void*)0x55) + std::to_string(o == nullptr) + std::to_string((int)r)); return 0; }
extern "C" void netSendMessage__FiPUci(int h, unsigned char* m, int n) {
    short s; unsigned w; std::memcpy(&s, m + 2, 2); std::memcpy(&w, m + 4, 4);
    add("SEND(" + std::to_string(h) + "," + std::to_string(m[0]) + "," + std::to_string(m[1]) + "," + std::to_string(s) + "," + std::to_string(w) + "," + std::to_string(n) + ")");
}
extern "C" void resetPlayerAnimations__Fi(int p) { add("RESET" + std::to_string(p)); }
extern "C" void inventorySetAnim__FiP14AnimationStatei(int p, void* s, int z) { add("ANIM" + std::to_string(p) + "," + std::to_string((unsigned char*)s - animState) + "," + std::to_string(z)); }
extern "C" int compareToItem__Q24item11descriptionPQ24item11description(const void* c, const void* i) {
    unsigned a; std::memcpy(&a, c, 4); ++compareHits; lastItem = i; return a == 0xABCD;
}
extern "C" DropPoint objectFindEmptySpot__FG6Point3iiPc(DropPoint p, int a, int b, char* c) {
    add("SPOT(" + std::to_string((int)p.x) + "," + std::to_string((int)p.y) + "," + std::to_string((int)p.z) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c == nullptr) + ")");
    return DropPoint{p.x + 1, p.y + 1, p.z + 1};
}
extern "C" void* blockAlloc__Fi(int n) { add("ALLOC" + std::to_string(n)); return allocFail ? nullptr : propBuf; }
extern "C" void __8ItemPropG6Point3PQ24item11descriptioni(void* t, DropPoint p, const void* item, int z) {
    add("CTOR(" + std::to_string(t == propBuf) + "," + std::to_string((int)p.x) + "," + std::to_string((int)p.y) + "," + std::to_string((int)p.z) + "," + std::to_string(z) + ")");
    spotX = p.x;
}
extern "C" void removeItemFromInventory__Fiiii(int p, int s, int l, int g) { add("REMOVE(" + std::to_string(p) + "," + std::to_string(s) + "," + std::to_string(l) + "," + std::to_string(g) + ")"); }
extern "C" void playerRecomputeWeight__Fi(int p) { add("WEIGHT" + std::to_string(p)); }
extern "C" void playerRecomputeAbilityModifiers__Fv() { add("ABIL"); }

static unsigned char descBytes[8][40];
static char names[8][16];
static unsigned char obj[2048];
static unsigned char gridMem[145800 * 2 + 48600 * 3 + 100];
static unsigned char* rec(int p) { return player + p * 23984; }
static unsigned& w(int p, int off) { return *reinterpret_cast<unsigned*>(rec(p) + off); }
static Slot* slotOf(int p, int list, int i) { return &player_lists(p)[list].slots[i]; }
static void setFlags(Slot* s, long long f) { std::memcpy(reinterpret_cast<unsigned char*>(s) + 8, &f, 8); }

static void reset() {
    std::memset(player, 0, sizeof player); std::memset(obj, 0xee, sizeof obj); std::memset(gridMem, 0, sizeof gridMem);
    std::memset(descBytes, 0, sizeof descBytes); std::memset(animState, 0, sizeof animState);
    for (int i = 0; i < 256; ++i) table[i] = nullptr;
    ev.clear(); local = 1; allocFail = 0; compareHits = 0; soundName.clear(); lumpName.clear();
    for (int i = 0; i < 8; ++i) { std::snprintf(names[i], 16, "item%d", i); unsigned t = put(names[i]); std::memcpy(descBytes[i], &t, 4); }
    float pos[3] = {10, 20, 30}; std::memcpy(obj + 16, pos, 12);
    w(0, 21872) = put(obj); w(1, 21872) = put(obj);
    invGrid = put(gridMem);
}
static unsigned makeItem(int p, int list, int slot, int descIdx, long long flags, int qty) {
    Slot* s = slotOf(p, list, slot);
    s->description = put(descBytes[descIdx]); s->quantity = (short)qty; setFlags(s, flags);
    return word_of(s);
}
#define CHECK(c, n) do { if (!(c)) { std::printf("fail %d: %s\n", n, ev.c_str()); return n; } } while (0)

int main() {
    // no description: nothing at all
    reset(); obj[1130] = 0x77;
    inventoryGridDropItem__Fiiciiii(0, 3, 1, 0, 0, 0, 0);
    CHECK(ev.empty() && obj[1130] == 0x77, 1);

    // name table; mode 1 (sound only for mode < 2)
    struct N { long long flags; int b28, b30; const char* name; };
    const N ns[] = {
        {0x800000, 0, 1, "inv_drop_lgwp_wd.anm"}, {0x800000, 0, 4, "inv_drop_lgwp_wd.anm"},
        {0x800000 | 0x20000000, 0, 5, "inv_drop_smwp_wd.anm"}, {0x800000, 0, 0, "inv_drop_mdwp_wd.anm"},
        {0x800000, 0, 5, "inv_drop_mdwp_wd.anm"}, {0x200000, 0, 0, "inv_drop_chain.anm"},
        {0, 3, 0, "inv_drop_sm_shield.anm"}, {0x200000 | 0x400000, 3, 0, "inv_drop_chain.anm"},
        {0x400000, 0, 0, "inv_drop_plate.anm"}, {0x40000, 0, 1, "inv_drop_lgwp_mtl.anm"},
        {0x40000, 0, 3, "inv_drop_lgwp_mtl.anm"}, {0x40000 | 0x20000000, 0, 4, "inv_drop_smwp_mtl.anm"},
        {0x40000, 0, 4, "inv_drop_mdwp_mtl.anm"}, {0x100000, 0, 0, "inv_drop_leather.anm"},
        {0x80000, 0, 0, "inv_drop_cloth.anm"}, {0x1000000, 0, 0, "inv_drop_potion.anm"},
        {0, 0, 0, "inv_drop_jewelry.anm"}, {0x4000000, 0, 0, "inv_drop_jewelry.anm"},
        {0x2000000, 0, 0, "inv_drop_jewelry.anm"}, {0x100000 | 0x80000, 0, 0, "inv_drop_leather.anm"},
    };
    int n = 100;
    for (const N& t : ns) {
        reset(); descBytes[0][28] = (unsigned char)t.b28; descBytes[0][30] = (unsigned char)t.b30;
        makeItem(0, 0, 0, 0, t.flags, 5);
        inventoryGridDropItem__Fiiciiii(0, 0, 0, 0, 0, 0, 1);
        CHECK(soundName == t.name && lumpName == "fx.lmp", n);
        CHECK(ev.find(std::string("FIND:fx.lmp:") + t.name + ";PLAY111;") != std::string::npos, n + 1000);
        ++n;
    }
    // mode 2: no name, no sound, no netPlayerIsLocal before the quantity step; byte at obj+1130 cleared in every mode
    reset(); makeItem(0, 0, 0, 0, 0x800000, 5); obj[1130] = 9;
    inventoryGridDropItem__Fiiciiii(0, 0, 0, 0, 0, 0, 2);
    CHECK(ev.find("SPRINTF") == std::string::npos && ev.find("FIND") == std::string::npos && obj[1130] == 0, 200);
    // remote player: name built, no sound
    reset(); local = 0; makeItem(0, 0, 0, 0, 0, 5);
    inventoryGridDropItem__Fiiciiii(0, 0, 0, 0, 0, 0, 1);
    CHECK(ev.find("SPRINTF") == 0 && ev.find("FIND") == std::string::npos, 201);

    // ordinary item: quantity 5 -> 4 (taken 1); mode 1: no spawn, no removal
    reset(); makeItem(0, 1, 2, 0, 0, 5);
    inventoryGridDropItem__Fiiciiii(0, 2, 1, 0, 0, 0, 3);
    CHECK(slotOf(0, 1, 2)->quantity == 4 && ev == "LOCAL0;WEIGHT0;ABIL;", 210);
    // byte 28 == 8: takes min(count, 20)
    reset(); descBytes[0][28] = 8; makeItem(0, 0, 0, 0, 0, 30);
    inventoryGridDropItem__Fiiciiii(0, 0, 0, 0, 0, 0, 5);
    CHECK(slotOf(0, 0, 0)->quantity == 10, 211);
    reset(); descBytes[0][28] = 8; makeItem(0, 0, 0, 0, 0, 12);
    inventoryGridDropItem__Fiiciiii(0, 0, 0, 0, 0, 0, 5);
    CHECK(slotOf(0, 0, 0)->quantity == 0 && ev.find("REMOVE") != std::string::npos, 212);
    // name contains "spear"
    reset(); std::strcpy(names[0], "a spear"); makeItem(0, 0, 0, 0, 0, 7);
    inventoryGridDropItem__Fiiciiii(0, 0, 0, 0, 0, 0, 5);
    CHECK(slotOf(0, 0, 0)->quantity == 0, 213);

    // grid scan: records whose first word is 0xABCD lose the quantity; table position by group and byte 29
    reset(); descBytes[0][29] = 2; makeItem(0, 0, 0, 0, 0, 9);
    unsigned char* tbl = gridMem + 145800 * 1 + 48600 * 2;
    for (int r : {0, 74}) for (int c : {0, 8}) { unsigned v = 0xABCD; std::memcpy(tbl + r * 72 + c * 5400, &v, 4); *(short*)(tbl + r * 72 + c * 5400 + 4) = 100; }
    unsigned other = 0xABCD; std::memcpy(gridMem + 145800 * 0 + 48600 * 2, &other, 4); *(short*)(gridMem + 48600 * 2 + 4) = 100;
    inventoryGridDropItem__Fiiciiii(0, 0, 0, 0, 0, 1, 5);
    CHECK(compareHits == 75 * 9, 220);
    CHECK(*(short*)(tbl + 4) == 99 && *(short*)(tbl + 74 * 72 + 8 * 5400 + 4) == 99 && *(short*)(gridMem + 48600 * 2 + 4) == 100, 221);
    // invGrid zero: no scan
    reset(); invGrid = 0; makeItem(0, 0, 0, 0, 0, 9);
    inventoryGridDropItem__Fiiciiii(0, 0, 0, 0, 0, 0, 5);
    CHECK(compareHits == 0, 222);
    // remote: no scan
    reset(); local = 0; makeItem(0, 0, 0, 0, 0, 9);
    inventoryGridDropItem__Fiiciiii(0, 0, 0, 0, 0, 0, 5);
    CHECK(compareHits == 0, 223);

    // mode 0: spawn at the first free spot of the object position, count set
    reset(); makeItem(0, 0, 1, 0, 0, 9); std::memset(propBuf, 0, sizeof propBuf);
    inventoryGridDropItem__Fiiciiii(0, 1, 0, 0, 0, 0, 0);
    CHECK(ev == "SPRINTF:inv_drop_jewelry.anm;LOCAL0;FIND:fx.lmp:inv_drop_jewelry.anm;PLAY111;LOCAL0;SPOT(10,20,30,48,12,1);ALLOC400;CTOR(1,11,21,31,0);WEIGHT0;ABIL;", 230);
    CHECK(*(short*)(propBuf + 332) == 1 && slotOf(0, 0, 1)->quantity == 8, 231);
    reset(); allocFail = 1; makeItem(0, 0, 1, 0, 0, 9); std::memset(propBuf, 0, sizeof propBuf);
    inventoryGridDropItem__Fiiciiii(0, 1, 0, 0, 0, 0, 0);
    CHECK(ev.find("CTOR") == std::string::npos && *(short*)(propBuf + 332) == 0, 232);
    // mode 0 with a stack: 30 spears -> prop count 20
    reset(); descBytes[0][28] = 8; makeItem(0, 0, 1, 0, 0, 30); std::memset(propBuf, 0, sizeof propBuf);
    inventoryGridDropItem__Fiiciiii(0, 1, 0, 0, 0, 0, 0);
    CHECK(*(short*)(propBuf + 332) == 20 && slotOf(0, 0, 1)->quantity == 10, 233);

    // removal: last item; grid cells matching are cleared except the dragged cell which is cleared unconditionally
    reset(); descBytes[0][29] = 1; unsigned it = makeItem(1, 2, 5, 0, 0, 1); (void)it;
    tbl = gridMem + 145800 * 1 + 48600 * 1;
    for (int r : {3, 4}) for (int c : {2, 6}) { unsigned v = 0xABCD; std::memcpy(tbl + r * 72 + c * 5400, &v, 4); }
    unsigned v1 = 0x1111; std::memcpy(tbl + 4 * 72 + 5 * 5400, &v1, 4);   // dragged cell holds something else
    inventoryGridDropItem__Fiiciiii(1, 5, 2, 6, 3, 1, 5);
    CHECK(slotOf(1, 2, 5)->quantity == 0, 240);
    CHECK(ev.find("REMOVE(1,5,1,1);ANIM1,264,0;WEIGHT1;ABIL;") != std::string::npos, 241);
    unsigned got;
    std::memcpy(&got, tbl + 3 * 72 + 2 * 5400, 4); CHECK(got == 0, 242);
    std::memcpy(&got, tbl + 4 * 72 + 6 * 5400, 4); CHECK(got == 0, 243);
    std::memcpy(&got, tbl + 3 * 72 + 6 * 5400, 4); CHECK(got == 0, 244);   // the dragged cell itself
    reset(); descBytes[0][29] = 1; makeItem(1, 2, 5, 0, 0, 1);
    tbl = gridMem + 145800 * 1 + 48600 * 1;
    { unsigned v = 0xABCD; std::memcpy(tbl + 3 * 72 + 6 * 5400, &v, 4); std::memcpy(tbl + 9 * 72 + 1 * 5400, &v, 4); }
    inventoryGridDropItem__Fiiciiii(1, 5, 2, 6, 3, 1, 5);
    std::memcpy(&got, tbl + 3 * 72 + 6 * 5400, 4); CHECK(got == 0, 245);
    std::memcpy(&got, tbl + 9 * 72 + 1 * 5400, 4); CHECK(got == 0, 246);
    // the scan skips the dragged cell: with the skip, compare is called once less (75*9 - 1) in the removal scan
    reset(); descBytes[0][29] = 1; makeItem(1, 2, 5, 0, 0, 1);
    inventoryGridDropItem__Fiiciiii(1, 5, 2, 6, 3, 1, 5);
    CHECK(compareHits == 675 + 674, 247);
    // quantity still positive: no removal
    reset(); makeItem(1, 2, 5, 0, 0, 3);
    inventoryGridDropItem__Fiiciiii(1, 5, 2, 6, 3, 1, 5);
    CHECK(ev.find("REMOVE") == std::string::npos && ev.find("ANIM") == std::string::npos, 248);
    // no grid, remote: removal still happens, nothing zeroed
    reset(); invGrid = 0; makeItem(1, 2, 5, 0, 0, 1);
    inventoryGridDropItem__Fiiciiii(1, 5, 2, 6, 3, 1, 5);
    CHECK(ev.find("REMOVE(1,5,0,1)") != std::string::npos, 249);

    // equipped-state handling: state 7 and slot word A
    reset(); unsigned me = makeItem(0, 0, 0, 0, 0, 5);
    rec(0)[23582] = 7; w(0, 21960) = me; w(0, 21968) = 0x1234; w(0, 21896) = 5; w(0, 21900) = 6;
    inventoryGridDropItem__Fiiciiii(0, 0, 0, 0, 0, 0, 5);
    CHECK(ev.find("SEND(77,1,0,0,4660,8);") != std::string::npos, 250);
    CHECK(w(0, 21960) == 0 && w(0, 21896) == 0 && w(0, 21900) == 0 && w(0, 21968) == 0, 251);
    CHECK(rec(0)[23582] == 7, 252);
    // state 7 and slot word B
    reset(); me = makeItem(1, 0, 0, 0, 0, 5);
    rec(1)[23582] = 7; w(1, 21960) = 0x9999; w(1, 21968) = me; w(1, 21900) = 6;
    inventoryGridDropItem__Fiiciiii(1, 0, 0, 0, 0, 0, 5);
    CHECK(ev.find("RESET1;ANIM1,264,0;") != std::string::npos && rec(1)[23582] == 0 && w(1, 21968) == 0 && w(1, 21900) == 0 && w(1, 21960) == 0x9999, 253);
    // state 7 and neither: unchanged
    reset(); me = makeItem(1, 0, 0, 0, 0, 5);
    rec(1)[23582] = 7; w(1, 21960) = 0x9999; w(1, 21968) = 0x8888;
    inventoryGridDropItem__Fiiciiii(1, 0, 0, 0, 0, 0, 5);
    CHECK(ev.find("RESET") == std::string::npos && ev.find("SEND") == std::string::npos && w(1, 21960) == 0x9999, 254);
    // not state 7: slot words ignored in that step
    reset(); me = makeItem(1, 0, 0, 0, 0, 5);
    rec(1)[23582] = 3; w(1, 21960) = me;
    inventoryGridDropItem__Fiiciiii(1, 0, 0, 0, 0, 0, 5);
    CHECK(ev.find("SEND") == std::string::npos, 255);
    // selected equip word (byte 23572 = 1 selects +21964): state becomes 5, animations reset
    reset(); me = makeItem(0, 0, 0, 0, 0, 5);
    rec(0)[23572] = 1; w(0, 21964) = me; rec(0)[23582] = 2;
    inventoryGridDropItem__Fiiciiii(0, 0, 0, 0, 0, 0, 5);
    CHECK(rec(0)[23582] == 5 && ev.find("RESET0;") != std::string::npos, 256);
    // selected equip word holding another value: unchanged
    reset(); me = makeItem(0, 0, 0, 0, 0, 5);
    rec(0)[23572] = 1; w(0, 21964) = me + 4; rec(0)[23582] = 2;
    inventoryGridDropItem__Fiiciiii(0, 0, 0, 0, 0, 0, 5);
    CHECK(rec(0)[23582] == 2, 257);
    // negative selector byte reads a word below +21960
    reset(); me = makeItem(0, 0, 0, 0, 0, 5);
    rec(0)[23572] = (unsigned char)-1; w(0, 21956) = me; rec(0)[23582] = 2;
    inventoryGridDropItem__Fiiciiii(0, 0, 0, 0, 0, 0, 5);
    CHECK(rec(0)[23582] == 5, 258);
    return 0;
}
