#define NORRATH_HOST_ARENA
#include "save_checkpoint.cpp"

#include <cstdio>

extern "C" int nmPlayers = 0;
extern "C" {
unsigned char player[23984 * 2];
}
extern "C" int curAct = -1;
extern "C" int actCount = 0;
extern "C" int actLevelCount[5] = {};
extern "C" unsigned char* allActs[5] = {};
extern "C" int playersAlive[4] = {};
extern "C" {
unsigned char MC_SpawnPoint[48];
}
extern "C" int MC_CheckPointAge = 7;

static unsigned char objects[2][40];
extern "C" const unsigned char* norrath_host_objects[3] = {nullptr, objects[0], objects[1]};
extern "C" const char* norrath_host_names[8] = {};

static char filled[64];
static int name_calls;
static int has_saved_calls;
static int has_saved;
static const char* hud_text;
static int hud_a, hud_b, hud_c;
static int hud_calls;
static int updates;
static int update_first;
static int long_calls;
static const char* checkpoint_text = "checkpoint-body";
static const char* long_text = "long-name";

void copy_text(char* to, const char* from) {
    while ((*to++ = *from++) != 0) {}
}

int same_bytes(const unsigned char* left, const unsigned char* right, int count) {
    for (int index = 0; index < count; ++index) {
        if (left[index] != right[index]) return 0;
    }
    return 1;
}

void fill_bytes(unsigned char* at, unsigned char value, int count) {
    for (int index = 0; index < count; ++index) at[index] = value;
}

void store_word(unsigned char* at, unsigned bits) {
    at[0] = static_cast<unsigned char>(bits);
    at[1] = static_cast<unsigned char>(bits >> 8);
    at[2] = static_cast<unsigned char>(bits >> 16);
    at[3] = static_cast<unsigned char>(bits >> 24);
}

void set_player(int index, unsigned object_index, unsigned flags, unsigned float_bits) {
    unsigned char* record = player + index * 23984;
    store_word(record + 172, float_bits);
    store_word(record + 21872, object_index);
    if (object_index != 0) store_word(objects[object_index - 1] + 12, flags);
}

extern "C" void gameGetFullLevelName__FPc(char* name) {
    ++name_calls;
    copy_text(name, filled);
}
extern "C" const char* textGetString__FPc(const char* key) {
    if (key[0] != 'C' || key[9] != 't' || key[10] != 0) return "bad-key";
    return checkpoint_text;
}
extern "C" const char* getLongLevelName__FPc(const char* name) {
    ++long_calls;
    (void)name;
    return long_text;
}
extern "C" void hudSetInfoMessage__FPUsiic(const char* text, int first, int second, int third) {
    ++hud_calls;
    hud_text = text;
    hud_a = first;
    hud_b = second;
    hud_c = third;
}
extern "C" int MC_HasThisLevelBeenSaved__Fv() {
    ++has_saved_calls;
    return has_saved;
}
extern "C" void updateRamSlot__Fii(int first, int second) {
    ++updates;
    update_first = first;
    if (second != 0) update_first = -100;
}

int main() {
    fill_bytes(player, 0, static_cast<int>(sizeof player));
    fill_bytes(MC_SpawnPoint, 0x5a, static_cast<int>(sizeof MC_SpawnPoint));
    nmPlayers = 1;
    set_player(0, 1, 8, 0x3f800000u);
    MC_SaveCheckPoint__Fiii(3, 1, 9);
    if (name_calls != 0 || updates != 0 || hud_calls != 0) return 1;

    set_player(0, 1, 0, 0x3f800000u);
    for (int index = 0; index < 12; ++index) objects[0][16 + index] = static_cast<unsigned char>(0x10 + index);
    curAct = -1;
    playersAlive[0] = 99;
    MC_CheckPointAge = 4;
    MC_SaveCheckPoint__Fiii(3, 1, 9);
    if (name_calls != 0 || updates != 1 || update_first != 3 || MC_CheckPointAge != 0) return 2;
    if (!same_bytes(MC_SpawnPoint, objects[0] + 16, 12) || MC_SpawnPoint[12] != 0x5a) return 3;

    unsigned char act0[16];
    unsigned char act1[16];
    store_word(act0, 1);
    store_word(act0 + 8, 2);
    store_word(act1, 3);
    norrath_host_names[1] = "crypt";
    norrath_host_names[2] = "other";
    norrath_host_names[3] = "town";
    allActs[0] = act0;
    allActs[1] = act1;
    actLevelCount[0] = 2;
    actLevelCount[1] = 1;
    actCount = 2;
    curAct = 0;
    copy_text(filled, "missing");
    hud_calls = 0;
    updates = 0;
    MC_CheckPointAge = 6;
    MC_SaveCheckPoint__Fiii(8, 0, 1);
    if (name_calls != 1 || hud_calls != 1 || hud_text != checkpoint_text || hud_a != 180 ||
        hud_b != -1 || hud_c != 0 || updates != 0 || MC_CheckPointAge != 6) return 4;

    copy_text(filled, "crypt");
    has_saved = 0;
    has_saved_calls = 0;
    hud_calls = 0;
    fill_bytes(MC_SpawnPoint, 0x5a, static_cast<int>(sizeof MC_SpawnPoint));
    MC_SaveCheckPoint__Fiii(8, 1, 1);
    if (has_saved_calls != 1 || hud_calls != 0 || updates != 1 || update_first != 8) return 5;

    copy_text(filled, "crypt_west");
    has_saved_calls = 0;
    hud_calls = 0;
    long_calls = 0;
    MC_SaveCheckPoint__Fiii(8, 1, 1);
    if (has_saved_calls != 0 || hud_calls != 1 || hud_text != long_text || long_calls != 1) return 6;

    copy_text(filled, "crypt");
    has_saved = 1;
    hud_calls = 0;
    long_calls = 0;
    MC_SaveCheckPoint__Fiii(8, 1, 1);
    if (hud_calls != 1 || hud_text != long_text || long_calls != 1 || updates != 3) return 16;

    hud_calls = 0;
    MC_SaveCheckPoint__Fiii(8, 0, 1);
    if (hud_calls != 1 || hud_text != checkpoint_text || long_calls != 1) return 7;

    copy_text(filled, "nas");
    norrath_host_names[1] = "crypt";
    norrath_host_names[3] = "nas";
    hud_calls = 0;
    has_saved_calls = 0;
    MC_SaveCheckPoint__Fiii(2, 1, 0);
    if (hud_calls != 0 || has_saved_calls != 0 || updates != 5 || update_first != 2) return 8;

    fill_bytes(MC_SpawnPoint, 0x5a, static_cast<int>(sizeof MC_SpawnPoint));
    nmPlayers = 2;
    set_player(0, 0, 0, 0x3f800000u);
    set_player(1, 2, 0, 0x3f800000u);
    for (int index = 0; index < 12; ++index) objects[1][16 + index] = static_cast<unsigned char>(0x30 + index);
    playersAlive[0] = 1;
    curAct = -1;
    updates = 0;
    MC_SaveCheckPoint__Fiii(1, 0, 0);
    if (updates != 1) return 9;
    if (MC_SpawnPoint[0] != 0x5a) return 10;
    if (!same_bytes(MC_SpawnPoint + 12, objects[1] + 16, 12)) return 11;
    std::puts("save checkpoint passed");
    return 0;
}
