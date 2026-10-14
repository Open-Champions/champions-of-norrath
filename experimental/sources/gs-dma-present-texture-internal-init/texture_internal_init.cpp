/* Observed textureInternalInit__Fv at 0x0016d520, 440 bytes. It clears
 * cacheHead and cacheTail, points 16 cache records at dummyTex, clears
 * every other word of uncompMap, clears uncompSpin, fills texturePageTable
 * and texturePageSegLoc with 0xff, writes 17 words of -1 at texturePageBlock,
 * clears segmentUsed, clears texturePageLoc, and calls
 * makeUnscrambleTable__Fv. A zero decompressThreadid builds the
 * texdecompress words and calls CreateThread and StartThread. Fresh
 * compilation is not an original-byte match. The thread service effects
 * are unknown.
 */
extern "C" unsigned char cache[];
extern "C" unsigned char dummyTex[];
extern "C" unsigned char uncompMap[];
extern "C" unsigned char texturePageTable[];
extern "C" unsigned char texturePageSegLoc[];
extern "C" unsigned char segmentUsed[];
extern "C" unsigned char texturePageBlock[];
extern "C" unsigned char texturePageLoc[];
extern "C" unsigned char texdecompress[];
extern "C" unsigned char decompressStack[];
extern "C" int cacheHead;
extern "C" int cacheTail;
extern "C" int uncompSpin;
extern "C" int decompressThreadid;
extern "C" void texDecompressThread__FPv(void* argument);
extern "C" void makeUnscrambleTable__Fv();
extern "C" int CreateThread(int* words);
extern "C" int StartThread(int identifier, void* argument);

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_gp_value;
extern "C" void* norrath_host_memset(void* destination, int value, unsigned count);
#define NORRATH_GP (norrath_host_gp_value)
#define NORRATH_MEMSET norrath_host_memset
#else
static inline unsigned norrath_gp_value() {
    unsigned value;
    asm volatile("move %0, $28" : "=r"(value));
    return value;
}
extern "C" void* memset(void* destination, int value, unsigned count);
#define NORRATH_GP (norrath_gp_value())
#define NORRATH_MEMSET memset
#endif

static void store_word(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}

extern "C" void textureInternalInit__Fv() {
    cacheTail = 0;
    cacheHead = 0;
    const unsigned dummy = static_cast<unsigned>(reinterpret_cast<unsigned long>(dummyTex));
    for (int index = 0; index < 16; ++index) {
        unsigned char* record = cache + index * 12;
        store_word(record, dummy);
        record[4] = 0;
        record[5] = 0;
        record[6] = 0;
        record[7] = 0;
        record[8] = 0;
        record[9] = 0;
    }
    unsigned char* cursor = uncompMap + 10232;
    for (int remaining = 1279; ; cursor -= 8) {
        remaining -= 1;
        store_word(cursor, 0u);
        if (remaining < 0) break;
    }
    uncompSpin = 0;
    for (int index = 0; index < 40000; ++index)
        texturePageTable[index] = 0xff;
    for (int page = 0; page < 17; ++page) {
        store_word(texturePageBlock + page * 4, 0xffffffffu);
        unsigned char* row = texturePageSegLoc + page * 128;
        for (int byte = 0; byte < 128; ++byte)
            row[byte] = 0xff;
    }
    for (int index = 0; index < 256; ++index)
        segmentUsed[index] = 0;
    NORRATH_MEMSET(texturePageLoc, 0, 68u);
    makeUnscrambleTable__Fv();
    if (decompressThreadid != 0) return;
    store_word(texdecompress + 4, static_cast<unsigned>(reinterpret_cast<unsigned long>(
        &texDecompressThread__FPv)));
    store_word(texdecompress + 8, static_cast<unsigned>(reinterpret_cast<unsigned long>(decompressStack)));
    store_word(texdecompress + 12, 16384u);
    store_word(texdecompress + 16, NORRATH_GP);
    store_word(texdecompress + 20, 6u);
    const int thread = CreateThread(reinterpret_cast<int*>(texdecompress));
    decompressThreadid = thread;
    StartThread(thread, nullptr);
}
