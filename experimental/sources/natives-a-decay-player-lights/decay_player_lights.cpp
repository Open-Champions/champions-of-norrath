// amxDecayPlayerLights is the 144-byte native at 0x0013c348. When
// nmPlayersAlive is 2, cell 2 supplies the scale; otherwise cell 1 does.
// Cell 3 supplies the delta. Both are converted to float and multiplied by
// the 0.01 constant 0x3c23d70a. gamePlayerLightAttenuate at 0x00484f24 is
// then updated by the ordered compares below. A positive delta stores the
// scale and returns 1 when the scale is less than or equal to the sum, and
// otherwise stores the sum and returns 0. A negative delta stores the scale
// and returns 1 when the sum is less than or equal to the scale, and
// otherwise stores the sum and returns 0. A zero or unordered delta stores
// the sum and returns 0.
extern "C" int nmPlayersAlive;
extern "C" float gamePlayerLightAttenuate;
static float tenth() {
    union {
        unsigned as_bits;
        float as_float;
    } value;
    value.as_bits = 0x3c23d70au;
    return value.as_float;
}
extern "C" int amxDecayPlayerLights__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    float scale = static_cast<float>(nmPlayersAlive == 2 ? params[2] : params[1]) * tenth();
    float delta = static_cast<float>(params[3]) * tenth();
    float current = gamePlayerLightAttenuate;
    float sum = current + delta;
    if (0.0f < delta) {
        if (scale <= sum) {
            gamePlayerLightAttenuate = scale;
            return 1;
        }
        gamePlayerLightAttenuate = sum;
        return 0;
    }
    if (delta < 0.0f) {
        if (sum <= scale) {
            gamePlayerLightAttenuate = scale;
            return 1;
        }
        gamePlayerLightAttenuate = sum;
        return 0;
    }
    gamePlayerLightAttenuate = sum;
    return 0;
}
