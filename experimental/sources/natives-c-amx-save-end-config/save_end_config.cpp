// Observed amxSaveEndConfig__FP5__amxPi at 0x0013c058, 116 bytes. It clears
// the floorDrawEnable and objectDrawEnable bytes, stores 1 in gameNoDraw,
// gameOver, and gamePause, then ors bit 0 or bit 1 of the first
// MC_GameConfig word when gameDifficulty is 1 or 2. It always calls
// MC_SaveConfig and returns 0.
extern "C" unsigned char floorDrawEnable;
extern "C" unsigned char objectDrawEnable;
extern "C" int gameNoDraw;
extern "C" unsigned char gameOver;
extern "C" int gamePause;
extern "C" int gameDifficulty;
extern "C" unsigned char MC_GameConfig[];
extern "C" void MC_SaveConfig__Fv();

namespace {
unsigned load_word(const unsigned char* bytes) {
    unsigned value;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = bytes[0];
    out[1] = bytes[1];
    out[2] = bytes[2];
    out[3] = bytes[3];
    return value;
}

void store_word(unsigned char* bytes, unsigned value) {
    auto* in = reinterpret_cast<const unsigned char*>(&value);
    bytes[0] = in[0];
    bytes[1] = in[1];
    bytes[2] = in[2];
    bytes[3] = in[3];
}
}

extern "C" int amxSaveEndConfig__FP5__amxPi(void*, int*) {
    const int difficulty = gameDifficulty;
    floorDrawEnable = 0;
    objectDrawEnable = 0;
    gameNoDraw = 1;
    gameOver = 1;
    gamePause = 1;
    if (difficulty == 1) store_word(MC_GameConfig, load_word(MC_GameConfig) | 1u);
    else if (difficulty == 2) store_word(MC_GameConfig, load_word(MC_GameConfig) | 2u);
    MC_SaveConfig__Fv();
    return 0;
}
