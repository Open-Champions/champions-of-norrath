// Observed textureClear__Fv at 0x00170f88, 432 bytes.
// Resets the texture streaming state, in this order:
//  1. decodeQTail, decodeQHead := 0; the bytes texturesAreLoading and
//     texturesAreDecompressing := 0.
//  2. For each of the first nmAlwaysPages entries of the 8-byte records at
//     alwaysLoadedPage, the word at record offset 4 is freed with memFree when
//     nonzero and then cleared. nmAlwaysPages is read again after each free.
//  3. For each of the 1280 8-byte records of uncompMap, when the word at
//     record offset 0 is a nonzero pointer, the word at offset 20 of what it
//     points to is cleared; the record word is cleared either way.
//  4. uncompSpin := 0; all 40000 bytes of texturePageTable := 0xff; the 17
//     words of texturePageBlock := -1; the 17 128-byte rows of
//     texturePageSegLoc := 0xff; the 256 bytes of segmentUsed := 0.
//  5. decodeQTail := 0, decodeQHead := decodeQTail, texDecodeStarted := 0,
//     TerminateThread(decompressThreadid), StartThread(decompressThreadid, 0),
//     loadInProgress := 0, cacheClear, textureInternalInit.
//  6. textureUncompBufferLocked := 0.
struct NorrathPageRecord {
    int unobserved;
    void* pointer;
};

struct NorrathUncompRecord {
    int* target;
    int unobserved;
};

extern "C" int decodeQTail;
extern "C" int decodeQHead;
extern "C" unsigned char texturesAreLoading;
extern "C" unsigned char texturesAreDecompressing;
extern "C" int nmAlwaysPages;
extern "C" NorrathPageRecord alwaysLoadedPage[];
extern "C" NorrathUncompRecord uncompMap[1280];
extern "C" int uncompSpin;
extern "C" unsigned char texturePageTable[40000];
extern "C" int texturePageBlock[17];
extern "C" unsigned char texturePageSegLoc[17][128];
extern "C" unsigned char segmentUsed[256];
extern "C" int texDecodeStarted;
extern "C" int decompressThreadid;
extern "C" unsigned char loadInProgress;
extern "C" unsigned char textureUncompBufferLocked;

extern "C" void memFree__FPv(void* block);
extern "C" int TerminateThread(int thread);
extern "C" int StartThread(int thread, void* argument);
extern "C" void cacheClear__Fv();
extern "C" void textureInternalInit__Fv();

extern "C" void textureClear__Fv() {
    decodeQTail = 0;
    decodeQHead = decodeQTail;
    texturesAreLoading = 0;
    texturesAreDecompressing = 0;
    for (int i = 0; i < nmAlwaysPages; i++) {
        if (alwaysLoadedPage[i].pointer != 0) {
            memFree__FPv(alwaysLoadedPage[i].pointer);
            alwaysLoadedPage[i].pointer = 0;
        }
    }
    for (int i = 0; i < 1280; i++) {
        if (uncompMap[i].target != 0) uncompMap[i].target[5] = 0;
        uncompMap[i].target = 0;
    }
    uncompSpin = 0;
    for (int i = 0; i < 40000; i++) texturePageTable[i] = 0xff;
    for (int row = 0; row < 17; row++) {
        texturePageBlock[row] = -1;
        for (int i = 0; i < 128; i++) texturePageSegLoc[row][i] = 0xff;
    }
    for (int i = 0; i < 256; i++) segmentUsed[i] = 0;
    decodeQTail = 0;
    int thread = decompressThreadid;
    decodeQHead = decodeQTail;
    texDecodeStarted = 0;
    TerminateThread(thread);
    StartThread(decompressThreadid, 0);
    loadInProgress = 0;
    cacheClear__Fv();
    textureInternalInit__Fv();
    textureUncompBufferLocked = 0;
}
