// Observed: displayItems__FiiP12screenStructi at 0x001bc140 is 124 bytes and returns nothing.
// With the four arguments a, b, screen, d it calls, in order:
//  1. shopInventoryGridRun__FiiiP12screenStruct(b, d, a, screen);
//  2. netPlayerIsLocal__Fi(b); when that returns nonzero,
//     shopInventoryGridDraw__FiiiP12screenStruct(b, d, a, screen).
// Inferred: b is a player index (it is the argument of the local-player test), so the grid is
// drawn only for a player on this console.
// Unknown: what a and d mean; callers pass d = 3 for the inventory screen.

extern "C" {
void shopInventoryGridRun__FiiiP12screenStruct(int player, int third, int first, void* screen);
int netPlayerIsLocal__Fi(int player);
void shopInventoryGridDraw__FiiiP12screenStruct(int player, int third, int first, void* screen);
}

extern "C" void displayItems__FiiP12screenStructi(int a, int b, void* screen, int d) {
    shopInventoryGridRun__FiiiP12screenStruct(b, d, a, screen);
    if (netPlayerIsLocal__Fi(b)) shopInventoryGridDraw__FiiiP12screenStruct(b, d, a, screen);
}
