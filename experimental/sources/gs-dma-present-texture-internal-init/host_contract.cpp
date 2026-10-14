#define NORRATH_HOST_CONTRACT 1
#include "texture_internal_init.cpp"

#include <cstdio>
#include <cstring>

extern "C" unsigned char cache[192] = {};
extern "C" unsigned char dummyTex[4] = {};
extern "C" unsigned char uncompMap[10240] = {};
extern "C" unsigned char texturePageTable[40000] = {};
extern "C" unsigned char texturePageSegLoc[2176] = {};
extern "C" unsigned char segmentUsed[256] = {};
extern "C" unsigned char texturePageBlock[128] = {};
extern "C" unsigned char texturePageLoc[68] = {};
extern "C" unsigned char texdecompress[48] = {};
extern "C" unsigned char decompressStack[8] = {};
extern "C" int cacheHead = 0;
extern "C" int cacheTail = 0;
extern "C" int uncompSpin = 0;
extern "C" int decompressThreadid = 0;
extern "C" unsigned norrath_host_gp_value = 0;

static int unscramble_calls;
static int create_calls;
static int start_calls;
static int create_result;
static void* started_argument;

extern "C" void texDecompressThread__FPv(void*) {}
extern "C" void makeUnscrambleTable__Fv() { unscramble_calls += 1; }
extern "C" void* norrath_host_memset(void* destination, int value, unsigned count) {
    return std::memset(destination, value, count);
}
extern "C" int CreateThread(int* words) {
    create_calls += 1;
    if (words != reinterpret_cast<int*>(texdecompress)) create_calls = -100;
    return create_result;
}
extern "C" int StartThread(int identifier, void* argument) {
    start_calls += 1;
    if (identifier != create_result) start_calls = -100;
    started_argument = argument;
    return 0;
}

static unsigned word_at(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) |
           (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) |
           (static_cast<unsigned>(address[3]) << 24);
}

int main() {
    std::memset(cache, 0xa5, sizeof cache);
    std::memset(uncompMap, 0xa5, sizeof uncompMap);
    std::memset(texturePageTable, 0xa5, sizeof texturePageTable);
    std::memset(texturePageSegLoc, 0xa5, sizeof texturePageSegLoc);
    std::memset(texturePageBlock, 0xa5, sizeof texturePageBlock);
    std::memset(segmentUsed, 0xa5, sizeof segmentUsed);
    std::memset(texturePageLoc, 0xa5, sizeof texturePageLoc);
    std::memset(texdecompress, 0xa5, sizeof texdecompress);
    norrath_host_gp_value = 0x004879f0u;
    decompressThreadid = 0;
    cacheHead = 7;
    cacheTail = 8;
    uncompSpin = 9;
    create_result = 42;
    unscramble_calls = 0;
    create_calls = 0;
    start_calls = 0;
    textureInternalInit__Fv();
    if (cacheHead != 0 || cacheTail != 0 || uncompSpin != 0) return 1;
    const unsigned dummy = static_cast<unsigned>(reinterpret_cast<unsigned long>(dummyTex));
    if (word_at(cache) != dummy || cache[4] != 0 || cache[10] != 0xa5) return 2;
    if (word_at(cache + 15 * 12) != dummy || cache[15 * 12 + 9] != 0 || cache[15 * 12 + 10] != 0xa5) return 3;
    if (word_at(uncompMap) != 0u || uncompMap[4] != 0xa5) return 4;
    if (word_at(uncompMap + 10232) != 0u || uncompMap[10236] != 0xa5) return 5;
    if (texturePageTable[0] != 0xff || texturePageTable[39999] != 0xff) return 6;
    if (texturePageSegLoc[0] != 0xff || texturePageSegLoc[2175] != 0xff) return 7;
    if (word_at(texturePageBlock) != 0xffffffffu || word_at(texturePageBlock + 64) != 0xffffffffu) return 8;
    if (texturePageBlock[68] != 0xa5) return 9;
    if (segmentUsed[0] != 0 || segmentUsed[255] != 0) return 10;
    if (texturePageLoc[0] != 0 || texturePageLoc[67] != 0) return 11;
    if (unscramble_calls != 1 || create_calls != 1 || start_calls != 1) return 12;
    if (started_argument != nullptr || decompressThreadid != 42) return 13;
    if (texdecompress[0] != 0xa5) return 14;
    if (word_at(texdecompress + 12) != 16384u || word_at(texdecompress + 16) != 0x004879f0u) return 15;
    if (word_at(texdecompress + 20) != 6u) return 16;
    if (word_at(texdecompress + 4) != static_cast<unsigned>(reinterpret_cast<unsigned long>(&texDecompressThread__FPv))) return 17;

    decompressThreadid = 3;
    const int creates = create_calls;
    textureInternalInit__Fv();
    if (create_calls != creates || decompressThreadid != 3 || unscramble_calls != 2) return 18;
    std::puts("texture internal init contract passed");
    return 0;
}
