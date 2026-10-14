#include <cstdio>
#include <cstring>

extern "C" unsigned char floorDrawEnable = 0;
extern "C" unsigned char objectDrawEnable = 0;
extern "C" int gameNoDraw = 0;
extern "C" unsigned char gameOver = 0;
extern "C" int gamePause = 0;
extern "C" int gameDifficulty = 0;
extern "C" unsigned char MC_GameConfig[252] = {};
extern "C" int amxSaveEndConfig__FP5__amxPi(void*, int*);

static int saves;

extern "C" void MC_SaveConfig__Fv() { saves += 1; }

static unsigned config_word() {
    unsigned value = 0;
    std::memcpy(&value, MC_GameConfig, 4);
    return value;
}

static int check_common() {
    if (floorDrawEnable != 0 || objectDrawEnable != 0) return 1;
    if (gameNoDraw != 1 || gameOver != 1 || gamePause != 1) return 1;
    return 0;
}

int main() {
    int params[1] = {0};
    std::memset(MC_GameConfig, 0, sizeof MC_GameConfig);
    floorDrawEnable = 1;
    objectDrawEnable = 1;
    gameDifficulty = 1;
    saves = 0;
    if (amxSaveEndConfig__FP5__amxPi(nullptr, params) != 0) return 1;
    if (check_common() || config_word() != 1u || saves != 1) return 2;
    gameDifficulty = 2;
    floorDrawEnable = 1;
    if (amxSaveEndConfig__FP5__amxPi(nullptr, params) != 0) return 3;
    if (check_common() || config_word() != 3u || saves != 2) return 4;
    gameDifficulty = 0;
    unsigned before = config_word();
    if (amxSaveEndConfig__FP5__amxPi(nullptr, params) != 0) return 5;
    if (check_common() || config_word() != before || saves != 3) return 6;
    gameDifficulty = 3;
    if (amxSaveEndConfig__FP5__amxPi(nullptr, params) != 0) return 7;
    if (config_word() != before) return 8;
    std::printf("save end config passed\n");
    return 0;
}
