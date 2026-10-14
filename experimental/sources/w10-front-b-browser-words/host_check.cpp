#include "browser_words.cpp"
#include <cstdio>

int main() {
    int bad = sizeof(slotsCorrupted) != 64 || sizeof(undoQuickLoadStamp) != 64 || sizeof(sortedPlayerRace) != 160;
    bad |= sizeof(sortedPlayerRace) != 10 * 4 * sizeof(int);
    for (int i = 0; i < 64; ++i) bad |= slotsCorrupted[i] != 0 || undoQuickLoadStamp[i] != 0;
    for (int i = 0; i < 40; ++i) bad |= sortedPlayerRace[i] != 0;
    bad |= reinterpret_cast<unsigned long>(undoQuickLoadStamp) % 8 != 0;
    std::printf(bad ? "FAIL\n" : "ok: 64/64/160 bytes, zero, stamp 8-aligned\n");
    return bad;
}
