// Observed amxNetworkMode at 0x0013a6a0, 8 bytes. It returns the
// sign-extended byte netNetworkMode at 0x00483829.
extern "C" signed char netNetworkMode;

extern "C" int amxNetworkMode__FP5__amxPi(void*, int*) {
    return netNetworkMode;
}
