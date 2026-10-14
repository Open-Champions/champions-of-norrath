// Observed textSystemInit__Fv at 0x002cb360, 40 bytes. It calls
// VIStartKeyboardDriver__Fv, then Init__10VIKeyboard with the local object
// vik at 0x008d86a0. vik is .bss, st_size 0. outMsg is 536 bytes later.
// sif-keyboard keeps its own static copy of that object and does not export
// it, so this definition is a second object at the same original address.
extern "C" void VIStartKeyboardDriver__Fv();
extern "C" void Init__10VIKeyboard(void* keyboard);

extern "C" {
unsigned char vik[536] = {};
}

extern "C" void textSystemInit__Fv() {
    VIStartKeyboardDriver__Fv();
    Init__10VIKeyboard(vik);
}
