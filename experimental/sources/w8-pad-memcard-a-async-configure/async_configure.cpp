// Observed: asyncConfigure__Fv at 0x0025b838 is 132 bytes. It looks up the text
// key "ConfigTitle" with textGetString__FPc and formats it with
// usprintf__FPUsT0ffffffff into a 128-element 16-bit buffer on the stack, passing
// 137.0f then seven zeros as the eight float arguments. It stores zero in the byte
// whichPad and calls runConfigure__FiPUsi(1, buffer, 0). A result other than 0
// or 2 calls newMode__Fii(-1, 0) and stores 1 in gameBailFromGameLoop.
// Inferred: nothing.
// Unknown: the meaning of the float arguments and of the runConfigure results.

extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* dest, unsigned short* format,
                                         float a, float b, float c, float d, float e,
                                         float f, float g, float h);
extern "C" int runConfigure__FiPUsi(int mode, unsigned short* title, int extra);
extern "C" int newMode__Fii(int mode, int keep);
extern "C" signed char whichPad;
extern "C" int gameBailFromGameLoop;

extern "C" void asyncConfigure__Fv() {
    unsigned short buffer[128];
    usprintf__FPUsT0ffffffff(buffer, textGetString__FPc(const_cast<char*>("ConfigTitle")),
                             137.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    whichPad = 0;
    int result = runConfigure__FiPUsi(1, buffer, 0);
    if (result != 0 && result != 2) {
        newMode__Fii(-1, 0);
        gameBailFromGameLoop = 1;
    }
}
