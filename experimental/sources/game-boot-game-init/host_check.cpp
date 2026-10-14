// Host contract for gameInit. Not an EE plan input.
#include "game_init.cpp"

#include <cstdio>
#include <cstring>

extern "C" {
signed char controlStyle = 0;
unsigned dropButton = 0;
unsigned infoButton = 0;
unsigned dropButtonFontChar = 0;
unsigned infoButtonFontChar = 0;
unsigned padAcceptButtonFontChar = 0;
unsigned padBackButtonFontChar = 0;
unsigned padAcceptButton = 0;
unsigned padBackButton = 0;
unsigned equipButton = 0;
unsigned equipButtonFontChar = 0;
unsigned char MC_GameConfig[252];
unsigned attackButton[4];
unsigned spellButton1[4];
unsigned spellButton2[4];
unsigned actionButton[4];
unsigned blockButton[4];
unsigned healthPotionButton[4];
unsigned energyPotionButton[4];
unsigned automapButton[4];
unsigned weaponToggleButton[4];
unsigned soundSfxVolume = 0;
unsigned soundMusicVolume = 0;
unsigned engineBrightnessCorrection = 0;
unsigned soundStereoEnable = 0;
unsigned padVibrationEnable = 0;
unsigned DLG_ShowDialogText = 0;
unsigned subtitlesOn = 0;
unsigned damageDisplayOn = 0;
unsigned gameMultiPlayerMarkersOn = 0;
unsigned gameCombatCamera = 0;
unsigned gameReverseCameraYaw = 0;
unsigned chatOn = 0;
unsigned commRecord = 0;
unsigned commThresh = 0;
unsigned commPlayback = 0;
unsigned commEnable = 0;
unsigned commEcho = 0;
unsigned g_BloodLevel = 0;
void* haloTexture2 = nullptr;
}

static int failures = 0;
static char log_text[16];
static int log_used = 0;
static void (*seen_task[2])();
static int seen_priority[2];
static char seen_task_name[2][16];
static int seen_flag[2];
static int tasks = 0;
static unsigned sfx_at_update = 0;
static unsigned music_at_update = 0;
static unsigned blood_at_update = 0;
static unsigned blood_at_load = 0;
static char loaded[16];
static void (*handler)() = nullptr;
static void* halo = nullptr;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static void mark(char step) {
    if (log_used < 15) log_text[log_used++] = step;
    log_text[log_used] = 0;
}
static void write_u32(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}

extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int flag) {
    if (tasks < 2) {
        seen_task[tasks] = callback;
        seen_priority[tasks] = priority;
        std::snprintf(seen_task_name[tasks], sizeof seen_task_name[tasks], "%s", name);
        seen_flag[tasks] = flag;
    }
    ++tasks;
}
extern "C" void gameCameraTask__Fv() {}
extern "C" void gameDrawWorld__Fv() {}
extern "C" void soundUpdateMusicVolume__Fv() {
    mark('U');
    sfx_at_update = soundSfxVolume;
    music_at_update = soundMusicVolume;
    blood_at_update = g_BloodLevel;
}
extern "C" void lumpLoad__FPc(char* name) {
    mark('L');
    blood_at_load = g_BloodLevel;
    std::snprintf(loaded, sizeof loaded, "%s", name);
}
extern "C" void helpMessageInit__Fv() { mark('H'); }
extern "C" void gameResetGameState__Fv() { mark('R'); }
extern "C" void animSetSpecialSoundHandler__FPFiG6Point3_v(void (*value)()) {
    mark('A');
    handler = value;
}
extern "C" void specialSoundHandler__FiG6Point3() {}
extern "C" void worldSetLightHaloTexture__FP8_texture(void* texture) {
    mark('W');
    halo = texture;
}

static void fill_config() {
    std::memset(MC_GameConfig, 0x5a, sizeof MC_GameConfig);
    write_u32(MC_GameConfig + 0, 0x11111111u);
    write_u32(MC_GameConfig + 4, 0x3f800000u);
    write_u32(MC_GameConfig + 8, 0x3f000000u);
    write_u32(MC_GameConfig + 12, 0x3e800000u);
    write_u32(MC_GameConfig + 16, 2u);
    write_u32(MC_GameConfig + 20, 3u);
    write_u32(MC_GameConfig + 24, 4u);
    write_u32(MC_GameConfig + 28, 5u);
    write_u32(MC_GameConfig + 32, 6u);
    write_u32(MC_GameConfig + 36, 7u);
    write_u32(MC_GameConfig + 40, 8u);
    write_u32(MC_GameConfig + 44, 9u);
    write_u32(MC_GameConfig + 48, 10u);
    write_u32(MC_GameConfig + 52, 0x01000000u);
    write_u32(MC_GameConfig + 84, 0x01000008u);
    write_u32(MC_GameConfig + 88, 0x02000000u);
    write_u32(MC_GameConfig + 92, 0x02000004u);
    write_u32(MC_GameConfig + 196, 11u);
    write_u32(MC_GameConfig + 200, 12u);
    write_u32(MC_GameConfig + 204, 13u);
    write_u32(MC_GameConfig + 208, 14u);
    write_u32(MC_GameConfig + 212, 15u);
    write_u32(MC_GameConfig + 216, 16u);
}

static void sentinel_buttons() {
    dropButton = 9u;
    infoButton = 0x55555555u;
    dropButtonFontChar = 9u;
    infoButtonFontChar = 0x66666666u;
    padAcceptButtonFontChar = 9u;
    padBackButtonFontChar = 9u;
    padAcceptButton = 9u;
    padBackButton = 9u;
    equipButton = 9u;
    equipButtonFontChar = 9u;
}

int main() {
    haloTexture2 = reinterpret_cast<void*>(static_cast<unsigned long>(0x44u));
    g_BloodLevel = 99u;
    fill_config();
    sentinel_buttons();
    controlStyle = 1;
    gameInit__Fv();
    expect(tasks == 2, "two tasks");
    expect(seen_task[0] == gameCameraTask__Fv && seen_priority[0] == 11, "camera task");
    expect(std::strcmp(seen_task_name[0], "camera") == 0 && seen_flag[0] == 0, "camera task arguments");
    expect(seen_task[1] == gameDrawWorld__Fv && seen_priority[1] == 30, "world task");
    expect(std::strcmp(seen_task_name[1], "drawWorld") == 0 && seen_flag[1] == 1, "world task arguments");
    expect(dropButton == 16u && infoButton == 128u, "style one buttons");
    expect(dropButtonFontChar == 124u && infoButtonFontChar == 125u, "style one font chars");
    expect(padAcceptButtonFontChar == 126u && padBackButtonFontChar == 123u, "style one accept/back chars");
    expect(padAcceptButton == 64u && padBackButton == 32u, "style one accept/back");
    expect(equipButton == 64u && equipButtonFontChar == 126u, "style one equip");
    expect(attackButton[0] == 0x01000000u && weaponToggleButton[0] == 0x01000008u, "first record ends");
    expect(attackButton[1] == 0x02000000u && spellButton1[1] == 0x02000004u, "second record");
    expect(sfx_at_update == 0x3e800000u && music_at_update == 0x3f000000u, "volumes before music update");
    expect(blood_at_update == 99u && blood_at_load == 7u, "blood stored between update and load");
    expect(engineBrightnessCorrection == 0x3f800000u && soundStereoEnable == 2u, "brightness and stereo");
    expect(padVibrationEnable == 3u && DLG_ShowDialogText == 4u, "vibration and dialog text");
    expect(subtitlesOn == 5u && damageDisplayOn == 6u, "subtitles and damage");
    expect(gameMultiPlayerMarkersOn == 8u && gameCombatCamera == 9u, "markers and combat camera");
    expect(gameReverseCameraYaw == 10u && chatOn == 11u, "yaw and chat");
    expect(commRecord == 12u && commThresh == 13u && commPlayback == 14u, "comm floats");
    expect(commEnable == 15u && commEcho == 16u && g_BloodLevel == 7u, "comm flags and blood");
    expect(std::strcmp(loaded, "globsnd.lmp") == 0, "global sound lump");
    expect(std::strcmp(log_text, "ULHRAW") == 0, "call order");
    expect(handler == specialSoundHandler__FiG6Point3 && halo == haloTexture2, "handler and halo");

    sentinel_buttons();
    controlStyle = 0;
    gameInit__Fv();
    expect(dropButton == 128u && dropButtonFontChar == 124u, "style zero drop");
    expect(infoButton == 0x55555555u && infoButtonFontChar == 0x66666666u, "style zero leaves info");
    expect(padAcceptButtonFontChar == 123u && padBackButtonFontChar == 126u, "style zero chars");
    expect(padAcceptButton == 32u && padBackButton == 320u, "style zero buttons");
    expect(equipButton == 32u && equipButtonFontChar == 123u, "style zero equip");

    sentinel_buttons();
    controlStyle = 2;
    gameInit__Fv();
    expect(dropButton == 9u && infoButton == 0x55555555u && padBackButton == 9u, "other style leaves buttons");
    controlStyle = static_cast<signed char>(0xff);
    sentinel_buttons();
    gameInit__Fv();
    expect(dropButton == 9u && equipButton == 9u, "negative style leaves buttons");
    if (failures != 0) return 1;
    std::puts("game init contract passed");
    return 0;
}
