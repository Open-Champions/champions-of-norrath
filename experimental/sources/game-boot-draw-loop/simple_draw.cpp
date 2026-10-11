// Observed simpleDrawLoop at 0x002cfc68, 268 bytes.
// It clears frameNm, calls engineSetDisplayDisable(2), and stores 1 in
// supersampleMode. Each pass calls engineFrameStart(coreClearColor, 0),
// padProcess, the supplied function when it is non-null, and cdProcess.
// The function's v0 becomes the loop condition. No argument is written for
// that call. While gameControlPaused is greater than zero, a nonzero
// (pad+8 | pad+96) masked with 0xff0fffff keeps the counter; otherwise the
// loaded counter is reduced by one. frameNm is then increased by one and
// engineFrameEnd(0) and soundFrame run. A zero result repeats. A nonzero
// result calls gameClear(1), engineSetDisplayDisable(1), soundResetStream
// for 0, 1, and 2, and lumpClear(1).
// main at 0x002d0aa8 calls this with displaySOELogo at 0x002cd7d8. That is
// main's first call of this function, and it is not behind a branch.
// displaySOELogo draws before it returns, and this loop calls engineFrameEnd
// only after that return. A null function leaves the result at zero, so the
// loop does not end. No frame was presented by running this.
extern "C" void engineSetDisplayDisable__Fi(int value);
extern "C" void engineFrameStart__FUib(unsigned color, int second);
extern "C" void padProcess__Fv();
extern "C" void cdProcess__Fv();
extern "C" void engineFrameEnd__Fb(int value);
extern "C" void soundFrame__Fv();
extern "C" void gameClear__Fi(int value);
extern "C" void soundResetStream__Fi(int stream);
extern "C" void lumpClear__Fi(int value);
extern "C" unsigned frameNm;
extern "C" unsigned char supersampleMode;
extern "C" unsigned coreClearColor;
extern "C" unsigned gameControlPaused;
extern "C" unsigned char pad[];

namespace {
unsigned read_u32(const unsigned char* p) {
    return static_cast<unsigned>(p[0]) | (static_cast<unsigned>(p[1]) << 8) |
           (static_cast<unsigned>(p[2]) << 16) | (static_cast<unsigned>(p[3]) << 24);
}
}

extern "C" void simpleDrawLoop__FPFv_i(int (*callback)()) {
    frameNm = 0u;
    engineSetDisplayDisable__Fi(2);
    supersampleMode = 1u;
    int returned = 0;
    for (;;) {
        engineFrameStart__FUib(coreClearColor, 0);
        padProcess__Fv();
        if (callback != nullptr) returned = callback();
        cdProcess__Fv();
        const unsigned paused = gameControlPaused;
        if (static_cast<int>(paused) > 0) {
            const unsigned low = read_u32(pad + 8u) & 0xff0fffffu;
            const unsigned high = read_u32(pad + 96u) & 0xff0fffffu;
            if (low == 0u && high == 0u) gameControlPaused = paused - 1u;
        }
        frameNm = frameNm + 1u;
        engineFrameEnd__Fb(0);
        soundFrame__Fv();
        if (returned != 0) break;
    }
    gameClear__Fi(1);
    engineSetDisplayDisable__Fi(1);
    soundResetStream__Fi(0);
    soundResetStream__Fi(1);
    soundResetStream__Fi(2);
    lumpClear__Fi(1);
}
