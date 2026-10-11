// Observed amxSetFarClip__FP5__amxPi at 0x0013b6d0, 36 bytes. Cell 1 is
// converted from a signed word to a float and passed to worldSetFarClip.
// The native returns 0.
extern "C" void worldSetFarClip__Ff(float distance);

extern "C" int amxSetFarClip__FP5__amxPi(void*, int* params) {
    worldSetFarClip__Ff(static_cast<float>(params[1]));
    return 0;
}
