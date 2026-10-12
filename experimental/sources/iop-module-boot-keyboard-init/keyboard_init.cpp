// Observed Init__10VIKeyboard at 0x002cbf30, 84 bytes. It clears
// gKeyboardAttached at 0x004865f0, prints the init banner, then writes the
// object: -1 at offset 4, the address of sQueue at offset 8, and 0 at
// offsets 528, 12, and 0, in that order. It returns 0. sQueue at 0x008d8d48
// is .bss, st_size 0. stack begins 264 bytes later. gKeyboardAttached is the
// four zero bytes in .sdata. Bytes between the written words are not stored.
extern "C" int printf(const char* text, ...);

extern "C" {
int gKeyboardAttached = 0;
unsigned char sQueue[264] = {};
}

extern "C" int Init__10VIKeyboard(void* keyboard) {
    auto* words = static_cast<unsigned*>(keyboard);
    gKeyboardAttached = 0;
    printf("======= INIT KEYBOARD =======\n");
    words[1] = 0xffffffffu;
    words[2] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(sQueue));
    words[132] = 0;
    words[3] = 0;
    words[0] = 0;
    return 0;
}
