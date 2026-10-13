// Host check for inventorySyncQuestItems__Fi. removeItemFromInventory shifts
// later slots down like the real one; additions are recorded. Not an EE plan input.
#define NORRATH_INVENTORY_HOST 1
#include "sync_quest_items.cpp"
#include <cstring>
#include <string>
unsigned char player[4 * 23984];
int nmPlayers;
namespace inventory_items { const void* host_resolve(unsigned) { return nullptr; } }
static std::string ev;
using namespace inventory_items;
static void setflag(int p, int k, bool on) {
    unsigned long long v = 0;
    unsigned char* at = reinterpret_cast<unsigned char*>(&player_lists(p)[2].slots[k]) + 8;
    std::memcpy(&v, at, 8);
    v = on ? (v | 0x8000000000ull) : (v & ~0x8000000000ull);
    std::memcpy(at, &v, 8);
}
static void tag(int p, int k, short q) { player_lists(p)[2].slots[k].quantity = q; }
extern "C" {
void removeItemFromInventory__Fiiii(int p, int k, int list, int g) {
    ev += "R" + std::to_string(p) + "," + std::to_string(k) + "," + std::to_string(list) + "," + std::to_string(g) + ";";
    List& l = player_lists(p)[2];
    for (int j = k; j < l.count - 1; ++j) l.slots[j] = l.slots[j + 1];
    --l.count;
}
void addItemToInventory__FiPQ24item11descriptioni(int p, void* slot, int n) {
    ev += "A" + std::to_string(p) + ":" + std::to_string(static_cast<Slot*>(slot)->quantity) + "," + std::to_string(n) + ";";
}
}
int main() {
    nmPlayers = 3;
    List* src = &player_lists(1)[2];
    src->count = 3;
    for (int k = 0; k < 3; ++k) { tag(1, k, 10 + k); setflag(1, k, k != 1); }
    List* a = &player_lists(0)[2];
    a->count = 4;
    for (int k = 0; k < 4; ++k) { tag(0, k, 20 + k); setflag(0, k, k == 0 || k == 1); }   // two adjacent flagged slots
    List* b = &player_lists(2)[2];
    b->count = 1;
    tag(2, 0, 30); setflag(2, 0, false);
    inventorySyncQuestItems__Fi(1);
    // player 0: slot 0 removed, the old slot 1 shifts into 0 and is removed too
    if (ev != "R0,0,2,0;R0,0,2,0;A0:10,1;A0:12,1;A2:10,1;A2:12,1;") return 1;
    if (a->count != 2 || a->slots[0].quantity != 22 || a->slots[1].quantity != 23) return 2;
    if (b->count != 1 || src->count != 3) return 3;
    ev.clear(); nmPlayers = 0;
    inventorySyncQuestItems__Fi(1);
    if (!ev.empty()) return 4;
    return 0;
}
