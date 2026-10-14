// Host check for removeItemFromInventory__Fiiii. Record words are 32-bit
// tokens or the low 32 bits of host addresses, so every equality the routine
// makes is checked against those. Callees are recorded by the stubs below.
#define NORRATH_INVENTORY_HOST 1
#include "remove.cpp"
#include <cstdio>
#include <cstring>

extern "C" unsigned char player[23984 * 2] = {};
extern "C" signed char interfaceOn = 1;
extern "C" unsigned invGrid = 0;

static int resets, recomputes, locals;
static int local_result = 1;
static int compare_calls;
extern "C" void resetPlayerAnimations__Fi(int p) { resets = resets * 10 + p + 1; }
extern "C" int netPlayerIsLocal__Fi(int p) { ++locals; return local_result && p == 0; }
extern "C" void playerRecomputeAbilityModifiers__Fv() { ++recomputes; }
extern "C" int compareToItem__Q24item11descriptionPQ24item11description(const void* left, const void* right) {
    ++compare_calls;
    unsigned a, b;
    std::memcpy(&a, left, 4);
    std::memcpy(&b, right, 4);
    return a == b && a != 0;
}

using namespace inventory_items;
static const void* table[64];
static unsigned put(const void* object) {
    for (unsigned i = 0; i < 64; ++i) if (table[i] == nullptr) { table[i] = object; return i + 1; }
    return 0;
}
namespace inventory_items { const void* host_resolve(unsigned word) { return table[word - 1]; } }

static Description descs[8];
static unsigned token[8];
static Slot* grid;

static unsigned char* rec(int owner) { return player + owner * 23984; }
static unsigned& w(int owner, unsigned offset) { return *reinterpret_cast<unsigned*>(rec(owner) + offset); }
static List* lists(int owner) { return player_lists(owner); }
static void reset_all() {
    std::memset(player, 0, sizeof player);
    resets = recomputes = locals = compare_calls = 0;
    local_result = 1;
    invGrid = 0;
    for (int i = 0; i < 8; ++i) { std::memset(&descs[i], 0, sizeof descs[i]); }
}
// Make a list with n slots, slot i using description kinds[i] (type kinds[i]).
static void fill(int owner, int list, int n, const int* kinds) {
    List* l = lists(owner) + list;
    for (int i = 0; i < n; ++i) {
        l->slots[i].description = token[kinds[i]];
        l->slots[i].quantity = static_cast<short>(100 + i);
        l->slots[i].index = static_cast<short>(i);
    }
    l->count = n;
}
#define CHECK(c, code) do { if (!(c)) { std::printf("fail %d\n", code); return code; } } while (0)

int main() {
    static Slot grid_store[2 * 3 * 675 + 8];
    grid = grid_store;
    for (int i = 0; i < 8; ++i) { descs[i].type = static_cast<signed char>(i); token[i] = put(&descs[i]); }
    const unsigned grid_token = put(grid_store);
    descs[5].type = 5; descs[6].type = 6;

    // Test 1: remove slot 1 of 4 in list 1; kinds are the types 0, 2, 3, 5.
    reset_all();
    for (int i = 0; i < 8; ++i) descs[i].type = static_cast<signed char>(i);
    int kinds[4] = {0, 2, 3, 5};
    fill(0, 1, 4, kinds);
    List* l = lists(0) + 1;
    unsigned a0 = word_of(&l->slots[0]), a1 = word_of(&l->slots[1]), a2 = word_of(&l->slots[2]), a3 = word_of(&l->slots[3]);
    w(0, 21896 + 2 * 4) = a1;   // equipped word of the removed item (type 2)
    w(0, 21896 + 3 * 4) = a2;   // moves to slot 1
    w(0, 21916) = a3;           // type 5 word, moves to slot 2
    w(0, 21960) = a2;           // moves
    w(0, 21964) = a3;           // moves
    w(0, 21968) = a3;           // moves
    w(0, 21896) = a0;           // stays
    rec(0)[23572] = 0;
    removeItemFromInventory__Fiiii(0, 1, 1, 0);
    CHECK(l->count == 3, 1);
    CHECK(l->slots[0].description == token[0] && l->slots[1].description == token[3] && l->slots[2].description == token[5], 2);
    CHECK(l->slots[3].description == 0, 3);
    CHECK(l->slots[0].quantity == 100 && l->slots[1].quantity == 102 && l->slots[2].quantity == 103, 4);
    CHECK(l->slots[0].index == 0 && l->slots[1].index == 1 && l->slots[2].index == 2, 5);
    CHECK(w(0, 21896 + 2 * 4) == 0, 6);
    CHECK(w(0, 21896 + 3 * 4) == a1, 7);
    CHECK(w(0, 21916) == a2, 8);
    CHECK(w(0, 21960) == a1 && w(0, 21964) == a2 && w(0, 21968) == a2, 9);
    CHECK(w(0, 21896) == a0, 10);
    CHECK(recomputes == 1 && resets == 0 && rec(0)[23582] == 0, 11);
    CHECK(locals == 1 && compare_calls == 0, 12);

    // Test 2: type 6 clears the second shared word; the selected hand resets.
    reset_all();
    for (int i = 0; i < 8; ++i) descs[i].type = static_cast<signed char>(i);
    int kinds2[2] = {6, 1};
    fill(0, 0, 2, kinds2);
    l = lists(0);
    a0 = word_of(&l->slots[0]);
    w(0, 21916) = 0x1111; w(0, 21920) = a0; w(0, 21960) = 0x2222; w(0, 21964) = a0;
    rec(0)[23572] = 1;
    removeItemFromInventory__Fiiii(0, 0, 0, 0);
    CHECK(w(0, 21916) == 0x1111 && w(0, 21920) == 0, 20);
    CHECK(resets == 1 && rec(0)[23582] == 5, 21);
    CHECK(w(0, 21964) == 0 && w(0, 21960) == 0x2222, 22);
    CHECK(l->count == 1 && l->slots[0].description == token[1] && l->slots[1].description == 0, 23);

    // Test 3: removing the last slot only zeroes it; negative byte selects backwards.
    reset_all();
    for (int i = 0; i < 8; ++i) descs[i].type = static_cast<signed char>(i);
    int kinds3[3] = {1, 1, 4};
    fill(1, 2, 3, kinds3);
    l = lists(1) + 2;
    a2 = word_of(&l->slots[2]);
    w(1, 21896 + 16) = a2;
    w(1, 21960 + 2 * 4) = a2;  // reached with selected byte 2
    rec(1)[23572] = 2;
    removeItemFromInventory__Fiiii(1, 2, 2, 0);
    CHECK(l->count == 2 && l->slots[2].description == 0, 30);
    CHECK(l->slots[0].index == 0 && l->slots[1].index == 1, 31);
    CHECK(w(1, 21896 + 16) == 0, 32);
    CHECK(resets == 2 && rec(1)[23582] == 5, 33);
    CHECK(locals == 1 && compare_calls == 0, 34);   // owner 1 is not local

    // Test 4: shop table scan for a local player.
    reset_all();
    for (int i = 0; i < 8; ++i) descs[i].type = static_cast<signed char>(i);
    std::memset(grid_store, 0, sizeof grid_store);
    invGrid = grid_token;
    int kinds4[3] = {1, 2, 3};
    fill(0, 1, 3, kinds4);
    l = lists(0) + 1;
    // group 1, list 1: record row 4 column 2 and row 74 column 8
    Slot* e1 = grid_store + 1 * 2025 + 1 * 675 + 2 * 75 + 4;
    Slot* e2 = grid_store + 1 * 2025 + 1 * 675 + 8 * 75 + 74;
    Slot* other = grid_store + 0 * 2025 + 1 * 675 + 2 * 75 + 4;   // wrong group
    e1->description = token[3];   // matches slot 2 of the list after the move
    e2->description = token[1];   // matches slot 0
    other->description = token[1];
    e1->index = -5; e2->index = -5; other->index = -5;
    Slot* base = grid_store + 1 * 2025 + 1 * 675;
    base[0].description = token[1];
    base[0].index = -5;
    base[674].description = token[1];   // the same record as e2
    base[674].index = -5;
    base[-1].description = token[1];   // neighbours outside the group/list block
    base[-1].index = -5;
    base[675].description = token[1];
    base[675].index = -5;
    removeItemFromInventory__Fiiii(0, 1, 1, 1);   // list becomes {1, 3}
    CHECK(compare_calls == 3 * 64, 40);
    CHECK(e1->index == 1, 41);
    CHECK(e2->index == 0, 42);
    CHECK(other->index == -5, 43);
    CHECK(base[0].index == 0 && base[674].index == 0, 47);
    CHECK(base[-1].index == -5 && base[675].index == -5, 48);
    interfaceOn = 0;
    compare_calls = 0;
    fill(0, 1, 3, kinds4);
    removeItemFromInventory__Fiiii(0, 0, 1, 1);
    CHECK(compare_calls == 0, 44);
    interfaceOn = 1;
    invGrid = 0;
    fill(0, 1, 3, kinds4);
    removeItemFromInventory__Fiiii(0, 0, 1, 1);
    CHECK(compare_calls == 0, 45);
    invGrid = grid_token;
    local_result = 0;
    fill(0, 1, 3, kinds4);
    removeItemFromInventory__Fiiii(0, 0, 1, 1);
    CHECK(compare_calls == 0, 46);
    return 0;
}
