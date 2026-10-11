// Observed amxShopRunning__FP5__amxPi at 0x0013b6c8, 8 bytes. It returns
// the shopRunning word and does not read the AMX arguments.
extern "C" int shopRunning;

extern "C" int amxShopRunning__FP5__amxPi(void*, int*) {
    return shopRunning;
}
