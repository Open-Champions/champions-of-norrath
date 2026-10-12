// Host contract for gamePopConfig__Fv. Not an EE plan input.
#include "pushed_config.hpp"
extern "C" {
PushedConfig pushedConfig;
unsigned soundStereoEnable, padVibrationEnable, DLG_ShowDialogText, subtitlesOn, damageDisplayOn, g_BloodLevel,
    gameMultiPlayerMarkersOn, gameCombatCamera, gameReverseCameraYaw, chatOn, commEnable, commEcho;
float soundSfxVolume, soundMusicVolume, engineBrightnessCorrection, commRecord, commThresh, commPlayback;
}
#include "pop_config.cpp"
int main() {
    PushedConfig& c = pushedConfig;
    c.soundStereoEnable = 1; c.padVibrationEnable = 2; c.DLG_ShowDialogText = 3; c.subtitlesOn = 4;
    c.damageDisplayOn = 5; c.g_BloodLevel = 6; c.gameMultiPlayerMarkersOn = 7; c.gameCombatCamera = 8;
    c.gameReverseCameraYaw = 9; c.chatOn = 10; c.commEnable = 11; c.commEcho = 12;
    c.soundSfxVolume = 0.25f; c.soundMusicVolume = 0.5f; c.engineBrightnessCorrection = 0.75f;
    c.commRecord = 1.25f; c.commThresh = 1.5f; c.commPlayback = 1.75f;
    gamePopConfig__Fv();
    if (soundStereoEnable != 1 || padVibrationEnable != 2 || DLG_ShowDialogText != 3 || subtitlesOn != 4 ||
        damageDisplayOn != 5 || g_BloodLevel != 6 || gameMultiPlayerMarkersOn != 7 || gameCombatCamera != 8 ||
        gameReverseCameraYaw != 9 || chatOn != 10 || commEnable != 11 || commEcho != 12) return 1;
    if (soundSfxVolume != 0.25f || soundMusicVolume != 0.5f || engineBrightnessCorrection != 0.75f ||
        commRecord != 1.25f || commThresh != 1.5f || commPlayback != 1.75f) return 2;
    return 0;
}
