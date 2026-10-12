// Observed subDrawLoop__FPFv_i at 0x002d0de8, 228 bytes.
// It is simpleDrawLoop's per-frame loop without the exit cleanup. It clears
// frameNm, calls engineSetDisplayDisable(2) and stores 1 in supersampleMode.
// Each pass calls engineFrameStart(coreClearColor, 0), padProcess, the
// supplied function when it is non-null, and cdProcess. The function's
// return value is the exit condition and is kept across passes: a null
// function leaves it zero, so the loop only ends through a function that
// returns nonzero. While gameControlPaused is above zero, the counter is
// reduced by one only if both pad words at +8 and +96, masked with
// 0xff0fffff, are zero. frameNm is then increased, engineFrameEnd(0) and
// soundFrame run, and a nonzero stored result ends the loop. The only
// exit action is engineSetDisplayDisable(1): no gameClear, stream reset or
// lump clear occurs. The return type is unknown; nothing meaningful is left
// in the result register, so void is used.
extern "C" void engineSetDisplayDisable__Fi(int value);
extern "C" void engineFrameStart__FUib(unsigned color, int second);
extern "C" void padProcess__Fv();
extern "C" void cdProcess__Fv();
extern "C" void engineFrameEnd__Fb(int value);
extern "C" void soundFrame__Fv();
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

extern "C" void subDrawLoop__FPFv_i(int (*callback)()) {
    frameNm = 0u;
    engineSetDisplayDisable__Fi(2);
    supersampleMode = 1u;
    int returned = 0;
    do {
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
    } while (returned == 0);
    engineSetDisplayDisable__Fi(1);
}
