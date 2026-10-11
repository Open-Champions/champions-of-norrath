// Observed amxShakeCamera__FP5__amxPi at 0x0013c208, 40 bytes. Cell 1 is
// converted from a signed word to a float. gameSetCameraShake receives
// that float and a zero word. The native returns 0.
extern "C" void gameSetCameraShake__Ffb(float amount, int held);

extern "C" int amxShakeCamera__FP5__amxPi(void*, int* params) {
    gameSetCameraShake__Ffb(static_cast<float>(params[1]), 0);
    return 0;
}
