// Host check for texLoadAlwaysLoadedPages__Fv. Not an EE plan input. The
// header's table pointer is a host pointer; the page-location words store only
// the low 32 bits of the block address, so they are compared that way.
#include <cstdint>
#include <cstring>
struct _worldHeader;
struct _texture;
struct Page { int page; void* block; };
extern "C" {
_worldHeader* world;
int nmAlwaysPages;
Page alwaysLoadedPage[4];
signed char texturePageTable[64];
int texturePageLoc[16];
char engineCurrentLevelName[80] = "level_one";
void texLoadAlwaysLoadedPages__Fv();
}
static int calls[8], ncalls;
static unsigned char heap[2][4096];
static int allocs;
static char lastName[80];
static int lastCdSize, lastCdLoc;
static int initCount;
static int initTable[4];
static void* initBlocks[4];
extern "C" void* memAlloc__FiPc(int bytes, char* label) {
    if (std::strcmp(label, "always loaded page") != 0) return 0;
    calls[ncalls++] = bytes;
    unsigned char* p = heap[allocs++];
    int count = 2;
    std::memcpy(p, &count, 4);
    return p;
}
extern "C" int sprintf(char* d, const char* f, ...);
extern "C" void cdLoad__FPcPvii(char* name, void* dst, int size, int loc) {
    std::strcpy(lastName, name);
    (void)dst;
    lastCdSize = size;
    lastCdLoc = loc;
}
extern "C" void textureInitTexture__FP12_worldHeaderP8_texturePUcPc(_worldHeader*, _texture* t, unsigned char* table, char* block) {
    initTable[initCount] = table[0] == 0 && table[1] == 1 && table[200] == 200 && table[255] == 255;
    initBlocks[initCount] = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(t) - reinterpret_cast<unsigned char*>(block));
    ++initCount;
}
alignas(8) static unsigned char header[128];
alignas(8) static int records[16];
int main() {
    std::memset(texturePageTable, -1, sizeof texturePageTable);
    world = reinterpret_cast<_worldHeader*>(header);
    int base = 3;
    std::memcpy(header + 88, &base, 4);
    int* rp = records;
    std::memcpy(header + 100, &rp, sizeof rp);
    records[0] = 1000; records[1] = 3000;   // page 3: location 1000, size 3000
    records[2] = 2000; records[3] = 2048;   // page 4: size already a multiple
    alwaysLoadedPage[0].page = 4;
    alwaysLoadedPage[1].page = 3;
    nmAlwaysPages = 2;
    texLoadAlwaysLoadedPages__Fv();
    if (ncalls != 2 || calls[0] != 2048 || calls[1] != 4096) return 1;
    if (alwaysLoadedPage[0].block != heap[0] || alwaysLoadedPage[1].block != heap[1]) return 2;
    if (texturePageTable[4] != 9 || texturePageTable[3] != 10 || texturePageTable[5] != -1) return 3;
    if (texturePageLoc[9] != static_cast<int>(reinterpret_cast<std::uintptr_t>(heap[0])) ||
        texturePageLoc[10] != static_cast<int>(reinterpret_cast<std::uintptr_t>(heap[1]))) return 4;
    if (std::strcmp(lastName, "level_one.tex") != 0 || lastCdSize != 4096 || lastCdLoc != 1000) return 5;
    // Two records per block, each 64 bytes after the block's count word.
    if (initCount != 4 || !initTable[0] || !initTable[3]) return 6;
    if (reinterpret_cast<std::intptr_t>(initBlocks[0]) != 64 || reinterpret_cast<std::intptr_t>(initBlocks[1]) != 128) return 7;
    nmAlwaysPages = 0;
    initCount = 0;
    texLoadAlwaysLoadedPages__Fv();
    return initCount != 0 ? 8 : 0;
}
