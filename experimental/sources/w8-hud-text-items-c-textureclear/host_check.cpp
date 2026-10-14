#include <cstring>
#include <string>
#include <vector>
struct PageRecord { int unobserved; void* pointer; };
struct UncompRecord { int* target; int unobserved; };
int decodeQTail, decodeQHead, nmAlwaysPages, uncompSpin, texPageBlockPlaceholder, texDecodeStarted, decompressThreadid;
unsigned char texturesAreLoading, texturesAreDecompressing, loadInProgress, textureUncompBufferLocked;
PageRecord alwaysLoadedPage[8];
UncompRecord uncompMap[1280];
unsigned char texturePageTable[40000];
int texturePageBlock[17];
unsigned char texturePageSegLoc[17][128];
unsigned char segmentUsed[256];
static std::vector<std::string> calls;
static void* freed[8]; static int freed_count;
extern "C" void memFree__FPv(void* p) { calls.push_back("free"); freed[freed_count++] = p; if (nmAlwaysPages == 3) nmAlwaysPages = 2; }
extern "C" int TerminateThread(int t) { calls.push_back("term" + std::to_string(t)); return 0; }
extern "C" int StartThread(int t, void* a) { calls.push_back("start" + std::to_string(t) + (a ? "x" : "")); return 0; }
extern "C" void cacheClear__Fv() { calls.push_back("cache"); if (textureUncompBufferLocked != 1) calls.push_back("BAD-lock-cleared-early"); }
extern "C" void textureInternalInit__Fv() { calls.push_back("init"); }
extern "C" void textureClear__Fv();

int main() {
    int block[8], other[8];
    memset(block, 0x11, sizeof block); memset(other, 0x22, sizeof other);
    decodeQTail = 5; decodeQHead = 6; texturesAreLoading = texturesAreDecompressing = 1;
    nmAlwaysPages = 3;
    alwaysLoadedPage[0].pointer = &block; alwaysLoadedPage[1].pointer = 0; alwaysLoadedPage[2].pointer = &other;
    alwaysLoadedPage[3].pointer = &other;
    uncompMap[2].target = block; uncompMap[9].target = other; uncompMap[9].unobserved = 77;
    uncompSpin = 9;
    memset(texturePageTable, 0, sizeof texturePageTable);
    memset(texturePageBlock, 0, sizeof texturePageBlock);
    memset(texturePageSegLoc, 0, sizeof texturePageSegLoc);
    memset(segmentUsed, 0x33, sizeof segmentUsed);
    texDecodeStarted = 4; decompressThreadid = 42; loadInProgress = 1; textureUncompBufferLocked = 1;
    textureClear__Fv();
    if (decodeQTail || decodeQHead || texturesAreLoading || texturesAreDecompressing) return 1;
    // The count shrinks after the first free so entry 2 is never visited.
    if (freed_count != 1 || freed[0] != &block || alwaysLoadedPage[0].pointer || alwaysLoadedPage[2].pointer != &other) return 2;
    if (block[5] != 0 || other[5] != 0 || block[4] != 0x11111111 || other[6] != 0x22222222) return 3;
    if (uncompMap[2].target || uncompMap[9].target) return 4;
    if (uncompSpin) return 5;
    for (unsigned char b : texturePageTable) if (b != 0xff) return 6;
    for (int w : texturePageBlock) if (w != -1) return 7;
    for (auto& row : texturePageSegLoc) for (unsigned char b : row) if (b != 0xff) return 8;
    for (unsigned char b : segmentUsed) if (b) return 9;
    if (texDecodeStarted || loadInProgress || textureUncompBufferLocked) return 10;
    std::vector<std::string> want = {"free", "term42", "start42", "cache", "init"};
    if (calls != want) return 11;
    return 0;
}
