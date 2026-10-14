#include <cstring>
extern "C" int nmPlayers;
extern "C" unsigned char player[23984 * 4];
int nmPlayers;
unsigned char player[23984 * 4];
extern "C" void removeAllQuestsFromLog__Fv();

int main() {
    memset(player, 0x5a, sizeof player);
    int counts[4] = {3, 0, 5, 2};
    for (int p = 0; p < 4; p++) memcpy(player + p * 23984 + 21864, &counts[p], 4);
    nmPlayers = 3;
    unsigned char before[sizeof player];
    memcpy(before, player, sizeof player);
    removeAllQuestsFromLog__Fv();
    for (int p = 0; p < 4; p++) {
        unsigned char* rec = player + p * 23984;
        int count; memcpy(&count, rec + 21864, 4);
        int old = counts[p];
        if (p < 3 && count != 0) return 1;
        if (p == 3) {
            if (count != 2 || memcmp(rec, before + p * 23984, 23984)) return 2;
            continue;
        }
        unsigned char want[23984];
        memcpy(want, before + p * 23984, 23984);
        memset(want + 21864, 0, 4);
        for (int s = 0; s < old; s++) {
            memcpy(want + 17768 + s * 64, "NoQuests", 9);
            memcpy(want + 17800 + s * 64, "NoQuests", 9);
        }
        if (memcmp(rec, want, 23984)) return 3 + p;
    }
    // Negative player count does nothing.
    memcpy(before, player, sizeof player);
    nmPlayers = -1;
    removeAllQuestsFromLog__Fv();
    if (memcmp(before, player, sizeof player)) return 10;
    return 0;
}
