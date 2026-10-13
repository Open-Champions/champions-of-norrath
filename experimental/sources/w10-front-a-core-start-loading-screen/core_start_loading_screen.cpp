// Observed: coreStartLoadingScreen__Fv at 0x002cdb80 is 1564 bytes and returns nothing. It does
// nothing at all when coreLoadingScreenRunning (signed byte) is nonzero. Otherwise, in order:
//  1. eRandState = eRandState * 0x19660d + 0x3c6ef35f; loadFrames = 0;
//     sprintf(name, "load%d.lmp", (eRandState >> 16) % 5), the remainder being taken unsigned;
//     lump = lumpLoad__FPc(name).
//  2. Six textures are looked up with lumpFindResource__FPcT0(name, ...) in this order: one chosen
//     by the signed byte language (1 "titleloadsp.tex", 2 "titleloadit.tex", 3 "titleloadfr.tex",
//     4 "titleloadgr.tex", anything else "titleload.tex"), then "scrollmap.tex",
//     "scrollmapb.tex", "blackout.tex", "foregrnd.tex" and "foregrndb.tex".
//  3. drawSizeOverEnable = 0; engineDisplayDisable = 4. Then three times: engineFrameStart__FUib
//     (0x80000000, 1); textureDrawColor__FP8_textureiiiUi(texture, 0, 0, 6, 0x80606060) for the
//     six textures in the order above; each texture's first 64 bytes are copied to the next of six
//     consecutive 64-byte slots (titleloadTexCopy, the two of scrollmapTexCopy, blackoutTexCopy,
//     the two of foregrndTexCopy); engineFrameEnd__Fb(0).
//  4. drawListClear__Fv(); drawSizeOverEnable = 1; lumpUnload__FPv(lump).
//  5. When loadThreadID is -1, the thread parameter block loadThreadParam gets the entry address
//     of loadThreadMain__FPv at word 1, the address of loadStack at word 2, 8192 at word 3, the
//     global pointer register at word 4 and 5 at word 5, and loadThreadID = CreateThread(block).
//  6. coreLoadingScreenRunning = 1; loadingScreenRequestExit = 0; StartThread(loadThreadID, 0).
// Inferred: the layout of loadThreadParam follows the EE kernel's thread-parameter block (entry,
// stack, stack size, global pointer, priority); the six slots keep copies of the textures for the
// loading thread to draw after the lump is gone; the entry points to the thread.
// Unknown: the 64-byte layout of a texture, the exact use of the 3 frames, and the sizes of the
// parameter block and stack beyond the words used here.

extern "C" {
extern signed char coreLoadingScreenRunning;
extern signed char loadingScreenRequestExit;
extern signed char language;
extern int loadFrames;
extern int loadThreadID;
extern unsigned eRandState;
extern int drawSizeOverEnable;
extern int engineDisplayDisable;
extern unsigned loadThreadParam[];
extern unsigned char loadStack[];
int sprintf(char* out, const char* format, ...);
void* lumpLoad__FPc(const char* name);
void lumpUnload__FPv(void* lump);
void* lumpFindResource__FPcT0(char* lump, const char* name);
void engineFrameStart__FUib(unsigned color, int flag);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color);
void engineFrameEnd__Fb(bool flag);
void drawListClear__Fv();
void loadThreadMain__FPv(void* argument);
int CreateThread(void* param);
void StartThread(int id, int argument);
}

namespace {
struct Texture64 {
    unsigned char bytes[64];
};
}

extern "C" Texture64 titleloadTexCopy;
extern "C" Texture64 scrollmapTexCopy[2];
extern "C" Texture64 blackoutTexCopy;
extern "C" Texture64 foregrndTexCopy[2];

extern "C" void coreStartLoadingScreen__Fv() {
    if (coreLoadingScreenRunning) return;
    eRandState = eRandState * 0x19660d + 0x3c6ef35f;
    loadFrames = 0;
    char name[80];
    sprintf(name, "load%d.lmp", (eRandState >> 16) % 5);
    void* lump = lumpLoad__FPc(name);
    const char* title;
    switch (language) {
        case 1: title = "titleloadsp.tex"; break;
        case 2: title = "titleloadit.tex"; break;
        case 3: title = "titleloadfr.tex"; break;
        case 4: title = "titleloadgr.tex"; break;
        default: title = "titleload.tex"; break;
    }
    Texture64* t0 = static_cast<Texture64*>(lumpFindResource__FPcT0(name, title));
    Texture64* t1 = static_cast<Texture64*>(lumpFindResource__FPcT0(name, "scrollmap.tex"));
    Texture64* t2 = static_cast<Texture64*>(lumpFindResource__FPcT0(name, "scrollmapb.tex"));
    Texture64* t3 = static_cast<Texture64*>(lumpFindResource__FPcT0(name, "blackout.tex"));
    Texture64* t4 = static_cast<Texture64*>(lumpFindResource__FPcT0(name, "foregrnd.tex"));
    Texture64* t5 = static_cast<Texture64*>(lumpFindResource__FPcT0(name, "foregrndb.tex"));
    drawSizeOverEnable = 0;
    engineDisplayDisable = 4;
    for (int pass = 0; pass < 3; ++pass) {
        engineFrameStart__FUib(0x80000000u, 1);
        textureDrawColor__FP8_textureiiiUi(t0, 0, 0, 6, 0x80606060u);
        textureDrawColor__FP8_textureiiiUi(t1, 0, 0, 6, 0x80606060u);
        textureDrawColor__FP8_textureiiiUi(t2, 0, 0, 6, 0x80606060u);
        textureDrawColor__FP8_textureiiiUi(t3, 0, 0, 6, 0x80606060u);
        textureDrawColor__FP8_textureiiiUi(t4, 0, 0, 6, 0x80606060u);
        textureDrawColor__FP8_textureiiiUi(t5, 0, 0, 6, 0x80606060u);
        titleloadTexCopy = *t0;
        scrollmapTexCopy[0] = *t1;
        scrollmapTexCopy[1] = *t2;
        blackoutTexCopy = *t3;
        foregrndTexCopy[0] = *t4;
        foregrndTexCopy[1] = *t5;
        engineFrameEnd__Fb(false);
    }
    drawListClear__Fv();
    drawSizeOverEnable = 1;
    lumpUnload__FPv(lump);
    if (loadThreadID == -1) {
        unsigned gp;
        __asm__ volatile("move %0, $gp" : "=r"(gp));
        loadThreadParam[1] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(&loadThreadMain__FPv));
        loadThreadParam[2] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(loadStack));
        loadThreadParam[3] = 8192;
        loadThreadParam[4] = gp;
        loadThreadParam[5] = 5;
        loadThreadID = CreateThread(loadThreadParam);
    }
    coreLoadingScreenRunning = 1;
    loadingScreenRequestExit = 0;
    StartThread(loadThreadID, 0);
}
