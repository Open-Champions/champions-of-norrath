#include <cstdio>
#include <cstring>

extern "C" int nmPlayers = 0;
extern "C" int amxRemovePlayerItem__FP5__amxPi(void*, int*);

static int removed_player;
static int removed_index;
static int removed_count;
static int removed_mode;
static int removes;
static int lookups;

extern "C" int amx_GetAddr(void*, int address, int** physical) {
    static int slot;
    if (address != 77) slot = -1;
    *physical = &slot;
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", "potion");
    return 0;
}

extern "C" void inventoryGetItemIndex__FiPcPiT2(int, char* name, int* index, int* count) {
    lookups += 1;
    if (std::strcmp(name, "potion") != 0) {
        *index = -1;
        return;
    }
    *index = 4;
    *count = 2;
}

extern "C" void removeItemFromInventory__Fiiii(int player, int index, int count, int mode) {
    removed_player = player;
    removed_index = index;
    removed_count = count;
    removed_mode = mode;
    removes += 1;
}

int main() {
    int params[3] = {8, 3, 77};
    if (amxRemovePlayerItem__FP5__amxPi(nullptr, params) != 0) return 1;
    if (lookups != 1 || removes != 1) return 2;
    if (removed_player != 3 || removed_index != 4 || removed_count != 2 || removed_mode != 0) return 3;
    params[1] = -1;
    nmPlayers = 2;
    removes = 0;
    lookups = 0;
    if (amxRemovePlayerItem__FP5__amxPi(nullptr, params) != 0) return 4;
    if (lookups != 2 || removes != 2 || removed_player != 1) return 5;
    nmPlayers = 0;
    removes = 0;
    if (amxRemovePlayerItem__FP5__amxPi(nullptr, params) != 0 || removes != 0) return 6;
    std::printf("remove player item passed\n");
    return 0;
}
