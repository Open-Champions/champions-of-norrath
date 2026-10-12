// Host contract for gameSomeoneInShop__Fv. Not an EE plan input.
extern "C" signed char netNetworkMode = 0;
extern "C" int nmPlayersAlive = 0;
extern "C" int playersAlive[4] = {};
extern "C" { alignas(16) unsigned char player[4 * 23984]; }
#define records player
alignas(16) static unsigned char objs[4][32];
#include "someone_in_shop.cpp"
static void setup(int slot, unsigned flags) {
    unsigned char** at = reinterpret_cast<unsigned char**>(records + slot * 23984 + 21872);
    *at = objs[slot];
    *reinterpret_cast<unsigned*>(objs[slot] + 12) = flags;
}
int main() {
    for (int i = 0; i < 4; ++i) setup(i, 0x7fffffu | 0xff000000u);   // bit 0x800000 clear
    nmPlayersAlive = 3; playersAlive[0] = 2; playersAlive[1] = 0; playersAlive[2] = 3;
    netNetworkMode = 0; setup(0, 0x800000u);
    if (gameSomeoneInShop__Fv() != 0) return 1;
    netNetworkMode = 1;
    if (gameSomeoneInShop__Fv() != 1) return 2;          // listed index 0 is in shop
    setup(0, 0);
    if (gameSomeoneInShop__Fv() != 0) return 3;
    setup(1, 0x800000u);                                  // slot 1 not listed
    if (gameSomeoneInShop__Fv() != 0) return 4;
    setup(3, 0x800000u);                                  // third listed index
    if (gameSomeoneInShop__Fv() != 1) return 5;
    nmPlayersAlive = 2;                                   // third entry beyond the count
    if (gameSomeoneInShop__Fv() != 0) return 6;
    nmPlayersAlive = 0; setup(2, 0x800000u);
    if (gameSomeoneInShop__Fv() != 0) return 7;
    return 0;
}
