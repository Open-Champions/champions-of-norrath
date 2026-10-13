// Host check for playerGetPotionsLeft: group choice, sums, third list only, which array receives the total.
// Includes the source so the record words can be resolved on the host.
#define NORRATH_INVENTORY_HOST 1
#include "player_get_potions_left.cpp"
#include <cstring>
#include <map>
unsigned char player[2 * 23984];
int healthPotionsLeft[4], manaPotionsLeft[4];
namespace { std::map<unsigned, const void*> words; unsigned next = 0x1000; }
static unsigned intern(const void* p) { words[next] = p; next += 0x10; return next - 0x10; }
const void* inventory_items::host_resolve(unsigned w) { return words.at(w); }
struct Row { unsigned name_word; unsigned char rest[28]; };
static void add(int owner, int list, int index, const char* name, short quantity) {
    static Row rows[64]; static unsigned used = 0;
    static char names[64][32];
    Row& row = rows[used]; std::strcpy(names[used], name); ++used;
    row.name_word = intern(names[used - 1]);
    inventory_items::List* l = inventory_items::player_lists(owner) + list;
    l->slots[index].description = intern(&row);
    l->slots[index].quantity = quantity;
    if (l->count <= index) l->count = index + 1;
}
int main() {
    add(1, 2, 0, "healingpotion", 4); add(1, 2, 1, "fullhealpotion", 3); add(1, 2, 2, "rejuvpotion", 5);
    add(1, 2, 3, "lesrejuvpotion", 2); add(1, 2, 4, "healingpotion", -1);
    add(1, 1, 0, "healingpotion", 50);                       // other lists are ignored
    add(0, 2, 0, "minorhealpotion", 7);
    if (playerGetPotionsLeft__Fii(1, 0) != 6 || healthPotionsLeft[1] != 6 || manaPotionsLeft[1] != 0) return 1;
    if (playerGetPotionsLeft__Fii(1, 1) != 7 || manaPotionsLeft[1] != 7 || healthPotionsLeft[1] != 6) return 2;
    if (playerGetPotionsLeft__Fii(1, 2) != 7 || healthPotionsLeft[1] != 7) return 3;   // rejuvenation half, health array
    if (playerGetPotionsLeft__Fii(0, 0) != 7 || healthPotionsLeft[0] != 7) return 4;
    if (playerGetPotionsLeft__Fii(0, 1) != 0 || manaPotionsLeft[0] != 0) return 5;
    return 0;
}
