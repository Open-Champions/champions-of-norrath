// Host contract for worldFreeWorld__FP12_worldHeader. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct _worldHeader;
struct _texture;
signed char texturePageTable[64];
int texturePageLoc[4];
void* mt_dyndat;
void* mt_throwaway;
static int log_[32], n;
extern "C" void textureFreeTexture__FP8_texture(_texture* t) { log_[n++] = 1000 + int(reinterpret_cast<long>(t)); }
extern "C" void memFree__FPv(void* p) { log_[n++] = 2000 + int(reinterpret_cast<long>(p)); }
extern "C" void texClearAlwaysLoadedPages__Fv() { log_[n++] = 3000; }
extern "C" void worldFreeWorld__FP12_worldHeader(_worldHeader*);
alignas(8) static unsigned char world[160];
alignas(8) static unsigned char recs[3 * 60];
alignas(8) static unsigned char block[32];
alignas(8) static unsigned char linked[4800];
int main() {
    std::memset(texturePageTable, -1, sizeof texturePageTable);
    texturePageTable[5] = 2;
    texturePageLoc[2] = 100;
    *reinterpret_cast<int*>(world) = 3;
    *reinterpret_cast<unsigned char**>(world + 36) = recs;
    // record 0: texture 5 at offset 7 -> free 107; record 1: texture 9 maps to -1; record 2: bit 7 link.
    *reinterpret_cast<short*>(recs + 52) = 5;
    *reinterpret_cast<int*>(recs + 32) = 7;
    *reinterpret_cast<short*>(recs + 60 + 52) = 9;
    *reinterpret_cast<int*>(recs + 120 + 36) = 0x80;
    *reinterpret_cast<unsigned char**>(recs + 120) = block;
    *reinterpret_cast<unsigned char**>(block + 8) = linked;
    *reinterpret_cast<int*>(linked + 4768) = 77;
    *reinterpret_cast<long*>(world + 108) = 0;
    *reinterpret_cast<long*>(world + 124) = 0;
    mt_dyndat = reinterpret_cast<void*>(1);
    worldFreeWorld__FP12_worldHeader(reinterpret_cast<_worldHeader*>(world));
    if (n != 3 || log_[0] != 1107 || log_[1] != 2001 || log_[2] != 3000) return 1;
    // Host pointers are wider than the target's, so only the low word of block+8 is compared.
    if (*reinterpret_cast<int*>(linked + 4768) != 0 || *reinterpret_cast<int*>(block + 8) != 0) return 2;
    if (mt_dyndat != 0 || mt_throwaway != 0) return 3;
    std::printf("world free world contract passed\n");
    return 0;
}
