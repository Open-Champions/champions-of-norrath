// Observed: optionMenu at 0x00428880 is 224 bytes: thirteen 16-byte records
// and one all-zero record. A second table (no symbol) starts directly after
// at 0x00428960 and is not part of this object. Records (label key, kind,
// value object, flags 0 in every record):
//   menu_bright       4  engineBrightnessCorrection
//   menu_music        4  soundMusicVolume
//   menu_sfx          4  soundSfxVolume
//   menu_controls     0  none
//   menu_combatCam    3  gameCombatCamera
//   menu_reverseYaw   3  gameReverseCameraYaw
//   menu_vibration    3  padVibrationEnable
//   menu_stereo       3  soundStereoEnable
//   menu_blood        3  g_BloodLevel
//   menu_subtitles    3  subtitlesOn
//   menu_damagedisplay 3 damageDisplayOn
//   menu_chat         3  chatOn
//   menu_markers      3  gameMultiPlayerMarkersOn
// menuRun treats kind 4 as a float slider and kind 3 as an on/off int; the
// first three value objects are read as floats elsewhere in the image.
// Inferred: the kind numbers by menuRun's use. Unknown: the original types.
// A separate local object named optionMenu at 0x0037eff0 is not this one.
struct norrath_menu_record {
    const char* label;
    unsigned kind;
    void* value;
    unsigned flags;
};
static_assert(__SIZEOF_POINTER__ != 4 || sizeof(norrath_menu_record) == 16, "records are 16 bytes");

extern "C" {
extern float engineBrightnessCorrection;
extern float soundMusicVolume;
extern float soundSfxVolume;
extern int gameCombatCamera;
extern int gameReverseCameraYaw;
extern int padVibrationEnable;
extern int soundStereoEnable;
extern int g_BloodLevel;
extern int subtitlesOn;
extern int damageDisplayOn;
extern int chatOn;
extern int gameMultiPlayerMarkersOn;

norrath_menu_record optionMenu[14] = {
    {"menu_bright", 4u, &engineBrightnessCorrection, 0u},
    {"menu_music", 4u, &soundMusicVolume, 0u},
    {"menu_sfx", 4u, &soundSfxVolume, 0u},
    {"menu_controls", 0u, 0, 0u},
    {"menu_combatCam", 3u, &gameCombatCamera, 0u},
    {"menu_reverseYaw", 3u, &gameReverseCameraYaw, 0u},
    {"menu_vibration", 3u, &padVibrationEnable, 0u},
    {"menu_stereo", 3u, &soundStereoEnable, 0u},
    {"menu_blood", 3u, &g_BloodLevel, 0u},
    {"menu_subtitles", 3u, &subtitlesOn, 0u},
    {"menu_damagedisplay", 3u, &damageDisplayOn, 0u},
    {"menu_chat", 3u, &chatOn, 0u},
    {"menu_markers", 3u, &gameMultiPlayerMarkersOn, 0u},
    {0, 0u, 0, 0u},
};
}
