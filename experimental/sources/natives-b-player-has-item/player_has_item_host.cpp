#include <cstdio>
#include <cstring>
extern "C" int amxPlayerHasItem__FP5__amxPi(void*, int*);
extern "C" unsigned char nmPlayers[4] = {};
static int calls = 0;
static int indexes[4];
extern "C" int amx_GetAddr(void*, int, void** physical) {
    static char packed[4];
    *physical = packed;
    return 0;
}
extern "C" int amx_GetString(char* destination, void*) {
    std::strcpy(destination, "gem");
    return 0;
}
extern "C" int queryInventoryItem__FPci(char* name, int index) {
    if (std::strcmp(name, "gem") != 0) return -1;
    indexes[calls] = index;
    const int found = calls == 0 ? 3 : 4;
    if (calls == 0) nmPlayers[0] = 1;
    calls += 1;
    return found;
}

int main() {
    std::memset(nmPlayers, 0, 4);
    int cells[] = {4, 2};
    if (amxPlayerHasItem__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (calls != 0) return 2;
    nmPlayers[0] = 3;
    if (amxPlayerHasItem__FP5__amxPi(nullptr, cells) != 3) return 3;
    if (calls != 1 || indexes[0] != 0) return 4;
    std::puts("amxPlayerHasItem passed");
    return 0;
}
