// Observed amxRemovePlayerItem__FP5__amxPi at 0x00139ad0, 248 bytes. Cell 1
// is a player index and cell 2 is an AMX item name. Index -1 walks every
// player from 0 to nmPlayers. inventoryGetItemIndex fills an index and a
// count. A found index other than -1 is removed with a zero fourth
// argument. The native returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" int nmPlayers;
extern "C" void inventoryGetItemIndex__FiPcPiT2(int player, char* name, int* index, int* count);
extern "C" void removeItemFromInventory__Fiiii(int player, int index, int count, int mode);

extern "C" int amxRemovePlayerItem__FP5__amxPi(void* amx, int* params) {
    char name[160];
    int index = -1;
    int count = 0;
    int* physical = nullptr;
    amx_GetAddr(amx, params[2], &physical);
    amx_GetString(name, physical);
    const int player = params[1];
    if (player != -1) {
        inventoryGetItemIndex__FiPcPiT2(player, name, &index, &count);
        if (index != -1) removeItemFromInventory__Fiiii(player, index, count, 0);
        return 0;
    }
    int cursor = 0;
    if (nmPlayers <= 0) return 0;
    do {
        inventoryGetItemIndex__FiPcPiT2(cursor, name, &index, &count);
        const int found = index;
        if (found != -1) removeItemFromInventory__Fiiii(cursor, found, count, 0);
        cursor += 1;
    } while (cursor < nmPlayers);
    return 0;
}
