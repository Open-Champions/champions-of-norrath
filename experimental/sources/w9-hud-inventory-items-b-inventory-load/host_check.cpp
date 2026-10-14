#include <cstdlib>
#include <cstring>
#include <cstdint>
extern "C" {
int inventoryLoadState;
void* inventoryLump;
unsigned char* invGrid;
unsigned char* shopGrid;
unsigned char inventory[464];
int inventoryLoad__Fv();
}

static int log_[32], n;
static unsigned char* buf;
static int busy;
static char* doneFlag;
static void* addBuf;
static int initCalls;

extern "C" void _shopCheck__Fi(int v) { log_[n++] = v; }
extern "C" void* textureLockCacheBuffer__Fv() { log_[n++] = 1; return buf; }
extern "C" int cdIsBusy__Fv() { log_[n++] = 2; return busy; }
extern "C" void cdLoadAsync__FPcPvT0ii(char* name, void* b, void* done, int x, int y) {
    log_[n++] = 3;
    if (std::strcmp(name, "inventry.lmp") || b != buf || x != 0 || y != 0) log_[n++] = -1;
    if (inventoryLoadState != 1) log_[n++] = -2;   // state store happens before the call
    doneFlag = (char*)done;
}
extern "C" void shopUnpackToGrid__Fiii(int a, int b, int c) {
    log_[n++] = 4; log_[n++] = a; log_[n++] = b; log_[n++] = c;
    // the grid is zeroed before this call
    if (invGrid[0] || invGrid[145800 * 3 + 48600 * 2 + 5400 * 8 + 72 * 74]) log_[n++] = -3;
}
extern "C" void lumpAdd__FPvPCc(void* b, const char* name) { log_[n++] = 5; addBuf = b; if (std::strcmp(name, "inventry.lmp")) log_[n++] = -4; }
extern "C" void invInitSharedInterfaceTextures__Fv() { log_[n++] = 6; ++initCalls;
    if (inventoryLoadState != 2 || inventoryLump != buf) log_[n++] = -5; }

int main() {
    const size_t size = 0x400000 + 16;
    buf = (unsigned char*)std::malloc(size);
    // dirty everything so the zeroing is observable
    std::memset(buf, 0xaa, size);
    *(int*)(inventory + 52) = 77;

    // state 0, buffer available, not busy: starts the load
    n = 0; busy = 0; inventoryLoadState = 0;
    if (inventoryLoad__Fv() != 0) return 1;
    int want0[] = {154, 1, 2, 3, 4, 77, 0, 0, 203};
    if (n != 9) return 3;
    for (int i = 0; i < 9; ++i) if (log_[i] != want0[i]) return 4;
    if (inventoryLoadState != 1 || invGrid != buf + 0x300000 || shopGrid != buf + 0x400000) return 5;
    // every record's first word is zero; nothing else in the table is touched
    int zeroed = 0, bad = 0;
    for (int g = 0; g < 4; ++g) for (int k = 0; k < 3; ++k) for (int c = 0; c < 9; ++c) for (int r = 0; r < 75; ++r) {
        unsigned char* p = invGrid + g * 145800 + k * 48600 + c * 5400 + r * 72;
        if (*(int*)p == 0) ++zeroed;
        if (p[4] != 0xaa || p[71] != 0xaa) ++bad;
    }
    if (zeroed != 4 * 3 * 9 * 75 || bad != 0) return 6;
    if (buf[0x300000 + 145800 * 4] != 0xaa) return 7;

    // state 1, not finished: nothing, closing check, 0
    n = 0; *doneFlag = 0;
    if (inventoryLoad__Fv() != 0 || n != 2 || log_[0] != 154 || log_[1] != 203) return 8;
    // state 1, finished: add lump, state 2, lump set, textures, return 1
    n = 0; *doneFlag = 1;
    if (inventoryLoad__Fv() != 1) return 9;
    int want1[] = {154, 1, 5, 6};
    if (n != 4) return 10;
    for (int i = 0; i < 4; ++i) if (log_[i] != want1[i]) return 11;
    if (inventoryLoadState != 2 || inventoryLump != buf || addBuf != buf) return 12;
    // state 2: 1, no closing check
    n = 0;
    if (inventoryLoad__Fv() != 1 || n != 1 || log_[0] != 154) return 13;

    // state 0 but the buffer is unavailable: no busy query, 0
    n = 0; inventoryLoadState = 0; void* saved = buf; buf = nullptr;
    if (inventoryLoad__Fv() != 0 || n != 3 || log_[1] != 1 || log_[2] != 203 || inventoryLoadState != 0) return 14;
    // state 0, busy: 0 and state stays
    buf = (unsigned char*)saved; n = 0; busy = 1;
    if (inventoryLoad__Fv() != 0 || n != 4 || log_[2] != 2 || log_[3] != 203 || inventoryLoadState != 0) return 15;
    // unknown state: closing check only
    n = 0; inventoryLoadState = 3;
    if (inventoryLoad__Fv() != 0 || n != 2 || inventoryLoadState != 3) return 16;
    return 0;
}
