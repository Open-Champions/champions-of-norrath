// Observed playerGetPotionsLeft__Fii at 0x001abd68, 312 bytes.
// Arguments: a player number and a selector. The selector picks which half of an
// 18-entry table of item names (a file-local table, potionType.1289 at
// 0x00422b48) is used: a selector of 0 uses entries 9..17 (the nine healing
// potions, "fullhealpotion" down to "leshealingpotion") and any non-zero selector
// uses entries 0..8 (the nine rejuvenation potions). For each of the nine names
// in that half, every slot of the player's third inventory list (record+328 +
// 2*4616; the count is re-read after each name comparison) whose item name equals
// the table name adds its signed halfword quantity (slot+4). The total is stored
// in a four-word array indexed by the player number (selector 1 ->
// manaPotionsLeft at 0x0034b8b8, every other selector -> healthPotionsLeft at
// 0x0034b8a8) and returned.
// Inferred: "health" and "mana" follow the symbol names; the potion names show
// the two groups are healing and rejuvenation potions. Unknown: why selector 2 and
// above read the rejuvenation half yet store into the health array.
#include "../inventory-items-record/record.hpp"
extern "C" int strcmp(const char* left, const char* right);
extern "C" int healthPotionsLeft[4];
extern "C" int manaPotionsLeft[4];

namespace {
const char* const potion_names[18] = {
    "fullrejuvpotion", "giantrejuvpotion", "exrejuvpotion", "greatrejuvpotion", "largerejuvpotion", "rejuvpotion",
    "minorrejuvpotion", "smallrejuvpotion", "lesrejuvpotion", "fullhealpotion", "gianthealpotion",
    "exhealingpotion", "greathealpotion", "largehealpotion", "healingpotion", "minorhealpotion",
    "smallhealpotion", "leshealingpotion"};
}

extern "C" int playerGetPotionsLeft__Fii(int owner, int selector) {
    using namespace inventory_items;
    const List* list = player_lists(owner) + 2;
    const int first = selector != 0 ? 0 : 9;
    int total = 0;
    for (int name = first; name < first + 9; ++name) {
        for (int index = 0; index < list->count; ++index) {
            const Slot& slot = list->slots[index];
            if (strcmp(item_name(slot), potion_names[name]) == 0) total += slot.quantity;
        }
    }
    if (selector == 1) manaPotionsLeft[owner] = total;
    else healthPotionsLeft[owner] = total;
    return total;
}
