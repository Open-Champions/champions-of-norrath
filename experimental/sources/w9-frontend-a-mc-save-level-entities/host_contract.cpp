#include "mc_save_level_entities.cpp"

#include <cstdio>
#include <cstring>

int asyncMode;
int levelFlags;
unsigned char gameOver;
int curAct;
unsigned char* allActs[6];
unsigned char ramSlot[0xb3000];

static int table_index = 1;
static int table_calls;
static int saves;
static char* saved_destination;
static int saved_size;

extern "C" int getSaveTableEntry__FPc(char*) {
    table_calls += 1;
    return table_index;
}
extern "C" int strcmp(const char* left, const char* right) {
    while (*left != 0 && *left == *right) {
        ++left;
        ++right;
    }
    return static_cast<unsigned char>(*left) - static_cast<unsigned char>(*right);
}
extern "C" char* strcpy(char* destination, const char* source) {
    char* start = destination;
    while ((*destination++ = *source++) != 0) {
    }
    return start;
}
extern "C" int saveLevelDataASync__FPci(char* destination, int size) {
    saves += 1;
    saved_destination = destination;
    saved_size = size;
    return 1;
}

static unsigned char* entry(int i) { return ramSlot + 0x1b610 + 24 * i; }
static int word(unsigned char* at) {
    int v;
    std::memcpy(&v, at, 4);
    return v;
}
static void put(unsigned char* at, int v) { std::memcpy(at, &v, 4); }
static void reset() {
    std::memset(ramSlot, 0, sizeof ramSlot);
    asyncMode = 0;
    levelFlags = 0;
    gameOver = 0;
    saves = 0;
    table_calls = 0;
}

int main() {
    static unsigned char act[16];
    allActs[0] = act;
    curAct = 0;
    put(act + 8 + 4, 0x500);
    table_index = 1;

    reset();
    asyncMode = 3;
    if (MC_SaveLevelEntities__FPc(const_cast<char*>("a")) != 0 || table_calls != 0 || saves != 0) return 1;
    reset();
    levelFlags = 0x800;
    if (MC_SaveLevelEntities__FPc(const_cast<char*>("a")) != 0 || table_calls != 0 || saves != 0) return 2;

    reset();
    table_index = -1;
    if (MC_SaveLevelEntities__FPc(const_cast<char*>("a")) != 0 || table_calls != 1 || saves != 0 || entry(0)[0] != 0) return 3;

    // New name in an empty table: recorded with the act entry's size word and saved at the base.
    reset();
    table_index = 1;
    MC_SaveLevelEntities__FPc(const_cast<char*>("lvl1"));
    if (std::strcmp(reinterpret_cast<char*>(entry(0)), "lvl1") != 0 || word(entry(0) + 20) != 0x500) return 4;
    if (saves != 1 || saved_destination != reinterpret_cast<char*>(ramSlot + 0x1b898) || saved_size != 0x500) return 5;

    // Second name follows the first; a repeated first name reuses its entry and keeps its size.
    MC_SaveLevelEntities__FPc(const_cast<char*>("lvl2"));
    if (saved_destination != reinterpret_cast<char*>(ramSlot + 0x1b898 + 0x500)) return 6;
    put(act + 8 + 4, 0x999);
    MC_SaveLevelEntities__FPc(const_cast<char*>("lvl1"));
    if (word(entry(0) + 20) != 0x500 || saved_destination != reinterpret_cast<char*>(ramSlot + 0x1b898) || saved_size != 0x500) return 7;
    if (entry(2)[0] != 0) return 8;

    // gameOver set: the size is 0x88b8 and the table lookup is skipped.
    reset();
    gameOver = 1;
    MC_SaveLevelEntities__FPc(const_cast<char*>("end"));
    if (table_calls != 0 || word(entry(0) + 20) != 0x88b8 || saved_size != 0x88b8) return 9;

    // Too large: the entry is recorded, but nothing is saved.
    reset();
    put(act + 8 + 4, 0xb2000);
    MC_SaveLevelEntities__FPc(const_cast<char*>("big"));
    if (saves != 0 || word(entry(0) + 20) != 0xb2000) return 10;
    // Exactly at the limit still saves: 0x1b898 + size == 0xb29c8.
    reset();
    put(act + 8 + 4, 0xb29c8 - 0x1b898);
    MC_SaveLevelEntities__FPc(const_cast<char*>("edge"));
    if (saves != 1) return 11;
    reset();
    put(act + 8 + 4, 0xb29c8 - 0x1b898 + 1);
    MC_SaveLevelEntities__FPc(const_cast<char*>("edge"));
    if (saves != 0) return 12;

    // A full table of other names falls through to entry 26, past the table.
    reset();
    put(act + 8 + 4, 0x10);
    for (int i = 0; i < 26; ++i) {
        entry(i)[0] = 'n';
        entry(i)[1] = static_cast<unsigned char>('a' + i);
        put(entry(i) + 20, 4);
    }
    MC_SaveLevelEntities__FPc(const_cast<char*>("new"));
    if (std::strcmp(reinterpret_cast<char*>(entry(26)), "new") != 0 || word(entry(26) + 20) != 0x10) return 13;
    if (saves != 1 || saved_destination != reinterpret_cast<char*>(ramSlot + 0x1b898 + 26 * 4)) return 14;
    std::puts("mc save level entities passed");
    return 0;
}
