// Observed gameMaybeSaveConfig__Fii at 0x001cca60, 1192 bytes.
// The first argument is saved and tested only when the difference count is
// zero. The second argument is tested after the comparisons. A nonzero second
// argument returns the count and does not write or call MC_SaveConfig.
// Float slots use c.eq.s. A mismatch, including NaN, increments the count.
// Word slots increment when the 32-bit patterns differ. The slots are
// soundSfxVolume at MC_GameConfig+12, soundMusicVolume at +8,
// soundStereoEnable at +16, engineBrightnessCorrection at +4, g_BloodLevel
// at +36, padVibrationEnable at +20, DLG_ShowDialogText at +24, subtitlesOn
// at +28, damageDisplayOn at +32, gameMultiPlayerMarkersOn at +40,
// gameCombatCamera at +44, gameReverseCameraYaw at +48, commRecord at +200,
// chatOn at +196, commThresh at +204, commPlayback at +208, commEcho at +216,
// and commEnable at +212.
// When netNetworkMode is zero, four 36-byte records starting at
// MC_GameConfig+52 are compared with attackButton, spellButton1,
// spellButton2, actionButton, blockButton, healthPotionButton,
// energyPotionButton, automapButton, and weaponToggleButton. Each button
// object contributes the word at index * 4. A nonzero network byte skips
// that compare.
// When the second argument is zero and the count is nonzero, the live scalar
// words are stored back into those MC_GameConfig offsets and the nine config
// words are stored onto the button objects. MC_SaveConfig then runs.
// When the count is zero, MC_SaveConfig runs only if the first argument is
// nonzero, and the button and scalar stores are skipped. The returned value
// is the count. Nothing here was executed.
extern "C" unsigned char MC_GameConfig[252];
extern "C" unsigned soundSfxVolume;
extern "C" unsigned soundMusicVolume;
extern "C" unsigned soundStereoEnable;
extern "C" unsigned engineBrightnessCorrection;
extern "C" unsigned g_BloodLevel;
extern "C" unsigned padVibrationEnable;
extern "C" unsigned DLG_ShowDialogText;
extern "C" unsigned subtitlesOn;
extern "C" unsigned damageDisplayOn;
extern "C" unsigned gameMultiPlayerMarkersOn;
extern "C" unsigned gameCombatCamera;
extern "C" unsigned gameReverseCameraYaw;
extern "C" unsigned commRecord;
extern "C" unsigned chatOn;
extern "C" unsigned commThresh;
extern "C" unsigned commPlayback;
extern "C" unsigned commEcho;
extern "C" unsigned commEnable;
extern "C" unsigned char netNetworkMode;
extern "C" unsigned attackButton[4];
extern "C" unsigned spellButton1[4];
extern "C" unsigned spellButton2[4];
extern "C" unsigned actionButton[4];
extern "C" unsigned blockButton[4];
extern "C" unsigned healthPotionButton[4];
extern "C" unsigned energyPotionButton[4];
extern "C" unsigned automapButton[4];
extern "C" unsigned weaponToggleButton[4];
extern "C" void MC_SaveConfig__Fv();

static unsigned read_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

static void write_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}

static int float_differs(unsigned left, unsigned right) {
    float left_value = 0.0f;
    float right_value = 0.0f;
    __builtin_memcpy(&left_value, &left, 4);
    __builtin_memcpy(&right_value, &right, 4);
    return left_value != right_value;
}

static int count_button_differences() {
    int differences = 0;
    unsigned* const live[9] = {
        attackButton, spellButton1, spellButton2, actionButton, blockButton,
        healthPotionButton, energyPotionButton, automapButton, weaponToggleButton
    };
    for (unsigned index = 0; index < 4u; ++index) {
        const unsigned char* record = MC_GameConfig + 52u + index * 36u;
        for (unsigned word = 0; word < 9u; ++word) {
            if (live[word][index] != read_u32(record + word * 4u)) ++differences;
        }
    }
    return differences;
}

static void store_buttons_from_config() {
    unsigned* const live[9] = {
        attackButton, spellButton1, spellButton2, actionButton, blockButton,
        healthPotionButton, energyPotionButton, automapButton, weaponToggleButton
    };
    for (unsigned index = 0; index < 4u; ++index) {
        const unsigned char* record = MC_GameConfig + 52u + index * 36u;
        for (unsigned word = 0; word < 9u; ++word) live[word][index] = read_u32(record + word * 4u);
    }
}

extern "C" int gameMaybeSaveConfig__Fii(int first, int second) {
    int differences = 0;
    if (float_differs(soundSfxVolume, read_u32(MC_GameConfig + 12))) ++differences;
    if (float_differs(soundMusicVolume, read_u32(MC_GameConfig + 8))) ++differences;
    if (soundStereoEnable != read_u32(MC_GameConfig + 16)) ++differences;
    if (float_differs(engineBrightnessCorrection, read_u32(MC_GameConfig + 4))) ++differences;
    if (g_BloodLevel != read_u32(MC_GameConfig + 36)) ++differences;
    if (padVibrationEnable != read_u32(MC_GameConfig + 20)) ++differences;
    if (DLG_ShowDialogText != read_u32(MC_GameConfig + 24)) ++differences;
    if (subtitlesOn != read_u32(MC_GameConfig + 28)) ++differences;
    if (damageDisplayOn != read_u32(MC_GameConfig + 32)) ++differences;
    if (gameMultiPlayerMarkersOn != read_u32(MC_GameConfig + 40)) ++differences;
    if (gameCombatCamera != read_u32(MC_GameConfig + 44)) ++differences;
    if (gameReverseCameraYaw != read_u32(MC_GameConfig + 48)) ++differences;
    if (float_differs(commRecord, read_u32(MC_GameConfig + 200))) ++differences;
    if (chatOn != read_u32(MC_GameConfig + 196)) ++differences;
    if (float_differs(commThresh, read_u32(MC_GameConfig + 204))) ++differences;
    if (float_differs(commPlayback, read_u32(MC_GameConfig + 208))) ++differences;
    if (commEcho != read_u32(MC_GameConfig + 216)) ++differences;
    if (commEnable != read_u32(MC_GameConfig + 212)) ++differences;
    if (netNetworkMode == 0) differences += count_button_differences();
    if (second != 0) return differences;
    if (differences != 0) {
        write_u32(MC_GameConfig + 12, soundSfxVolume);
        write_u32(MC_GameConfig + 8, soundMusicVolume);
        write_u32(MC_GameConfig + 16, soundStereoEnable);
        write_u32(MC_GameConfig + 4, engineBrightnessCorrection);
        write_u32(MC_GameConfig + 36, g_BloodLevel);
        write_u32(MC_GameConfig + 20, padVibrationEnable);
        write_u32(MC_GameConfig + 24, DLG_ShowDialogText);
        write_u32(MC_GameConfig + 28, subtitlesOn);
        write_u32(MC_GameConfig + 32, damageDisplayOn);
        write_u32(MC_GameConfig + 40, gameMultiPlayerMarkersOn);
        write_u32(MC_GameConfig + 44, gameCombatCamera);
        write_u32(MC_GameConfig + 48, gameReverseCameraYaw);
        write_u32(MC_GameConfig + 200, commRecord);
        write_u32(MC_GameConfig + 196, chatOn);
        write_u32(MC_GameConfig + 204, commThresh);
        write_u32(MC_GameConfig + 208, commPlayback);
        write_u32(MC_GameConfig + 216, commEcho);
        write_u32(MC_GameConfig + 212, commEnable);
        store_buttons_from_config();
    }
    if (differences != 0 || first != 0) MC_SaveConfig__Fv();
    return differences;
}
