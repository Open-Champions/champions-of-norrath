#include "load_ram_slot.cpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

unsigned char ramSlot[200000];
void* gameCam = &ramSlot;
unsigned char levelSeeds[4096];
int gameAutoMapMode, nmPlayers;
signed char playerStartYaw[8];
unsigned char playerStartPos[48];
unsigned char player[4 * 23984];
signed char keepPlayerPositions;
int gameDifficulty, curPlaySeconds, gameNetReplicateQuestItems, saveCheckPointCount;
char levelName[128], loadName[128];
static std::string events;

extern "C" {
int Load__6CameraPc(void*, unsigned char* b) { events += "cam" + std::to_string(b - ramSlot) + ";"; return 42; }
int jumpGateLoad__FPc(unsigned char* b) { events += "jg" + std::to_string(b - ramSlot) + ";"; return 17; }
void convertIndiciesToPointers__Fi(int n) { events += "conv" + std::to_string(n) + ";"; }
void scriptClearAllGameState__Fv() { events += "clear;"; }
void scriptSetGameState__FPci(unsigned char* n, int v) {
    events += std::string("set ") + reinterpret_cast<char*>(n) + "=" + std::to_string(v) + ";";
}
void inventorySyncQuestItems__Fi(int n) { events += "sync" + std::to_string(n) + ";"; }
}

static void wr(int off, const void* d, int n) { std::memcpy(ramSlot + off, d, n); }

int main() {
    for (int i = 0; i < 200000; ++i) ramSlot[i] = 0;
    std::strcpy(reinterpret_cast<char*>(ramSlot), "cave");
    std::strcpy(reinterpret_cast<char*>(ramSlot) + 20, "deep");
    int secs = 777, diff = 2;
    wr(100, &secs, 4);
    ramSlot[108] = static_cast<unsigned char>(diff);
    const int seeds = 112 + 42 + 17;
    for (int i = 0; i < 4096; ++i) ramSlot[seeds + i] = static_cast<unsigned char>(i * 3);
    int mode = 5, count = 2;
    wr(seeds + 4096, &mode, 4);
    wr(seeds + 4100, &count, 4);
    int e0 = seeds + 4104;
    ramSlot[e0] = 0xff; ramSlot[e0 + 1] = 3;
    for (int i = 0; i < 12; ++i) ramSlot[e0 + 2 + i] = static_cast<unsigned char>(100 + i);
    for (int i = 0; i < 23984; ++i) ramSlot[e0 + 14 + i] = static_cast<unsigned char>(i + 1);
    int e1 = e0 + 23998;
    ramSlot[e1] = 9; ramSlot[e1 + 1] = 8;
    ramSlot[e1 + 14 + 21872] = 0x55;  // would be zeroed after the copy
    int rec = (e1 + 23998 + 2 * 23998 + 3) & ~3;
    std::strcpy(reinterpret_cast<char*>(ramSlot) + rec, "kills");
    int val = 12;
    wr(rec + 32, &val, 4);
    std::strcpy(reinterpret_cast<char*>(ramSlot) + rec + 36, "boss");
    gameNetReplicateQuestItems = 1;
    loadRamSlot__Fv();
    if (events != "cam112;jg" + std::to_string(112 + 42) + ";conv0;conv1;clear;set kills=12;set boss=0;sync0;")
        return 1;
    if (levelSeeds[5] != 15 || gameAutoMapMode != 5 || nmPlayers != 2) return 2;
    if (playerStartYaw[0] != -1 || playerStartYaw[1] != 3 || playerStartYaw[2] != 9) return 3;
    if (playerStartPos[0] != 100 || playerStartPos[11] != 111) return 4;
    if (player[0] != 1 || player[23983] != static_cast<unsigned char>(23984)) return 5;
    if (*reinterpret_cast<int*>(player + 21872) != 0 || *reinterpret_cast<int*>(player + 23984 + 21872) != 0) return 6;
    if (keepPlayerPositions != 1 || gameDifficulty != 2 || curPlaySeconds != 777) return 7;
    if (std::strcmp(levelName, "cave_deep") != 0 || levelName[80] != -3) return 8;
    if (std::strcmp(loadName, "cave_deep") != 0) return 9;
    if (gameNetReplicateQuestItems != 0 || saveCheckPointCount != 99) return 10;
    // no sub-name: plain copy
    ramSlot[20] = 0;
    events.clear();
    loadRamSlot__Fv();
    if (std::strcmp(levelName, "cave") != 0 || std::strcmp(loadName, "cave") != 0) return 11;
    std::puts("load ram slot passed");
    return 0;
}
