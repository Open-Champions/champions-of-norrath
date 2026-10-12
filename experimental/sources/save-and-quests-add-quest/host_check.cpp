#define NORRATH_SQ_HOST 1
#include <cstdio>
#include <cstring>
extern "C" {
int nmPlayers;
unsigned char player[4 * 23984];
}
#include "add_quest.cpp"
static int count_of(int p) { int v; std::memcpy(&v, player + p * 23984 + 21864, 4); return v; }
static char* name_of(int p, int i) { return (char*)player + p * 23984 + 17768 + i * 64; }
static char* desc_of(int p, int i) { return (char*)player + p * 23984 + 17800 + i * 64; }
static void set_count(int p, int v) { std::memcpy(player + p * 23984 + 21864, &v, 4); }
int main() {
    char q1[] = "findAxe", d1[] = "Find the axe";
    char q2[] = "bridge", d2[] = "Cross the bridge";
    nmPlayers = 0;
    addQuestToLog__FPcT0(q1, d1);
    if (count_of(0) != 0) return 1;
    nmPlayers = 2;
    addQuestToLog__FPcT0(q1, d1);
    if (count_of(0) != 1 || count_of(1) != 1) return 2;
    if (std::strcmp(name_of(1, 0), "findAxe") != 0 || std::strcmp(desc_of(1, 0), "Find the axe") != 0) return 3;
    // duplicates are ignored
    addQuestToLog__FPcT0(q1, d2);
    if (count_of(0) != 1 || std::strcmp(desc_of(0, 0), "Find the axe") != 0) return 4;
    addQuestToLog__FPcT0(q2, d2);
    if (count_of(0) != 2 || count_of(1) != 2) return 5;
    if (std::strcmp(name_of(0, 1), "bridge") != 0 || std::strcmp(desc_of(0, 1), "Cross the bridge") != 0) return 6;
    // the first record that has the name ends the whole call: player 1 is not reached
    std::memset(player, 0, sizeof player);
    nmPlayers = 2;
    addQuestToLog__FPcT0(q1, d1);
    set_count(1, 0);
    std::memset(player + 23984 + 17768, 0, 64);
    addQuestToLog__FPcT0(q1, d1);
    if (count_of(1) != 0) return 7;
    // a full log in the first record ends the call too
    std::memset(player, 0, sizeof player);
    set_count(0, 64);
    addQuestToLog__FPcT0(q2, d2);
    if (count_of(0) != 64 || count_of(1) != 0) return 8;
    // a full log in the second record leaves the first updated
    std::memset(player, 0, sizeof player);
    set_count(1, 64);
    addQuestToLog__FPcT0(q2, d2);
    if (count_of(0) != 1 || count_of(1) != 64) return 9;
    // the last slot (index 63) is usable
    std::memset(player, 0, sizeof player);
    set_count(0, 63);
    addQuestToLog__FPcT0(q1, d1);
    if (count_of(0) != 64 || std::strcmp(name_of(0, 63), "findAxe") != 0) return 10;
    std::puts("ok");
    return 0;
}
