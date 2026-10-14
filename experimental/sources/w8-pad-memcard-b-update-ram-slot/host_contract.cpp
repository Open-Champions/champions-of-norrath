#define NORRATH_HOST_ARENA 1
#include "update_ram_slot.cpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
unsigned char gameOver;
int nmPlayers;
int curPlaySeconds;
unsigned char gameDifficulty;
void* gameCam;
unsigned char levelSeeds[4096];
int gameAutoMapMode;
unsigned char player[4 * 23984];
unsigned char tempPlayerStruct[4 * 23984];
int validLocalPlayer;
int actCount;
int actLevelCount[5];
unsigned char* allActs[5];
char levelName[322];
unsigned char ramSlot[4 * 23998 + 4096 + 1024];
const unsigned char* norrath_host_objects[8];
const char* norrath_host_names[8];
}

static int camera_object;
static std::string log_text;
static std::string full_name;
static int local_mask;
static const char* entry_names[3];
static int entry_values[3];
static int entry_count;
static unsigned char objects[4][64];

extern "C" {
void gameGetFullLevelName__FPc(char* name) {
    log_text += "name;";
    std::strcpy(name, full_name.c_str());
}
int Save__6CameraPc(void* camera, unsigned char* out) {
    log_text += camera == &camera_object ? "cam;" : "cam?;";
    for (int index = 0; index < 5; ++index) out[index] = static_cast<unsigned char>(0xc0 + index);
    return 5;
}
int jumpGateSave__FPc(unsigned char* out) {
    log_text += "gate;";
    for (int index = 0; index < 3; ++index) out[index] = static_cast<unsigned char>(0xd0 + index);
    return 3;
}
int netPlayerIsLocal__Fi(int index) { return (local_mask >> index) & 1; }
void convertPointersToIndicies__Fi(int index) { log_text += "conv" + std::to_string(index) + ";"; }
const char* scriptGameStateEntry__FiPi(int index, int* value) {
    if (index >= entry_count) return nullptr;
    *value = entry_values[index];
    return entry_names[index];
}
void MC_SaveLevelEntities__FPc(char* name) { log_text += std::string("ents:") + name + ";"; }
}

static void set_word(unsigned char* at, unsigned value) { std::memcpy(at, &value, 4); }
static unsigned get_word(const unsigned char* at) { unsigned v; std::memcpy(&v, at, 4); return v; }
static int fail(int code) { std::printf("failed %d log=[%s]\n", code, log_text.c_str()); return code; }

static void setup() {
    std::memset(ramSlot, 0xee, sizeof ramSlot);
    std::memset(player, 0, sizeof player);
    for (unsigned index = 0; index < sizeof tempPlayerStruct; ++index)
        tempPlayerStruct[index] = static_cast<unsigned char>(index * 7u + index / 251u);
    for (int index = 0; index < 4096; ++index) levelSeeds[index] = static_cast<unsigned char>(index ^ 0x5a);
    for (int obj = 0; obj < 4; ++obj)
        for (int index = 0; index < 64; ++index)
            objects[obj][index] = static_cast<unsigned char>(obj * 64 + index + 1);
    for (int index = 0; index < 4; ++index) norrath_host_objects[index + 1] = objects[index];
    norrath_host_names[1] = "alpha";
    norrath_host_names[2] = "beta";
    norrath_host_names[3] = "game";
    norrath_host_names[4] = "zz";
    static unsigned char act0[16], act1[8];
    set_word(act0, 1); set_word(act0 + 8, 2);
    set_word(act1, 4);
    allActs[0] = act0;
    allActs[1] = act1;
    actCount = 2;
    actLevelCount[0] = 2;
    actLevelCount[1] = 1;
    std::memset(levelName, 0, sizeof levelName);
    std::strcpy(levelName + 81, "TITLE-ONE");
    std::strcpy(levelName + 161, "TITLE-TWO");
    gameCam = &camera_object;
    gameOver = 0;
    nmPlayers = 3;
    curPlaySeconds = 0x01020304;
    gameDifficulty = 2;
    gameAutoMapMode = 0x0a0b0c0d;
    validLocalPlayer = 2;
    local_mask = 0;
    entry_count = 0;
    full_name = "betaZone";
    log_text.clear();
}

static bool untouched_from(size_t offset) {
    for (size_t index = offset; index < sizeof ramSlot; ++index)
        if (ramSlot[index] != 0xee) return false;
    return true;
}

int main() {
    // No act matches: nothing is written and no entity save runs.
    setup();
    full_name = "other";
    updateRamSlot__Fii(1, 0);
    if (log_text != "name;" || !untouched_from(0)) return fail(1);

    // A prefix of the name matches the second entry of the first act; two players, no object word on one.
    setup();
    entry_names[0] = "e0"; entry_values[0] = 111;
    entry_names[1] = "e1"; entry_values[1] = 222;
    entry_count = 2;
    nmPlayers = 3;
    set_word(player + 21872, 1);                 // player 0 -> objects[0]
    set_word(player + 2 * 23984 + 21872, 3);     // player 2 -> objects[2]
    validLocalPlayer = 2;
    updateRamSlot__Fii(5, 0);
    if (log_text != "name;cam;gate;conv0;conv1;conv2;ents:betaZone;") return fail(2);
    if (std::strcmp(reinterpret_cast<char*>(ramSlot), "TITLE-ONE") != 0) return fail(3);
    if (std::strcmp(reinterpret_cast<char*>(ramSlot + 20), "TITLE-TWO") != 0) return fail(4);
    if (get_word(ramSlot + 100) != 0x01020304u) return fail(5);
    if (ramSlot[104] != (23984 & 255) || ramSlot[105] != (23984 >> 8)) return fail(6);
    if (ramSlot[106] != 3 || ramSlot[107] != 0 || ramSlot[108] != 2 || ramSlot[109] != 5) return fail(7);
    if (ramSlot[110] != 0xee || ramSlot[13] != 0xee) return fail(8);
    for (int index = 0; index < 5; ++index) if (ramSlot[112 + index] != 0xc0 + index) return fail(9);
    for (int index = 0; index < 3; ++index) if (ramSlot[117 + index] != 0xd0 + index) return fail(10);
    if (std::memcmp(ramSlot + 120, levelSeeds, 4096) != 0) return fail(11);
    if (get_word(ramSlot + 120 + 4096) != 0x0a0b0c0du) return fail(12);
    const size_t players = 120 + 4096 + 4;
    if (get_word(ramSlot + players) != 3u) return fail(13);
    size_t at = players + 4;
    // Player 0: own object (bytes 56,57 and 16..27).
    if (ramSlot[at] != objects[0][56] || ramSlot[at + 1] != objects[0][57]) return fail(14);
    if (std::memcmp(ramSlot + at + 2, objects[0] + 16, 12) != 0) return fail(15);
    if (std::memcmp(ramSlot + at + 14, tempPlayerStruct, 23984) != 0) return fail(16);
    at += 23998;
    // Player 1: no object, falls back to validLocalPlayer 2 (objects[2]).
    if (ramSlot[at] != objects[2][56] || std::memcmp(ramSlot + at + 2, objects[2] + 16, 12) != 0) return fail(17);
    if (std::memcmp(ramSlot + at + 14, tempPlayerStruct + 23984, 23984) != 0) return fail(18);
    at += 23998;
    if (ramSlot[at] != objects[2][56]) return fail(19);
    if (std::memcmp(ramSlot + at + 14, tempPlayerStruct + 2 * 23984, 23984) != 0) return fail(20);
    at += 23998;
    // Slot 3 is past nmPlayers: skipped, untouched.
    if (ramSlot[at] != 0xee || ramSlot[at + 23997] != 0xee) return fail(21);
    at += 23998;
    // The entries follow at the next multiple of four; the offsets here are already aligned.
    if ((at & 3) != 0) at += 4 - (at & 3);
    if (std::strcmp(reinterpret_cast<char*>(ramSlot + at), "e0") != 0 || get_word(ramSlot + at + 32) != 111u) return fail(22);
    if (std::strcmp(reinterpret_cast<char*>(ramSlot + at + 36), "e1") != 0 || get_word(ramSlot + at + 68) != 222u) return fail(23);
    if (!untouched_from(at + 72)) return fail(24);

    // validLocalPlayer unusable (negative): the first player with an object is used.
    setup();
    nmPlayers = 2;
    validLocalPlayer = -1;
    set_word(player + 23984 + 21872, 2);  // only player 1 has an object -> objects[1]
    updateRamSlot__Fii(0, 0);
    at = 120 + 4096 + 4 + 4;
    if (ramSlot[at] != objects[1][56] || std::memcmp(ramSlot + at + 2, objects[1] + 16, 12) != 0) return fail(25);
    if (std::memcmp(ramSlot + at + 14, tempPlayerStruct, 23984) != 0) return fail(26);
    if (get_word(ramSlot + at - 4) != 2u || ramSlot[106] != 2) return fail(27);

    // Single-player form (second nonzero): the first local player, count 1, three records skipped.
    setup();
    nmPlayers = 3;
    local_mask = 4;                                // only player 2 is local
    set_word(player + 2 * 23984 + 21872, 4);       // objects[3]
    entry_names[0] = "only"; entry_values[0] = 7; entry_count = 1;
    updateRamSlot__Fii(9, 1);
    if (log_text != "name;cam;gate;conv2;ents:betaZone;") return fail(28);
    if (ramSlot[106] != 1 || ramSlot[107] != 0 || ramSlot[109] != 9) return fail(29);
    at = 120 + 4096 + 4;
    if (get_word(ramSlot + at) != 1u) return fail(30);
    at += 4;
    if (ramSlot[at] != objects[3][56] || std::memcmp(ramSlot + at + 2, objects[3] + 16, 12) != 0) return fail(31);
    if (std::memcmp(ramSlot + at + 14, tempPlayerStruct + 2 * 23984, 23984) != 0) return fail(32);
    at += 23998 * 4;
    if (std::strcmp(reinterpret_cast<char*>(ramSlot + at), "only") != 0 || get_word(ramSlot + at + 32) != 7u) return fail(33);
    if (!untouched_from(at + 36)) return fail(34);

    // No local player: the index runs to nmPlayers.
    setup();
    nmPlayers = 2;
    local_mask = 0;
    set_word(player + 2 * 23984 + 21872, 2);
    updateRamSlot__Fii(0, 1);
    if (log_text != "name;cam;gate;conv2;ents:betaZone;") return fail(35);

    // gameOver: fixed 12-character name searched and copied at +0.
    setup();
    gameOver = 1;
    nmPlayers = 1;
    set_word(player + 21872, 1);
    norrath_host_names[1] = "game";
    updateRamSlot__Fii(0, 0);
    if (log_text.rfind("name;", 0) == 0) return fail(36);
    if (std::strcmp(reinterpret_cast<char*>(ramSlot), "gameComplete") != 0 || ramSlot[13] != 0xee) return fail(37);
    if (std::strcmp(reinterpret_cast<char*>(ramSlot + 20), "TITLE-TWO") != 0) return fail(38);
    if (log_text.find("ents:gameComplete;") == std::string::npos) return fail(39);

    std::puts("update ram slot passed");
    return 0;
}
