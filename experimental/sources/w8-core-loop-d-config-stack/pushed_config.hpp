#pragma once
// Observed layout of pushedConfig (0x007d0b70), the 304-byte block that
// gamePushConfig__Fv fills and gamePopConfig__Fv reads back. Only the
// offsets listed are touched by those two routines; the rest is unnamed
// padding here. Each field is named after the global it is copied from.
// Unknown: what other code reads the block, and the bytes between the fields.
struct PushedConfig {
    unsigned char gap0[4];
    float engineBrightnessCorrection;      // +4
    float soundMusicVolume;                // +8
    float soundSfxVolume;                  // +12
    unsigned soundStereoEnable;            // +16
    unsigned padVibrationEnable;           // +20
    unsigned DLG_ShowDialogText;           // +24
    unsigned subtitlesOn;                  // +28
    unsigned damageDisplayOn;              // +32
    unsigned g_BloodLevel;                 // +36
    unsigned gameMultiPlayerMarkersOn;     // +40
    unsigned gameCombatCamera;             // +44
    unsigned gameReverseCameraYaw;         // +48
    unsigned char gap1[144];
    unsigned chatOn;                       // +196
    float commRecord;                      // +200
    float commThresh;                      // +204
    float commPlayback;                    // +208
    unsigned commEnable;                   // +212
    unsigned commEcho;                     // +216
    unsigned char gap2[84];
};
static_assert(sizeof(PushedConfig) == 304, "pushedConfig extent");
static_assert(__builtin_offsetof(PushedConfig, gameReverseCameraYaw) == 48, "offset");
static_assert(__builtin_offsetof(PushedConfig, chatOn) == 196, "offset");
static_assert(__builtin_offsetof(PushedConfig, commEcho) == 216, "offset");
extern "C" PushedConfig pushedConfig;
