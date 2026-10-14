#include "enumerate_alive_players.cpp"
#include <cstring>
// Host build: the record pointer slot is filled with a real host pointer
// (8 bytes, aligned at offset 21872), so only the traversal logic is checked.
unsigned char player[4 * 23984];
int nmPlayers;
int nmPlayersAlive;
int localNmPlayersAlive;
int lastNmLocalPlayersAlive;
int validLocalPlayer;
int playersAlive[4];
int localPlayersAlive[4];
static int localMask;
int netPlayerIsLocal__Fi(int index) { return (localMask >> index) & 1; }

static void setObject(int slot, unsigned char* object) {
    std::memcpy(player + slot * 23984 + 21872, &object, sizeof object);
}

int main() {
    unsigned words[4][4] = {{0}};
    words[1][3] = 8;  // slot 1 flagged: skipped
    words[3][3] = 7;  // bit 3 clear: kept
    setObject(0, reinterpret_cast<unsigned char*>(words[0]));
    setObject(1, reinterpret_cast<unsigned char*>(words[1]));
    setObject(2, 0);  // null: skipped
    setObject(3, reinterpret_cast<unsigned char*>(words[3]));
    nmPlayers = 4;
    localNmPlayersAlive = 3;
    validLocalPlayer = 2;
    localMask = 0x8;
    enumerateAlivePlayers__Fv();
    if (lastNmLocalPlayersAlive != 3) return 1;
    if (nmPlayersAlive != 2 || playersAlive[0] != 0 || playersAlive[1] != 3) return 2;
    if (localNmPlayersAlive != 1 || localPlayersAlive[0] != 3) return 3;
    if (validLocalPlayer != 3) return 4;
    localMask = 0x9;
    enumerateAlivePlayers__Fv();
    if (lastNmLocalPlayersAlive != 1) return 5;
    if (localNmPlayersAlive != 2 || localPlayersAlive[1] != 3 || validLocalPlayer != 0) return 6;
    nmPlayers = 0;
    enumerateAlivePlayers__Fv();
    if (nmPlayersAlive != 0 || localNmPlayersAlive != 0 || validLocalPlayer != -1) return 7;
    return 0;
}
