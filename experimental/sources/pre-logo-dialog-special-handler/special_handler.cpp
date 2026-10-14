// Observed animSetSpecialSoundHandler__FPFiG6Point3_v at 0x00164020, 8 bytes.
// It stores the function pointer in the word specialHandler at 0x0048691c
// and returns. The next symbol, CDmode, begins four bytes later. The ELF
// size of specialHandler is 0. specialHandler is not defined here.
extern "C" void (*specialHandler)(int code, const unsigned char* point);

extern "C" void animSetSpecialSoundHandler__FPFiG6Point3_v(void (*handler)(int code, const unsigned char* point)) {
    specialHandler = handler;
}
