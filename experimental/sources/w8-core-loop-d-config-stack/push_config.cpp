// Observed gamePushConfig__Fv at 0x001cc920, 156 bytes. It copies eighteen
// settings globals into pushedConfig (0x007d0b70) and returns nothing. Each
// field of PushedConfig is named for the global it receives; the offsets are
// listed in pushed_config.hpp. Nothing else is read or written.
// Unknown: when the original calls this and what the comm* values configure.
#include "pushed_config.hpp"
extern "C" {
extern unsigned soundStereoEnable, padVibrationEnable, DLG_ShowDialogText, subtitlesOn, damageDisplayOn,
    g_BloodLevel, gameMultiPlayerMarkersOn, gameCombatCamera, gameReverseCameraYaw, chatOn, commEnable, commEcho;
extern float soundSfxVolume, soundMusicVolume, engineBrightnessCorrection, commRecord, commThresh, commPlayback;
}

extern "C" void gamePushConfig__Fv() {
    pushedConfig.soundStereoEnable = soundStereoEnable;
    pushedConfig.soundSfxVolume = soundSfxVolume;
    pushedConfig.soundMusicVolume = soundMusicVolume;
    pushedConfig.g_BloodLevel = g_BloodLevel;
    pushedConfig.engineBrightnessCorrection = engineBrightnessCorrection;
    pushedConfig.padVibrationEnable = padVibrationEnable;
    pushedConfig.DLG_ShowDialogText = DLG_ShowDialogText;
    pushedConfig.subtitlesOn = subtitlesOn;
    pushedConfig.damageDisplayOn = damageDisplayOn;
    pushedConfig.gameMultiPlayerMarkersOn = gameMultiPlayerMarkersOn;
    pushedConfig.gameCombatCamera = gameCombatCamera;
    pushedConfig.gameReverseCameraYaw = gameReverseCameraYaw;
    pushedConfig.chatOn = chatOn;
    pushedConfig.commRecord = commRecord;
    pushedConfig.commThresh = commThresh;
    pushedConfig.commPlayback = commPlayback;
    pushedConfig.commEcho = commEcho;
    pushedConfig.commEnable = commEnable;
}
