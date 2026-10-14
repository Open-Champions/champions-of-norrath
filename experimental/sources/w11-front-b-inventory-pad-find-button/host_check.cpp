// Host check for inventoryPadFindButton__Fi. Not an EE plan input.
#include "inventory_pad_find_button.cpp"
#include <cstring>
int nmPlayers;
alignas(8) unsigned char player[4 * 23984];
alignas(8) unsigned char netLocalPad[4 * 88];
static int local_mask;
static int local_calls;
static int asked[8];
extern "C" int netPlayerIsLocal__Fi(int index) {
    asked[local_calls++] = index;
    return (local_mask >> index) & 1;
}
static void life(int p, float v) { std::memcpy(player + 23984 * p + 172, &v, 4); }
static void buttons(int p, int v) { std::memcpy(netLocalPad + 88 * p + 12, &v, 4); }
static void reset() {
    std::memset(player, 0, sizeof player);
    std::memset(netLocalPad, 0, sizeof netLocalPad);
    nmPlayers = 4;
    local_mask = 0xf;
    local_calls = 0;
}
int main() {
    reset();
    for (int p = 0; p < 4; ++p) { life(p, 1.0f); buttons(p, 0); }
    if (inventoryPadFindButton__Fi(0x10) != -1 || local_calls != 4) return 1;
    // first match wins, scanning upward
    buttons(1, 0x30);
    buttons(3, 0x10);
    local_calls = 0;
    if (inventoryPadFindButton__Fi(0x10) != 1) return 2;
    if (local_calls != 2 || asked[0] != 0 || asked[1] != 1) return 3;
    // a zero or negative float disqualifies before the local test is made
    life(1, 0.0f);
    local_calls = 0;
    if (inventoryPadFindButton__Fi(0x10) != 3 || local_calls != 3) return 4;
    life(1, -2.0f);
    if (inventoryPadFindButton__Fi(0x10) != 3) return 5;
    // not local disqualifies
    life(1, 5.0f);
    local_mask = 0xd;
    if (inventoryPadFindButton__Fi(0x10) != 3) return 6;
    // mask is any-bit
    buttons(0, 0x4);
    if (inventoryPadFindButton__Fi(0x6) != 0) return 7;
    if (inventoryPadFindButton__Fi(0x0) != -1) return 8;
    // count bound
    nmPlayers = 3;
    local_mask = 0xf;
    if (inventoryPadFindButton__Fi(0x10) != 1) return 9;
    nmPlayers = 1;
    buttons(0, 0);
    if (inventoryPadFindButton__Fi(0x10) != -1) return 10;
    nmPlayers = 0;
    local_calls = 0;
    if (inventoryPadFindButton__Fi(0xffff) != -1 || local_calls != 0) return 11;
    return 0;
}
