// Host contract for gamePushConfig__Fv. Not an EE plan input.
#include "pushed_config.hpp"
#include <cstring>
extern "C" {
PushedConfig pushedConfig;
unsigned soundStereoEnable = 11, padVibrationEnable = 12, DLG_ShowDialogText = 13, subtitlesOn = 14,
    damageDisplayOn = 15, g_BloodLevel = 16, gameMultiPlayerMarkersOn = 17, gameCombatCamera = 18,
    gameReverseCameraYaw = 19, chatOn = 20, commEnable = 21, commEcho = 22;
float soundSfxVolume = 1.5f, soundMusicVolume = 2.5f, engineBrightnessCorrection = 3.5f, commRecord = 4.5f,
    commThresh = 5.5f, commPlayback = 6.5f;
}
#include "push_config.cpp"
int main() {
    std::memset(&pushedConfig, 0xaa, sizeof pushedConfig);
    gamePushConfig__Fv();
    const PushedConfig& c = pushedConfig;
    if (c.soundStereoEnable != 11 || c.padVibrationEnable != 12 || c.DLG_ShowDialogText != 13 ||
        c.subtitlesOn != 14 || c.damageDisplayOn != 15 || c.g_BloodLevel != 16 ||
        c.gameMultiPlayerMarkersOn != 17 || c.gameCombatCamera != 18 || c.gameReverseCameraYaw != 19 ||
        c.chatOn != 20 || c.commEnable != 21 || c.commEcho != 22) return 1;
    if (c.soundSfxVolume != 1.5f || c.soundMusicVolume != 2.5f || c.engineBrightnessCorrection != 3.5f ||
        c.commRecord != 4.5f || c.commThresh != 5.5f || c.commPlayback != 6.5f) return 2;
    // bytes outside the named fields stay as they were
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&c);
    for (unsigned i = 0; i < 4; ++i) if (p[i] != 0xaa) return 3;
    for (unsigned i = 52; i < 196; ++i) if (p[i] != 0xaa) return 4;
    for (unsigned i = 220; i < 304; ++i) if (p[i] != 0xaa) return 5;
    return 0;
}
