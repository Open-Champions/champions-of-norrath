// Observed texProcessDecompress__Fv at 0x00172838, 84 bytes.
// Reads the word at 0x10000000 (the EE timer 0 count register) first. If the
// pointer at 0x0048410c (texDecodeStarted) is nonnull, the current value of
// 0x004844a8 (drawFrameNm) is stored at byte offset 28 of the object it
// points to. Then WakeupThread is called with the value of 0x00486968
// (decompressThreadid). Finally the timer word is read again and the
// difference (second minus first, 32-bit wrap) is stored to 0x004840d0
// (texLastDecompressionTime). Nothing is returned.
// Symbol names come from the executable's symbol table. None of the globals
// or WakeupThread are defined in this package.
#if defined(NORRATH_HOST_TEX)
extern volatile unsigned norrath_host_timer;
#define TEX_TIMER norrath_host_timer
#else
#define TEX_TIMER (*reinterpret_cast<volatile unsigned*>(0x10000000u))
#endif

extern "C" {
extern unsigned* texDecodeStarted;
extern int drawFrameNm;
extern int decompressThreadid;
extern int texLastDecompressionTime;
int WakeupThread(int thread);
void texProcessDecompress__Fv() {
    const unsigned start = TEX_TIMER;
    if (texDecodeStarted != nullptr) {
        texDecodeStarted[7] = static_cast<unsigned>(drawFrameNm);
    }
    WakeupThread(decompressThreadid);
    texLastDecompressionTime = static_cast<int>(TEX_TIMER - start);
}
}
