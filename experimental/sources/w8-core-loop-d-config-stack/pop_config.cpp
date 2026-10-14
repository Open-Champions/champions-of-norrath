// Observed gamePopConfig__Fv at 0x001cc9c0, 156 bytes. It is the inverse of
// gamePushConfig__Fv: each of the same eighteen settings globals is loaded
// from its field of pushedConfig (0x007d0b70, layout in pushed_config.hpp).
// Nothing else is read or written and nothing is returned.
// Unknown: when the original calls this and what the comm* values configure.
#include "pushed_config.hpp"
extern "C" {
extern unsigned soundStereoEnable, padVibrationEnable, DLG_ShowDialogText, subtitlesOn, damageDisplayOn,
    g_BloodLevel, gameMultiPlayerMarkersOn, gameCombatCamera, gameReverseCameraYaw, chatOn, commEnable, commEcho;
extern float soundSfxVolume, soundMusicVolume, engineBrightnessCorrection, commRecord, commThresh, commPlayback;
}

extern "C" void gamePopConfig__Fv() {
    soundStereoEnable = pushedConfig.soundStereoEnable;
    soundSfxVolume = pushedConfig.soundSfxVolume;
    soundMusicVolume = pushedConfig.soundMusicVolume;
    g_BloodLevel = pushedConfig.g_BloodLevel;
    engineBrightnessCorrection = pushedConfig.engineBrightnessCorrection;
    padVibrationEnable = pushedConfig.padVibrationEnable;
    DLG_ShowDialogText = pushedConfig.DLG_ShowDialogText;
    subtitlesOn = pushedConfig.subtitlesOn;
    damageDisplayOn = pushedConfig.damageDisplayOn;
    gameMultiPlayerMarkersOn = pushedConfig.gameMultiPlayerMarkersOn;
    gameCombatCamera = pushedConfig.gameCombatCamera;
    gameReverseCameraYaw = pushedConfig.gameReverseCameraYaw;
    chatOn = pushedConfig.chatOn;
    commRecord = pushedConfig.commRecord;
    commThresh = pushedConfig.commThresh;
    commPlayback = pushedConfig.commPlayback;
    commEcho = pushedConfig.commEcho;
    commEnable = pushedConfig.commEnable;
}
