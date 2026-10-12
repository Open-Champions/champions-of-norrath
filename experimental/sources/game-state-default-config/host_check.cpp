// Host contract for gameSetDefaultGameConfig. Not an EE plan input.
#include "default_config.cpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" float soundSfxVolume = 0.0f;
extern "C" float soundMusicVolume = 0.0f;
extern "C" unsigned soundStereoEnable = 0;
extern "C" float engineBrightnessCorrection = 0.0f;
extern "C" unsigned g_BloodLevel = 9;
extern "C" unsigned padVibrationEnable = 0;
extern "C" unsigned DLG_ShowDialogText = 0;
extern "C" unsigned subtitlesOn = 0;
extern "C" unsigned damageDisplayOn = 0;
extern "C" unsigned gameMultiPlayerMarkersOn = 0;
extern "C" unsigned gameCombatCamera = 0;
extern "C" unsigned gameReverseCameraYaw = 7;
extern "C" float commRecord = 0.0f;
extern "C" unsigned chatOn = 0;
extern "C" float commThresh = 0.0f;
extern "C" float commPlayback = 0.0f;
extern "C" unsigned commEcho = 3;
extern "C" unsigned commEnable = 0;
extern "C" unsigned char controlStyle = 0;
extern "C" unsigned char MC_GameConfig[252] = {};

static int saves = 0;
static unsigned music_at_update = 0;
static unsigned record_at_update = 1;

static unsigned bits_of(float value) {
    unsigned bits = 0;
    std::memcpy(&bits, &value, 4);
    return bits;
}

static unsigned read_u32(const unsigned char* src) {
    unsigned value = 0;
    std::memcpy(&value, src, 4);
    return value;
}

extern "C" void soundUpdateMusicVolume__Fv() {
    ++saves;
    music_at_update = bits_of(soundMusicVolume);
    record_at_update = bits_of(commRecord);
}

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "contract failed: %s\n", message);
        std::exit(1);
    }
}

static void poison() {
    std::memset(MC_GameConfig, 0xa5, sizeof MC_GameConfig);
    soundSfxVolume = soundMusicVolume = engineBrightnessCorrection = 4.0f;
    commRecord = commThresh = commPlayback = 4.0f;
    soundStereoEnable = padVibrationEnable = DLG_ShowDialogText = 0;
    subtitlesOn = damageDisplayOn = gameMultiPlayerMarkersOn = gameCombatCamera = 0;
    gameReverseCameraYaw = 7;
    chatOn = commEnable = 0;
    commEcho = 3;
    g_BloodLevel = 9;
    saves = 0;
    music_at_update = 0;
    record_at_update = 1;
}

static bool record_is(unsigned index, const unsigned expect_words[9]) {
    const unsigned char* record = MC_GameConfig + 52u + index * 36u;
    for (unsigned word = 0; word < 9u; ++word) {
        if (read_u32(record + word * 4u) != expect_words[word]) return false;
    }
    return true;
}

static bool record_untouched(unsigned index) {
    const unsigned char* record = MC_GameConfig + 52u + index * 36u;
    for (unsigned byte = 0; byte < 36u; ++byte) if (record[byte] != 0xa5) return false;
    return true;
}

int main() {
    const unsigned style1[9] = {64u, 32u, 16u, 128u, 8u, 2u, 1u, 1024u, 4u};
    const unsigned style0[9] = {32u, 16u, 64u, 128u, 8u, 2u, 1u, 1024u, 4u};
    poison();
    controlStyle = 1;
    gameSetDefaultGameConfig__Fi(0);
    expect(saves == 0, "a single player does not update music");
    expect(bits_of(soundSfxVolume) == 0x40800000u, "scalars stay put for one player");
    expect(record_is(0, style1), "player 0 style 1 writes the first record");
    expect(record_untouched(1) && record_untouched(2) && record_untouched(3), "other records stay");

    poison();
    controlStyle = 0;
    gameSetDefaultGameConfig__Fi(3);
    expect(record_is(3, style0) && record_untouched(0), "player 3 style 0 writes only the last record");

    poison();
    controlStyle = 2;
    gameSetDefaultGameConfig__Fi(-1);
    expect(saves == 1 && music_at_update == 0x3f000000u, "music is 0.5 before the update");
    expect(record_at_update == 0x40800000u, "commRecord is loaded after the update");
    expect(bits_of(commRecord) == 0x3f4cccccu, "commRecord receives the literal bits");
    expect(bits_of(commThresh) == 0x3e800000u && bits_of(commPlayback) == 0x3f000000u, "thresh and playback defaults");
    expect(read_u32(MC_GameConfig + 200) == 0x3f4cccccu, "config commRecord matches");
    expect(read_u32(MC_GameConfig + 12) == 0x3f000000u && read_u32(MC_GameConfig + 8) == 0x3f000000u, "sfx and music config");
    expect(read_u32(MC_GameConfig + 4) == 0x3f000000u && bits_of(engineBrightnessCorrection) == 0x3f000000u, "brightness is 0.5");
    expect(commEnable == 1 && subtitlesOn == 1 && damageDisplayOn == 1 && soundStereoEnable == 1, "enable words are 1");
    expect(gameMultiPlayerMarkersOn == 1 && gameCombatCamera == 1 && gameReverseCameraYaw == 0, "camera defaults");
    expect(g_BloodLevel == 0 && commEcho == 0 && chatOn == 1 && DLG_ShowDialogText == 1 && padVibrationEnable == 1, "remaining scalars");
    expect(read_u32(MC_GameConfig) == 0 && read_u32(MC_GameConfig + 216) == 0, "the first word and commEcho config are zero");
    expect(record_untouched(0) && record_untouched(3), "style 2 writes no buttons");

    poison();
    controlStyle = 0xff;
    gameSetDefaultGameConfig__Fi(-1);
    expect(record_untouched(0), "a negative style byte writes no buttons");

    poison();
    controlStyle = 1;
    gameSetDefaultGameConfig__Fi(5);
    expect(saves == 0 && record_untouched(0) && bits_of(soundSfxVolume) == 0x40800000u, "an unmatched argument writes nothing");

    poison();
    controlStyle = 0;
    gameSetDefaultGameConfig__Fi(-1);
    expect(record_is(0, style0) && record_is(1, style0) && record_is(2, style0) && record_is(3, style0), "argument -1 writes every style 0 record");
    std::puts("gameSetDefaultGameConfig contract passed");
    return 0;
}
