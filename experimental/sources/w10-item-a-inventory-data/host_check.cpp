// Host check for the inventory data objects. Not an EE plan input.
#include "inventory_data.cpp"
int main() {
    static_assert(sizeof(armorDebug) == 4 && sizeof(healthPotionsLeft) == 16 && sizeof(manaPotionsLeft) == 16 && sizeof(goldAnm) == 16, "sizes");
    if (buySellVertSpacing != 34 || inventoryDrawDelay != 0) return 1;
    for (int i = 0; i < 4; i++) if (armorDebug[i] != 0 || healthPotionsLeft[i] != 0 || manaPotionsLeft[i] != 0) return 2;
    for (int i = 0; i < 16; i++) if (goldAnm[i] != 0) return 3;
    return 0;
}
