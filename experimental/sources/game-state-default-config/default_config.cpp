// Observed gameSetDefaultGameConfig__Fi at 0x001cc778, 420 bytes.
// Argument -1 stores the scalar defaults, then writes every button record.
// Any other argument skips the scalars. Button record N is written only when
// the argument is -1 or N. The signed controlStyle byte selects the first
// three words: 1 writes 64, 32, 16 and 0 writes 32, 16, 64. Any other byte,
// including 0xff, writes no button words. The shared tail is 128, 8, 2, 1,
// 1024, 4. soundUpdateMusicVolume runs after the music word is stored and
// before the comm words. The commRecord bits are the .lit4 word 0x3f4ccccc
// at 0x00480660, which has no ELF symbol. Nothing here was executed.
extern "C" float soundSfxVolume;
extern "C" float soundMusicVolume;
extern "C" unsigned soundStereoEnable;
extern "C" float engineBrightnessCorrection;
extern "C" unsigned g_BloodLevel;
extern "C" unsigned padVibrationEnable;
extern "C" unsigned DLG_ShowDialogText;
extern "C" unsigned subtitlesOn;
extern "C" unsigned damageDisplayOn;
extern "C" unsigned gameMultiPlayerMarkersOn;
extern "C" unsigned gameCombatCamera;
extern "C" unsigned gameReverseCameraYaw;
extern "C" float commRecord;
extern "C" unsigned chatOn;
extern "C" float commThresh;
extern "C" float commPlayback;
extern "C" unsigned commEcho;
extern "C" unsigned commEnable;
extern "C" unsigned char controlStyle;
extern "C" unsigned char MC_GameConfig[];
extern "C" void soundUpdateMusicVolume__Fv();

static void write_u32(unsigned char* dest, unsigned value) {
    __builtin_memcpy(dest, &value, 4);
}

static void write_bits(float* dest, unsigned bits) {
    __builtin_memcpy(dest, &bits, 4);
}

static void write_button_record(unsigned index, int style) {
    unsigned char* record = MC_GameConfig + 52u + index * 36u;
    unsigned first = 32u;
    unsigned second = 16u;
    unsigned third = 64u;
    if (style == 1) {
        first = 64u;
        second = 32u;
        third = 16u;
    }
    write_u32(record + 0u, first);
    write_u32(record + 4u, second);
    write_u32(record + 8u, third);
    write_u32(record + 12u, 128u);
    write_u32(record + 16u, 8u);
    write_u32(record + 20u, 2u);
    write_u32(record + 24u, 1u);
    write_u32(record + 28u, 1024u);
    write_u32(record + 32u, 4u);
}

extern "C" void gameSetDefaultGameConfig__Fi(int player) {
    if (player == -1) {
        write_bits(&soundSfxVolume, 0x3f000000u);
        write_u32(MC_GameConfig + 12u, 0x3f000000u);
        write_u32(MC_GameConfig + 8u, 0x3f000000u);
        write_bits(&soundMusicVolume, 0x3f000000u);
        soundUpdateMusicVolume__Fv();
        write_bits(&commRecord, 0x3f4cccccu);
        write_bits(&commThresh, 0x3e800000u);
        write_bits(&commPlayback, 0x3f000000u);
        write_u32(MC_GameConfig + 212u, 1u);
        commEnable = 1u;
        DLG_ShowDialogText = 1u;
        write_u32(MC_GameConfig + 24u, 1u);
        subtitlesOn = 1u;
        write_u32(MC_GameConfig + 28u, 1u);
        damageDisplayOn = 1u;
        write_u32(MC_GameConfig + 32u, 1u);
        write_u32(MC_GameConfig + 16u, 1u);
        soundStereoEnable = 1u;
        write_u32(MC_GameConfig + 20u, 1u);
        padVibrationEnable = 1u;
        write_u32(MC_GameConfig + 36u, 0u);
        g_BloodLevel = 0u;
        write_u32(MC_GameConfig + 4u, 0x3f000000u);
        write_bits(&engineBrightnessCorrection, 0x3f000000u);
        write_u32(MC_GameConfig + 40u, 1u);
        gameMultiPlayerMarkersOn = 1u;
        write_u32(MC_GameConfig + 48u, 0u);
        gameReverseCameraYaw = 0u;
        write_u32(MC_GameConfig + 44u, 1u);
        gameCombatCamera = 1u;
        write_u32(MC_GameConfig + 196u, 1u);
        chatOn = 1u;
        write_u32(MC_GameConfig + 0u, 0u);
        write_u32(MC_GameConfig + 200u, 0x3f4cccccu);
        write_u32(MC_GameConfig + 204u, 0x3e800000u);
        write_u32(MC_GameConfig + 208u, 0x3f000000u);
        write_u32(MC_GameConfig + 216u, 0u);
        commEcho = 0u;
    }
    int style = static_cast<signed char>(controlStyle);
    for (int index = 0; index < 4; ++index) {
        if (player != -1 && player != index) continue;
        if (style != 0 && style != 1) continue;
        write_button_record(static_cast<unsigned>(index), style);
    }
}
