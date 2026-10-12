#include <cstdio>
#include <cstring>
extern "C" int amxGivePlayerExp__FP5__amxPi(void*, int*);
extern "C" unsigned char nmPlayers[4] = {};
static int indexes[4];
static int amounts[4];
static int calls = 0;
static int* live_cells;

extern "C" void givePlayerExperience__Fii(int index, int amount) {
    indexes[calls] = index;
    amounts[calls] = amount;
    if (calls == 0) {
        live_cells[1] = 4;
        nmPlayers[0] = 2;
        nmPlayers[1] = nmPlayers[2] = nmPlayers[3] = 0;
    }
    calls += 1;
}

int main() {
    std::memset(nmPlayers, 0, 4);
    int cells[] = {4, 10};
    if (amxGivePlayerExp__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (calls != 0) return 2;
    nmPlayers[0] = 0xff;
    nmPlayers[1] = nmPlayers[2] = nmPlayers[3] = 0xff;
    if (amxGivePlayerExp__FP5__amxPi(nullptr, cells) != 0) return 3;
    if (calls != 0) return 4;
    nmPlayers[0] = 3;
    nmPlayers[1] = nmPlayers[2] = nmPlayers[3] = 0;
    cells[1] = 10;
    live_cells = cells;
    if (amxGivePlayerExp__FP5__amxPi(nullptr, cells) != 0) return 5;
    if (calls != 2) return 6;
    if (indexes[0] != 0 || amounts[0] != 10) return 7;
    if (indexes[1] != 1 || amounts[1] != 4) return 8;
    std::puts("amxGivePlayerExp passed");
    return 0;
}
