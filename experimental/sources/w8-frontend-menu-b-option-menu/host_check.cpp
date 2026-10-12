#include "option_menu.cpp"

#include <cstdio>
#include <cstring>

extern "C" {
float engineBrightnessCorrection, soundMusicVolume, soundSfxVolume;
int gameCombatCamera, gameReverseCameraYaw, padVibrationEnable, soundStereoEnable, g_BloodLevel, subtitlesOn,
    damageDisplayOn, chatOn, gameMultiPlayerMarkersOn;
}

static int failures;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++failures; } } while (0)

int main() {
    struct Expect { const char* label; unsigned kind; void* value; };
    const Expect expect[13] = {
        {"menu_bright", 4, &engineBrightnessCorrection}, {"menu_music", 4, &soundMusicVolume},
        {"menu_sfx", 4, &soundSfxVolume}, {"menu_controls", 0, nullptr},
        {"menu_combatCam", 3, &gameCombatCamera}, {"menu_reverseYaw", 3, &gameReverseCameraYaw},
        {"menu_vibration", 3, &padVibrationEnable}, {"menu_stereo", 3, &soundStereoEnable},
        {"menu_blood", 3, &g_BloodLevel}, {"menu_subtitles", 3, &subtitlesOn},
        {"menu_damagedisplay", 3, &damageDisplayOn}, {"menu_chat", 3, &chatOn},
        {"menu_markers", 3, &gameMultiPlayerMarkersOn},
    };
    CHECK(sizeof optionMenu == 14 * sizeof(norrath_menu_record));
    for (int i = 0; i < 13; ++i) {
        CHECK(optionMenu[i].label != nullptr && std::strcmp(optionMenu[i].label, expect[i].label) == 0);
        CHECK(optionMenu[i].kind == expect[i].kind);
        CHECK(optionMenu[i].value == expect[i].value);
        CHECK(optionMenu[i].flags == 0);
    }
    CHECK(optionMenu[13].label == nullptr && optionMenu[13].kind == 0 && optionMenu[13].value == nullptr &&
          optionMenu[13].flags == 0);
    if (failures == 0) std::printf("option_menu ok\n");
    return failures != 0;
}
