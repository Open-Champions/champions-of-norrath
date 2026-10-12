// Host check for startNewWorld__Fv. Not an EE plan input.
#define NORRATH_HOST_CONTRACT
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#define sprintf norrath_host_sprintf
#define strcpy norrath_host_strcpy
#define strlen norrath_host_strlen
#include "start_new_world.cpp"
#undef sprintf
#undef strcpy
#undef strlen

extern "C" {
int frontEndRunning = 0;
int nmPlayers = 2;
int gamePlayersPresent = 2;
signed char playerRecalled = 0;
signed char recallEnter = 5;
signed char gotPlayerStartThisLevel = 0;
int commEnable = 0;
int coreLevelCommentTimer = 0;
int coreHeadsetResetTimer = 0;
void* world = (void*)0x77;
char levelName[400];
char engineCurrentLevelName[80];
Point3 recallPos[4];
Point3 playerStartPos[4];
unsigned short playerStartYaw[4];
unsigned char player[23984 * 4];
}
static std::string trace;
static unsigned stored_zero_at = 0xffffffffu;
static int death_task = 0;
static int save_entities = 0;
static int act_result = 7;
static int async_mode = 0;
static int connected[4] = {1, 1, 1, 1};
static int already_saved = 0;
static int block_ok = 1;

extern "C" void norrath_host_store_zero_word(unsigned address) { stored_zero_at = address; trace += "Z;"; }
extern "C" int norrath_host_sprintf(char* d, const char* f, ...) {
    va_list ap; va_start(ap, f);
    const int n = std::vsnprintf(d, 160, f, ap);
    va_end(ap);
    trace += "sprintf;";
    return n;
}
extern "C" char* norrath_host_strcpy(char* d, const char* s) { return std::strcpy(d, s); }
extern "C" unsigned norrath_host_strlen(const char* s) { return (unsigned)std::strlen(s); }
extern "C" void _engineCheckStack__FPCci(const char* file, int line) {
    trace += "K" + std::to_string(line) + (std::strcmp(file, "core.cc") == 0 ? "" : "?") + ";";
}
extern "C" int engineTaskActive__FPc(const char* name) {
    trace += "T;"; if (std::strcmp(name, "deathTask") != 0) trace += "?";
    return death_task;
}
extern "C" void gameGetFullLevelName__FPc(char* out) { trace += "G;"; std::strcpy(out, "full-name"); }
extern "C" int MC_SaveLevelEntities__FPc(char* n) { trace += std::string("E[") + n + "];"; return save_entities; }
extern "C" int MC_SetActByLevelName__FPc(char* n) { trace += std::string("A[") + n + "];"; return act_result; }
extern "C" void MC_CheckPlayerImportOrChange__Fv() { trace += "I;"; }
extern "C" void playerSelectionEnforce__Fv() { trace += "S;"; }
extern "C" void soundDisable__Ff(float v) { trace += "D" + std::to_string(v) + ";"; }
extern "C" void gameLoadWorld__FPc(char* n) { trace += std::string("L[") + n + "];"; }
extern "C" float worldFindHeight__FP12_worldHeaderG6Point3f(void* w, const Point3* p, float limit) {
    trace += "H[" + std::to_string((int)p->x) + "," + std::to_string((int)p->y) + "," + std::to_string((int)limit) + (w == (void*)0x77 ? "" : "?") + "];";
    return p->x * 100.0f + p->y;
}
extern "C" int scePrintf(const char* f, ...) {
    va_list ap; va_start(ap, f);
    char text[128];
    if (std::strncmp(f, "    %f", 6) == 0) {
        (void)va_arg(ap, unsigned long long);
        std::snprintf(text, sizeof text, "f");
    } else if (std::strncmp(f, ">>", 2) == 0) {
        std::snprintf(text, sizeof text, ">>%d", va_arg(ap, int));
    } else {
        std::snprintf(text, sizeof text, "?");
    }
    va_end(ap);
    trace += std::string("P[") + text + "];";
    return 0;
}
extern "C" unsigned long long fptodp(float v) { trace += "F" + std::to_string((int)v) + ";"; return 0; }
extern "C" void objectFindEmptySpot__FG6Point3iiPc(Point3* r, const Point3* p, int a, int b, char* c) {
    trace += "O[" + std::to_string((int)p->x) + "," + std::to_string(a) + "," + std::to_string(b) + (c ? "c" : "n") + "];";
    r->x = p->x + 1.0f; r->y = p->y + 2.0f; r->z = 55.0f;
}
extern "C" int netPlayerIsConnected__Fi(int i) { trace += "N" + std::to_string(i) + ";"; return connected[i]; }
extern "C" void* blockAlloc__Fi(int n) {
    static unsigned char blocks[4][1408]; static int used = 0;
    trace += "B" + std::to_string(n) + ";";
    return block_ok ? blocks[used++ & 3] : nullptr;
}
static int ctor_calls = 0;
extern "C" void* __6PlayerG6Point3sii(void* p, const Point3* pos, short yaw, int idx, int zero) {
    trace += "C[" + std::to_string((int)pos->x) + "," + std::to_string(yaw) + "," + std::to_string(idx) + "," + std::to_string(zero) + "];";
    ++ctor_calls; return p;
}
extern "C" int MC_GetAsyncMode__Fv() { trace += "M;"; return async_mode; }
extern "C" void removeAllQuestsFromLog__Fv() { trace += "Q;"; }
extern "C" int MC_HasThisLevelBeenSaved__Fv() { trace += std::string("V") + (recallEnter == 0 ? "" : "?") + ";"; return already_saved; }

static void setup(const char* name) {
    trace.clear();
    std::memset(levelName, 0, sizeof levelName);
    std::strcpy(levelName, name);
    std::memset(player, 0, sizeof player);
}
static void putf(unsigned char* p, float f) { std::memcpy(p, &f, 4); }

int main() {
    // A plain two-player start of "hub_two" with no entities saved. Player 0 alive, player 1 dead.
    setup("hubone_two");
    putf(player + 172, 5.0f); putf(player + 23984 + 172, 0.0f);
    levelName[80] = 9;
    levelName[81] = 0;
    playerStartPos[0] = {10.0f, 20.0f, 0.0f}; playerStartPos[1] = {30.0f, 40.0f, 0.0f};
    playerStartYaw[0] = 100; playerStartYaw[1] = 65000;
    startNewWorld__Fv();
    const std::string expect =
        "K285;K302;A[hubone_two];I;S;D0.050000;K349;L[hubone];K351;"
        "H[10,20,12];H[30,40,12];P[>>2];"
        // player 0: no relocation, alive and connected, constructed; quests cleared
        "F5;P[f];N0;B1408;C[10,100,0,0];M;T;Q;"
        // player 1: relocated, dead
        "O[30,36,14n];H[31,42,12];F0;P[f];M;T;Q;"
        "V;";
    if (trace != expect) { std::fprintf(stderr, "got: %s\n", trace.c_str()); return 2; }
    if (std::strcmp(levelName + 81, "hubone") != 0 || std::strcmp(levelName + 161, "two") != 0) return 3;
    if (levelName[242] != 0) return 4;
    if (std::strcmp(engineCurrentLevelName, "hubone") != 0) return 5;
    if (levelName[0] != 0 || (unsigned char)levelName[80] != 0xfe || levelName[241] != 9) return 6;
    if (playerStartYaw[1] != (unsigned short)(65000 + 8000)) return 7;
    if (playerStartPos[1].x != 31.0f || playerStartPos[1].y != 42.0f || playerStartPos[1].z != 3142.0f) return 8;
    if (playerStartPos[0].z != 1020.0f) return 9;
    if (coreLevelCommentTimer != 300 || recallEnter != 0) return 10;
    // The "None" name and quest clearing happen for both slots.
    if (std::memcmp(player + 36, "None", 5) != 0 || std::memcmp(player + 23984 + 36, "None", 5) != 0) return 11;

    // Entities saved with no death task: stores a zero word at the task result, clears the name, stops.
    setup("abc_def"); levelName[81] = 'x'; save_entities = 1; death_task = 0;
    startNewWorld__Fv();
    if (trace != "K285;T;G;E[full-name];Z;" || stored_zero_at != 0 || levelName[0] != 0) return 12;
    // Entities not saved: continues to the second check point.
    setup("abc_def"); levelName[81] = 'x'; save_entities = 0;
    startNewWorld__Fv();
    if (trace.compare(0, 36, "K285;T;G;E[full-name];K302;A[abc_def") != 0) return 13;
    // A death task skips the whole save step.
    setup("abc_def"); levelName[81] = 'x'; death_task = 1; save_entities = 1;
    startNewWorld__Fv();
    if (trace.compare(0, 11, "K285;T;K302") != 0) return 14;
    death_task = 0; save_entities = 0;

    // Front end running: no act lookup. A second part "gate" makes the "%s_%s" form.
    frontEndRunning = 1;
    setup("zone_gate"); levelName[81] = 'q'; levelName[161] = 'r';
    nmPlayers = 0;
    startNewWorld__Fv();
    if (trace != "K285;T;G;E[full-name];K302;I;S;sprintf;D0.050000;K349;L[zone];K351;P[>>2];V;") return 15;
    if (std::strcmp(levelName + 81, "zone") != 0 || std::strcmp(levelName + 161, "gate") != 0) return 16;
    // No underscore: the part after it is cleared.
    setup("plain"); levelName[161] = 'r';
    startNewWorld__Fv();
    if (std::strcmp(levelName + 81, "plain") != 0 || levelName[161] != 0) return 17;
    frontEndRunning = 0;

    // Recalled positions copy x and y only, then clear the recalled flag; dead players are skipped.
    setup("abc");
    nmPlayers = 2; gamePlayersPresent = 0; playerRecalled = 1;
    recallPos[0] = {1, 2, 3}; recallPos[1] = {4, 5, 6};
    playerStartPos[0] = {0, 0, 9}; playerStartPos[1] = {0, 0, 9};
    startNewWorld__Fv();
    if (playerRecalled != 0 || playerStartPos[1].x != 4.0f || playerStartPos[1].y != 5.0f) return 18;
    if (playerStartPos[1].z != 405.0f || playerStartPos[0].z != 102.0f) return 19;
    playerRecalled = 1; nmPlayers = 0;
    startNewWorld__Fv();
    if (playerRecalled != 1) return 20;

    // The third start mode skips relocation, an async mode of 3 skips quest clearing, and
    // the last step honours commEnable and an already saved level.
    setup("abc");
    nmPlayers = 2; gamePlayersPresent = 1; gotPlayerStartThisLevel = 3; async_mode = 3; already_saved = 1;
    commEnable = 1; coreLevelCommentTimer = 17; connected[1] = 0;
    putf(player + 172, 1.0f); putf(player + 23984 + 172, 2.0f);
    startNewWorld__Fv();
    if (trace.find("O[") != std::string::npos || trace.find("Q;") != std::string::npos) return 21;
    if (trace.find("C[") == std::string::npos || trace.find("N1;") == std::string::npos) return 22;
    if (trace.find("C[", trace.find("C[") + 1) != std::string::npos) return 23;
    if (coreLevelCommentTimer != 17 || commEnable != 0 || coreHeadsetResetTimer != 20) return 24;
    block_ok = 0; connected[1] = 1; trace.clear();
    startNewWorld__Fv();
    if (trace.find("C[") != std::string::npos) return 25;
    return 0;
}
