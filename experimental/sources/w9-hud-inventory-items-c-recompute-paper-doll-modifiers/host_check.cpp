// Host check for recomputePaperDollModifiers__Fi (includes the source; build the file alone).
#define NORRATH_INVENTORY_HOST 1
#include "recompute_paper_doll_modifiers.cpp"
#include <cstring>
#include <vector>
unsigned char player[2 * 23984];
static const void* table[256]; static unsigned used = 0;
static unsigned intern(const void* p) { table[used] = p; return 0x1000 + 0x10 * used++; }
const void* inventory_items::host_resolve(unsigned w) { return table[(w - 0x1000) / 0x10]; }
static std::vector<const void*> bonus_calls; static int bonus_value = 0;
extern "C" int getArmorBonus__4itemPQ24item11description(const void* s) { bonus_calls.push_back(s); return bonus_value; }
alignas(8) static unsigned char object[512];
struct Item { unsigned char row[40]; unsigned char slot[72]; unsigned slot_word; };
static Item items[4];
static void set_pair(Item& it, int j, short id, short value) {
    *reinterpret_cast<short*>(it.slot + 36 + 8 * j) = id; *reinterpret_cast<short*>(it.slot + 38 + 8 * j) = value;
}
static unsigned& rw(unsigned char* b, unsigned o) { return *reinterpret_cast<unsigned*>(b + o); }
static unsigned char* rec() { return player + 23984; }
static unsigned obj_word;
static void reset() {
    std::memset(player, 0, sizeof player); std::memset(object, 0x55, sizeof object); bonus_calls.clear(); bonus_value = 0;
    rw(rec(), 21872) = obj_word;
}
static unsigned link(Item& it) {   // connects slot -> row, returns the slot word
    unsigned row_word = intern(it.row), slot_word = intern(it.slot);
    std::memcpy(it.slot, &row_word, 4);
    return slot_word;
}
int main() {
    obj_word = intern(object);
    reset();
    Item& a = items[0]; Item& b = items[1]; Item& c = items[2];
    a.row[29] = 1; set_pair(a, 0, 0, 100); set_pair(a, 1, 13, 40); set_pair(a, 2, 5, 7); set_pair(a, 3, 3, -50);   // armor row
    b.row[29] = 2; set_pair(b, 0, 1, 250); set_pair(b, 1, 13, 9); set_pair(b, 2, 6, -4);                           // non-armor
    c.row[29] = 2; c.row[30] = 4; set_pair(c, 0, 13, 1000);
    rw(rec(), 21896 + 4 * 0) = link(a); rw(rec(), 21896 + 4 * 5) = link(b); rw(rec(), 21896 + 4 * 3) = link(c);
    rw(rec(), 21960) = rw(rec(), 21896 + 4 * 3);            // held item is c: row byte 30 is 4, so index 3 is skipped
    rw(rec(), 152) = 21; bonus_value = 30;
    const int got = recomputePaperDollModifiers__Fi(1);
    float* stats = reinterpret_cast<float*>(object + 88);
    if (bonus_calls.size() != 1 || bonus_calls[0] != a.slot) return 1;
    if (got != 30 + 9 + 10) return 2;                      // armor bonus + b's id 13 + 21/2 (rows with id 13 on armor are not added)
    if (stats[0] != 100 * 0.01f || stats[1] != 250 * 0.01f || stats[2] != 0 || stats[3] != -50 * 0.01f || stats[4] != 0) return 3;
    if (*reinterpret_cast<float*>(rec() + 176) != 7.0f || *reinterpret_cast<float*>(rec() + 200) != -4.0f) return 4;
    // Index 3 (item c) is only visited when no held item with row byte 30 == 4 is selected.
    reset();
    rw(rec(), 21896 + 4 * 3) = link(c); rw(rec(), 152) = -7;
    if (recomputePaperDollModifiers__Fi(1) != 1000 - 3) return 5;          // -7 / 2 rounds toward zero
    rw(rec(), 21960) = rw(rec(), 21896 + 4 * 3);                            // held = c itself, selected byte 0
    if (recomputePaperDollModifiers__Fi(1) != -3) return 6;
    rec()[23572] = 1; if (recomputePaperDollModifiers__Fi(1) != 1000 - 3) return 7;   // held word 0 at selector 1
    // No object: result 0 and nothing written.
    reset(); rw(rec(), 21872) = 0; if (recomputePaperDollModifiers__Fi(1) != 0 || object[88] != 0x55) return 8;
    return 0;
}
