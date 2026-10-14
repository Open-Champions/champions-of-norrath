// Observed gameInit at 0x001ccf08, 724 bytes.
// It registers gameCameraTask with 11, the unnamed bytes "camera" at
// 0x00484eb0, and 0, then gameDrawWorld with 30, the unnamed bytes
// "drawWorld" at 0x0046c9a0, and 1. controlStyle is a signed byte.
// Value 1 writes one button set. Value 0 writes a different set and leaves
// infoButton and infoButtonFontChar alone. Any other value writes neither set.
// It then copies four 36-byte records from MC_GameConfig, which is 252 bytes
// at 0x0042e300. The records begin at offset 52. Each record's nine words go
// to attackButton, spellButton1, spellButton2, actionButton, blockButton,
// healthPotionButton, energyPotionButton, automapButton, and
// weaponToggleButton. Those destinations are 16 bytes each.
// The same object is then copied by four-byte loads:
//   12 -> soundSfxVolume, 8 -> soundMusicVolume, then soundUpdateMusicVolume
//   4 -> engineBrightnessCorrection, 16 -> soundStereoEnable
//   20 -> padVibrationEnable, 24 -> DLG_ShowDialogText
//   28 -> subtitlesOn, 32 -> damageDisplayOn
//   40 -> gameMultiPlayerMarkersOn, 44 -> gameCombatCamera
//   48 -> gameReverseCameraYaw, 196 -> chatOn
//   200 -> commRecord, 204 -> commThresh, 208 -> commPlayback
//   212 -> commEnable, 216 -> commEcho, 36 -> g_BloodLevel
// The float slots are lwc1/swc1, so the four bytes are copied as they stand.
// g_BloodLevel is stored before lumpLoad of the unnamed bytes "globsnd.lmp"
// at 0x0046c530. Then helpMessageInit, gameResetGameState,
// animSetSpecialSoundHandler(specialSoundHandler), and
// worldSetLightHaloTexture(haloTexture2). The three text addresses have no
// ELF symbol. Nothing here was executed.
extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int flag);
extern "C" void gameCameraTask__Fv();
extern "C" void gameDrawWorld__Fv();
extern "C" void soundUpdateMusicVolume__Fv();
extern "C" void lumpLoad__FPc(char* name);
extern "C" void helpMessageInit__Fv();
extern "C" void gameResetGameState__Fv();
extern "C" void animSetSpecialSoundHandler__FPFiG6Point3_v(void (*handler)());
extern "C" void specialSoundHandler__FiG6Point3();
extern "C" void worldSetLightHaloTexture__FP8_texture(void* texture);

extern "C" signed char controlStyle;
extern "C" unsigned dropButton;
extern "C" unsigned infoButton;
extern "C" unsigned dropButtonFontChar;
extern "C" unsigned infoButtonFontChar;
extern "C" unsigned padAcceptButtonFontChar;
extern "C" unsigned padBackButtonFontChar;
extern "C" unsigned padAcceptButton;
extern "C" unsigned padBackButton;
extern "C" unsigned equipButton;
extern "C" unsigned equipButtonFontChar;
extern "C" unsigned char MC_GameConfig[252];
extern "C" unsigned attackButton[4];
extern "C" unsigned spellButton1[4];
extern "C" unsigned spellButton2[4];
extern "C" unsigned actionButton[4];
extern "C" unsigned blockButton[4];
extern "C" unsigned healthPotionButton[4];
extern "C" unsigned energyPotionButton[4];
extern "C" unsigned automapButton[4];
extern "C" unsigned weaponToggleButton[4];
extern "C" unsigned soundSfxVolume;
extern "C" unsigned soundMusicVolume;
extern "C" unsigned engineBrightnessCorrection;
extern "C" unsigned soundStereoEnable;
extern "C" unsigned padVibrationEnable;
extern "C" unsigned DLG_ShowDialogText;
extern "C" unsigned subtitlesOn;
extern "C" unsigned damageDisplayOn;
extern "C" unsigned gameMultiPlayerMarkersOn;
extern "C" unsigned gameCombatCamera;
extern "C" unsigned gameReverseCameraYaw;
extern "C" unsigned chatOn;
extern "C" unsigned commRecord;
extern "C" unsigned commThresh;
extern "C" unsigned commPlayback;
extern "C" unsigned commEnable;
extern "C" unsigned commEcho;
extern "C" unsigned g_BloodLevel;
extern "C" void* haloTexture2;

namespace {
unsigned read_u32(const unsigned char* p) {
    return static_cast<unsigned>(p[0]) | (static_cast<unsigned>(p[1]) << 8) |
           (static_cast<unsigned>(p[2]) << 16) | (static_cast<unsigned>(p[3]) << 24);
}
}

extern "C" void gameInit__Fv() {
    engineAddTask__FPFv_viPci(gameCameraTask__Fv, 11, const_cast<char*>("camera"), 0);
    engineAddTask__FPFv_viPci(gameDrawWorld__Fv, 30, const_cast<char*>("drawWorld"), 1);
    const signed char style = controlStyle;
    if (style == 1) {
        dropButton = 16u;
        infoButton = 128u;
        dropButtonFontChar = 124u;
        infoButtonFontChar = 125u;
        padAcceptButtonFontChar = 126u;
        padBackButtonFontChar = 123u;
        padAcceptButton = 64u;
        padBackButton = 32u;
        equipButton = 64u;
        equipButtonFontChar = 126u;
    } else if (style == 0) {
        dropButton = 128u;
        dropButtonFontChar = 124u;
        padAcceptButtonFontChar = 123u;
        padBackButtonFontChar = 126u;
        padAcceptButton = 32u;
        padBackButton = 320u;
        equipButton = 32u;
        equipButtonFontChar = 123u;
    }
    for (unsigned index = 0u; index < 4u; ++index) {
        const unsigned char* record = MC_GameConfig + 52u + index * 36u;
        attackButton[index] = read_u32(record + 0u);
        spellButton1[index] = read_u32(record + 4u);
        spellButton2[index] = read_u32(record + 8u);
        actionButton[index] = read_u32(record + 12u);
        blockButton[index] = read_u32(record + 16u);
        healthPotionButton[index] = read_u32(record + 20u);
        energyPotionButton[index] = read_u32(record + 24u);
        automapButton[index] = read_u32(record + 28u);
        weaponToggleButton[index] = read_u32(record + 32u);
    }
    soundSfxVolume = read_u32(MC_GameConfig + 12u);
    soundMusicVolume = read_u32(MC_GameConfig + 8u);
    soundUpdateMusicVolume__Fv();
    engineBrightnessCorrection = read_u32(MC_GameConfig + 4u);
    soundStereoEnable = read_u32(MC_GameConfig + 16u);
    padVibrationEnable = read_u32(MC_GameConfig + 20u);
    DLG_ShowDialogText = read_u32(MC_GameConfig + 24u);
    subtitlesOn = read_u32(MC_GameConfig + 28u);
    damageDisplayOn = read_u32(MC_GameConfig + 32u);
    gameMultiPlayerMarkersOn = read_u32(MC_GameConfig + 40u);
    gameCombatCamera = read_u32(MC_GameConfig + 44u);
    gameReverseCameraYaw = read_u32(MC_GameConfig + 48u);
    chatOn = read_u32(MC_GameConfig + 196u);
    commRecord = read_u32(MC_GameConfig + 200u);
    commThresh = read_u32(MC_GameConfig + 204u);
    commPlayback = read_u32(MC_GameConfig + 208u);
    commEnable = read_u32(MC_GameConfig + 212u);
    commEcho = read_u32(MC_GameConfig + 216u);
    g_BloodLevel = read_u32(MC_GameConfig + 36u);
    lumpLoad__FPc(const_cast<char*>("globsnd.lmp"));
    helpMessageInit__Fv();
    gameResetGameState__Fv();
    animSetSpecialSoundHandler__FPFiG6Point3_v(specialSoundHandler__FiG6Point3);
    worldSetLightHaloTexture__FP8_texture(haloTexture2);
}
