extern "C" int nmPlayersAlive = 0;
extern "C" float gamePlayerLightAttenuate = 0.0f;
extern "C" int amxDecayPlayerLights__FP5__amxPi(void*, int*);
static float tenth() {
    union {
        unsigned as_bits;
        float as_float;
    } value;
    value.as_bits = 0x3c23d70au;
    return value.as_float;
}
int main() {
    int params[4] = {12, 0, 100, 200};
    nmPlayersAlive = 1;
    gamePlayerLightAttenuate = 0.0f;
    if (amxDecayPlayerLights__FP5__amxPi(0, params) != 1) return 1;
    if (gamePlayerLightAttenuate != 0.0f) return 2;
    params[1] = 1000000;
    gamePlayerLightAttenuate = 0.0f;
    float sum = 200.0f * tenth();
    if (amxDecayPlayerLights__FP5__amxPi(0, params) != 0) return 3;
    if (gamePlayerLightAttenuate != sum) return 4;
    params[3] = -200;
    gamePlayerLightAttenuate = 5.0f;
    if (amxDecayPlayerLights__FP5__amxPi(0, params) != 1) return 5;
    if (gamePlayerLightAttenuate != 1000000.0f * tenth()) return 6;
    params[1] = 0;
    gamePlayerLightAttenuate = 5.0f;
    if (amxDecayPlayerLights__FP5__amxPi(0, params) != 0) return 7;
    if (gamePlayerLightAttenuate != 5.0f + (-200.0f * tenth())) return 8;
    params[3] = 0;
    params[1] = 100;
    gamePlayerLightAttenuate = 50.0f;
    if (amxDecayPlayerLights__FP5__amxPi(0, params) != 0) return 11;
    if (gamePlayerLightAttenuate != 50.0f) return 12;
    nmPlayersAlive = 2;
    params[1] = 0;
    params[2] = 1000000;
    params[3] = 200;
    gamePlayerLightAttenuate = 0.0f;
    if (amxDecayPlayerLights__FP5__amxPi(0, params) != 0) return 9;
    if (gamePlayerLightAttenuate != 200.0f * tenth()) return 10;
    return 0;
}
