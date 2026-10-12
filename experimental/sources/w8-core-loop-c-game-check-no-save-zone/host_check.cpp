// Host contract for gameCheckNoSaveZone. Not an EE plan input.
#include <cstdint>
#include <cstring>
#include "game_check_no_save_zone.cpp"

extern "C" {
int gameRecallInhibit = 0;
int saveOk = 99;
int nmPlayers = 0;
unsigned char player[4 * 23984] __attribute__((aligned(16)));
}

static int dlgActive = 0, dlgCalls = 0, iterateCalls = 0, saveOkAtIterate = -1;
static int clearSaveOk = 0;
static void (*iterateFn)(void*) = nullptr;
static int nmPlayersLowerTo = -1;
extern "C" int DLG_Active__Fv() { ++dlgCalls; return dlgActive; }
extern "C" void checkSave__FP10GameObject(void*) {}
extern "C" void objectIterateRunLists__FPFP10GameObject_v(void (*callback)(void*)) {
    ++iterateCalls;
    iterateFn = callback;
    saveOkAtIterate = saveOk;
    if (clearSaveOk) saveOk = 0;
    if (nmPlayersLowerTo >= 0) nmPlayers = nmPlayersLowerTo;
}

// Player pointers need 32-bit storage on the target; the host check keeps the record view
// in a table of host pointers and writes only the 4 low bytes that the source reads.
struct Block { alignas(8) unsigned char bytes[1340]; };
static Block blocks[4];

static void reset() {
    gameRecallInhibit = 0; dlgActive = 0; dlgCalls = 0; iterateCalls = 0;
    clearSaveOk = 0; nmPlayers = 0; saveOk = 99; saveOkAtIterate = -1; nmPlayersLowerTo = -1;
    std::memset(player, 0, sizeof player);
    std::memset(blocks, 0, sizeof blocks);
}
static void setPlayer(int index, Block* b) {
    unsigned char* slot = player + index * 23984 + 21872;
    std::memcpy(slot, &b, sizeof b);
}
static void setWord(Block& b, int value) { std::memcpy(b.bytes + 1328, &value, 4); }

int main() {
    reset();
    gameRecallInhibit = 5;
    if (gameCheckNoSaveZone__Fv() != 1 || dlgCalls != 0 || iterateCalls != 0) return 1;
    reset();
    dlgActive = 3;
    if (gameCheckNoSaveZone__Fv() != 1 || dlgCalls != 1 || iterateCalls != 0 || saveOk != 99) return 2;
    // No players: result depends on saveOk after the iteration.
    reset();
    if (gameCheckNoSaveZone__Fv() != 0 || iterateCalls != 1 || saveOkAtIterate != 1) return 3;
    if (iterateFn != checkSave__FP10GameObject) return 4;
    reset();
    clearSaveOk = 1;
    if (gameCheckNoSaveZone__Fv() != 1) return 5;
    // Players: a nonzero word behind any present pointer returns 1; zero pointers are skipped.
    reset();
    nmPlayers = 3;
    setPlayer(1, &blocks[1]);
    setPlayer(2, &blocks[2]);
    if (gameCheckNoSaveZone__Fv() != 0) return 6;
    setWord(blocks[2], -1);
    if (gameCheckNoSaveZone__Fv() != 1) return 7;
    // Records at or beyond nmPlayers are not examined.
    reset();
    nmPlayers = 2;
    setPlayer(2, &blocks[2]);
    setWord(blocks[2], 1);
    if (gameCheckNoSaveZone__Fv() != 0) return 8;
    // The count is read after the iteration: a callback that lowers it hides the player,
    // one that raises it from zero exposes the player.
    reset();
    nmPlayers = 3;
    nmPlayersLowerTo = 1;
    setPlayer(2, &blocks[2]);
    setWord(blocks[2], 1);
    if (gameCheckNoSaveZone__Fv() != 0) return 9;
    reset();
    nmPlayers = 0;
    nmPlayersLowerTo = 3;
    setPlayer(2, &blocks[2]);
    setWord(blocks[2], 1);
    if (gameCheckNoSaveZone__Fv() != 1) return 10;
    return 0;
}
