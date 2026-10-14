// Host contract for gameMaybeSaveConfig. Not an EE plan input.
#include "maybe_save.cpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" unsigned char MC_GameConfig[252] = {};
extern "C" unsigned soundSfxVolume = 0;
extern "C" unsigned soundMusicVolume = 0;
extern "C" unsigned soundStereoEnable = 0;
extern "C" unsigned engineBrightnessCorrection = 0;
extern "C" unsigned g_BloodLevel = 0;
extern "C" unsigned padVibrationEnable = 0;
extern "C" unsigned DLG_ShowDialogText = 0;
extern "C" unsigned subtitlesOn = 0;
extern "C" unsigned damageDisplayOn = 0;
extern "C" unsigned gameMultiPlayerMarkersOn = 0;
extern "C" unsigned gameCombatCamera = 0;
extern "C" unsigned gameReverseCameraYaw = 0;
extern "C" unsigned commRecord = 0;
extern "C" unsigned chatOn = 0;
extern "C" unsigned commThresh = 0;
extern "C" unsigned commPlayback = 0;
extern "C" unsigned commEcho = 0;
extern "C" unsigned commEnable = 0;
extern "C" unsigned char netNetworkMode = 0;
extern "C" unsigned attackButton[4] = {};
extern "C" unsigned spellButton1[4] = {};
extern "C" unsigned spellButton2[4] = {};
extern "C" unsigned actionButton[4] = {};
extern "C" unsigned blockButton[4] = {};
extern "C" unsigned healthPotionButton[4] = {};
extern "C" unsigned energyPotionButton[4] = {};
extern "C" unsigned automapButton[4] = {};
extern "C" unsigned weaponToggleButton[4] = {};

static int failures = 0;
static int saves = 0;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" void MC_SaveConfig__Fv() { ++saves; }

static void match_all() {
    std::memset(MC_GameConfig, 0, sizeof MC_GameConfig);
    soundSfxVolume = soundMusicVolume = soundStereoEnable = 0;
    engineBrightnessCorrection = g_BloodLevel = padVibrationEnable = 0;
    DLG_ShowDialogText = subtitlesOn = damageDisplayOn = 0;
    gameMultiPlayerMarkersOn = gameCombatCamera = gameReverseCameraYaw = 0;
    commRecord = chatOn = commThresh = commPlayback = commEcho = commEnable = 0;
    netNetworkMode = 0;
    std::memset(attackButton, 0, sizeof attackButton);
    std::memset(spellButton1, 0, sizeof spellButton1);
    std::memset(spellButton2, 0, sizeof spellButton2);
    std::memset(actionButton, 0, sizeof actionButton);
    std::memset(blockButton, 0, sizeof blockButton);
    std::memset(healthPotionButton, 0, sizeof healthPotionButton);
    std::memset(energyPotionButton, 0, sizeof energyPotionButton);
    std::memset(automapButton, 0, sizeof automapButton);
    std::memset(weaponToggleButton, 0, sizeof weaponToggleButton);
    saves = 0;
}

int main() {
    match_all();
    expect(gameMaybeSaveConfig__Fii(0, 0) == 0 && saves == 0, "matching state with both arguments zero does not save");

    match_all();
    expect(gameMaybeSaveConfig__Fii(1, 0) == 0 && saves == 1, "a nonzero first argument saves a match");
    expect(attackButton[0] == 0, "a match does not rewrite buttons");

    match_all();
    expect(gameMaybeSaveConfig__Fii(1, 1) == 0 && saves == 0, "a nonzero second argument skips the save");

    match_all();
    soundSfxVolume = 0x3f800000u;
    expect(gameMaybeSaveConfig__Fii(0, 0) == 1 && saves == 1, "a different sfx volume counts and saves");
    expect(read_u32(MC_GameConfig + 12) == 0x3f800000u, "the live sfx word is stored at offset 12");
    expect(read_u32(MC_GameConfig + 8) == 0, "music stays at offset 8");

    match_all();
    soundSfxVolume = 0x3f800000u;
    unsigned char before[252];
    std::memcpy(before, MC_GameConfig, sizeof before);
    expect(gameMaybeSaveConfig__Fii(0, 1) == 1 && saves == 0, "a difference with a nonzero second argument does not save");
    expect(std::memcmp(before, MC_GameConfig, sizeof before) == 0, "that path does not write the config");

    match_all();
    soundMusicVolume = 0x40000000u;
    engineBrightnessCorrection = 0x40400000u;
    expect(gameMaybeSaveConfig__Fii(0, 0) == 2, "music and brightness are separate counts");
    expect(read_u32(MC_GameConfig + 8) == 0x40000000u, "music is stored at offset 8");
    expect(read_u32(MC_GameConfig + 4) == 0x40400000u, "brightness is stored at offset 4");

    match_all();
    write_u32(MC_GameConfig + 12, 0x80000000u);
    soundSfxVolume = 0;
    expect(gameMaybeSaveConfig__Fii(0, 1) == 0, "positive zero matches negative zero");

    match_all();
    soundSfxVolume = 0x7fc00000u;
    expect(gameMaybeSaveConfig__Fii(0, 1) == 1 && saves == 0, "a NaN sfx word counts as different");

    match_all();
    write_u32(MC_GameConfig + 52, 2u);
    attackButton[0] = 1u;
    expect(gameMaybeSaveConfig__Fii(0, 0) == 1 && saves == 1, "one button word counts");
    expect(attackButton[0] == 2u, "the config word replaces the live button");
    expect(read_u32(MC_GameConfig + 52) == 2u, "the config button word stays");

    match_all();
    write_u32(MC_GameConfig + 52 + 3 * 36 + 32, 9u);
    weaponToggleButton[3] = 8u;
    soundStereoEnable = 3u;
    expect(gameMaybeSaveConfig__Fii(0, 0) == 2, "a late button and stereo both count");
    expect(weaponToggleButton[3] == 9u, "the last weapon word is copied from the config");
    expect(read_u32(MC_GameConfig + 16) == 3u, "stereo is stored at offset 16");

    match_all();
    attackButton[1] = 4u;
    netNetworkMode = 1;
    expect(gameMaybeSaveConfig__Fii(0, 0) == 0 && saves == 0, "network mode ignores button words");
    expect(attackButton[1] == 4u, "network mode leaves the live button");

    match_all();
    netNetworkMode = 0x80;
    commEnable = 6u;
    expect(gameMaybeSaveConfig__Fii(0, 0) == 1, "network mode still compares commEnable");
    expect(read_u32(MC_GameConfig + 212) == 6u, "commEnable is stored at offset 212");

    match_all();
    chatOn = 1u;
    damageDisplayOn = 2u;
    subtitlesOn = 3u;
    g_BloodLevel = 4u;
    expect(gameMaybeSaveConfig__Fii(0, 1) == 4 && saves == 0, "four scalar words count without saving");

    if (failures != 0) return 1;
    std::puts("gameMaybeSaveConfig contract passed");
    return 0;
}
