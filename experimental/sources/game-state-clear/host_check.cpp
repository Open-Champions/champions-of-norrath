// Host contract for gameClear. Not an EE plan input.
#include "clear.cpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" int shopActiveFrame = 1;
extern "C" int gameRecallInhibit = 1;
extern "C" int bossMeterCurrent = 1;
extern "C" int bossMeterMax = 1;
extern "C" int bossMeterAlpha = 1;
extern "C" int gameCameraShake = 1;
extern "C" int gameShowEnemiesInMap = 1;
extern "C" unsigned coreClearColor = 1;
extern "C" unsigned short gameDeathTimer = 1;
extern "C" unsigned char gameDeathTimerSingle[8] = {1, 1, 1, 1, 1, 1, 1, 1};
extern "C" unsigned char gameHidePlayers = 1;
extern "C" unsigned char recallOn = 1;
extern "C" unsigned char helpMessageOn = 1;
extern "C" int lastNmLocalPlayersAlive = 1;
extern "C" int localNmPlayersAlive = 1;
extern "C" unsigned multiHelpMessageOn = 1;
extern "C" unsigned char* world = 0;
extern "C" unsigned char particlesPaused = 1;
extern "C" int gamePause = 1;

static int failures = 0;
static char log_text[256];
static int log_used = 0;
static int shop_at_cut = 1;
static unsigned color_at_far = 0;
static unsigned short death_at_far = 0;
static unsigned char single_at_far[8];
static unsigned char hidden_at_far = 1;
static std::uint32_t far_bits = 0;
static unsigned char* world_at_spell = 0;
static unsigned char* world_at_583 = reinterpret_cast<unsigned char*>(1);
static int pause_at_spell = 1;
static unsigned char particles_at_spell = 1;
static int water_argument = -1;
static int lump_argument = -1;
static unsigned char world_object[4];

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static void mark(const char* token) {
    const int length = static_cast<int>(std::strlen(token));
    if (log_used + length + 1 < static_cast<int>(sizeof log_text)) {
        if (log_used != 0) log_text[log_used++] = ' ';
        std::memcpy(log_text + log_used, token, static_cast<std::size_t>(length));
        log_used += length;
        log_text[log_used] = 0;
    }
}

static void note_file(const char* file, int line, char kind) {
    if (std::strcmp(file, "game.cc") != 0) {
        mark("badfile");
        return;
    }
    char token[16];
    std::snprintf(token, sizeof token, "%c%d", kind, line);
    mark(token);
}

extern "C" void scePrintf(const char* text) {
    mark(std::strcmp(text, "gameclear\n") == 0 ? "printf" : "badprintf");
}
extern "C" void _listCheckAllIntegrity__FPCci(const char* file, int line) {
    if (line == 583) world_at_583 = world;
    note_file(file, line, 'i');
}
extern "C" void memCheckHeapReal__FPCci(const char* file, int line) { note_file(file, line, 'h'); }
extern "C" void _engineCheckStack__FPCci(const char* file, int line) { note_file(file, line, 's'); }
extern "C" void hudFinishGold__Fv() { mark("gold"); }
extern "C" void hudClearInfoMessage__Fv() { mark("info"); }
extern "C" void cutClear__Fv() {
    shop_at_cut = shopActiveFrame;
    mark("cut");
}
extern "C" void worldSetFarClip__Ff(float distance) {
    std::memcpy(&far_bits, &distance, 4);
    color_at_far = coreClearColor;
    death_at_far = gameDeathTimer;
    std::memcpy(single_at_far, gameDeathTimerSingle, 8);
    hidden_at_far = gameHidePlayers;
    mark("far");
}
extern "C" void floorClear__Fv() { mark("floor"); }
extern "C" void cdClear__Fv() { mark("cd"); }
extern "C" void P_Clear__Fv() { mark("pclear"); }
extern "C" void objectClear__Fv() { mark("object"); }
extern "C" void drawListClear__Fv() { mark("draw"); }
extern "C" void worldReset__FP12_worldHeader(unsigned char* header) {
    mark(header == world_object ? "reset" : "badreset");
}
extern "C" void worldClear__Fv() { mark("world"); }
extern "C" void lightClear__Fv() { mark("light"); }
extern "C" void textureClear__Fv() { mark("tex"); }
extern "C" void fxClear__Fv() { mark("fx"); }
extern "C" void gameLineClear__Fv() { mark("line"); }
extern "C" void worldAllocateWaterPatches__Fi(int value) {
    water_argument = value;
    mark("water");
}
extern "C" void lumpClear__Fi(int value) {
    lump_argument = value;
    mark("lump");
}
extern "C" void inventoryReset__Fv() { mark("inv"); }
extern "C" void scriptClear__Fv() { mark("script"); }
extern "C" void DLG_Clear__Fv() { mark("dlg"); }
extern "C" void jumpGateClear__Fv() { mark("jump"); }
extern "C" void padClear__Fv() { mark("pad"); }
extern "C" void playerClearSpellEffect__Fv() {
    world_at_spell = world;
    pause_at_spell = gamePause;
    particles_at_spell = particlesPaused;
    mark("spell");
}
extern "C" void blockAllocInit__Fv() { mark("block"); }

static void fill_dirty() {
    shopActiveFrame = 1;
    gameRecallInhibit = 1;
    bossMeterCurrent = 1;
    bossMeterMax = 1;
    bossMeterAlpha = 1;
    gameCameraShake = 1;
    gameShowEnemiesInMap = 1;
    coreClearColor = 1;
    gameDeathTimer = 1;
    std::memset(gameDeathTimerSingle, 1, 8);
    gameHidePlayers = 1;
    recallOn = 1;
    helpMessageOn = 1;
    lastNmLocalPlayersAlive = 1;
    localNmPlayersAlive = 1;
    multiHelpMessageOn = 0x01010101u;
    particlesPaused = 1;
    gamePause = 1;
    log_used = 0;
    log_text[0] = 0;
    water_argument = -1;
    lump_argument = -1;
    world_at_583 = reinterpret_cast<unsigned char*>(1);
}

int main() {
    const char* with_level =
        "printf i436 h437 s438 h441 s442 gold info cut far floor cd pclear "
        "i478 object i480 draw pclear reset world light tex fx line water lump "
        "inv script dlg jump pad spell i583 s584 block";
    const char* without_level =
        "printf i436 h437 s438 h441 s442 gold info cut far floor cd pclear "
        "i478 object i480 draw pclear reset world light tex fx line "
        "inv script dlg jump pad spell i583 s584 block";

    fill_dirty();
    world = world_object;
    gameClear__Fi(1);
    expect(std::strcmp(log_text, with_level) == 0, "nonzero argument releases the level");
    expect(shop_at_cut == -10000, "shop frame is -10000 before cutClear");
    expect(far_bits == 0x44e10000u, "far clip bits are 1800.0f");
    expect(color_at_far == 0x80000000u, "clear color is set before the far clip");
    expect(death_at_far == 240, "death timer is 240 before the far clip");
    expect(std::memcmp(single_at_far, "\xf0\x00\xf0\x00\xf0\x00\xf0\x00", 8) == 0, "all four death halfwords are 240");
    expect(hidden_at_far == 0, "players are unhidden before the far clip");
    expect(recallOn == 0 && helpMessageOn == 0, "recall and help are cleared");
    expect(lastNmLocalPlayersAlive == 0 && localNmPlayersAlive == 0, "alive counts are cleared");
    expect(multiHelpMessageOn == 0, "the help-message word is cleared");
    expect(gameRecallInhibit == 0 && bossMeterCurrent == 0 && bossMeterMax == 0 && bossMeterAlpha == 0, "meters are cleared");
    expect(gameCameraShake == 0 && gameShowEnemiesInMap == 0, "shake and map-enemy flags are cleared");
    expect(water_argument == 0 && lump_argument == 0, "water and lump clear receive 0");
    expect(world_at_spell == world_object, "world is still set during spell clear");
    expect(pause_at_spell == 0 && particles_at_spell == 0, "pause flags are clear before spell clear");
    expect(world_at_583 == 0 && world == 0, "world is cleared before the line 583 check");

    fill_dirty();
    world = world_object;
    gameClear__Fi(0);
    expect(std::strcmp(log_text, without_level) == 0, "a zero argument skips water and lump clear");

    fill_dirty();
    world = 0;
    gameClear__Fi(-1);
    expect(std::strstr(log_text, "pclear world") != 0, "a null world skips worldReset");
    expect(std::strstr(log_text, "water") != 0, "a negative argument still releases the level");

    if (failures != 0) return 1;
    std::puts("gameClear contract passed");
    return 0;
}
