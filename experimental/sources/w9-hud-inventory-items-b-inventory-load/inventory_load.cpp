// Observed inventoryLoad__Fv at 0x001ac510, 440 bytes.
// A step function for loading "inventry.lmp" (string at 0x4662f0). Returns 1 when
// the lump is loaded and 0 when it is not (yet). It begins with
// _shopCheck__Fi(154) and, on every path that returns 0, ends with
// _shopCheck__Fi(203); the already-loaded path returns 1 without the second call.
// What _shopCheck__Fi does is not established. State is the word
// inventoryLoadState (0x00484824):
//   0 (idle): textureLockCacheBuffer__Fv returns a buffer; if it is null, or
//     cdIsBusy__Fv is nonzero, return 0. Otherwise the state becomes 1 and
//     cdLoadAsync__FPcPvT0ii(name, buffer, &loadFinished, 0, 0) starts the read.
//     (The state store sits in the call's delay slot and executes first.)
//     invGrid is set to buffer + 0x300000. The table there is 4 x 3 x 9 x 75
//     records of 72 bytes (row r, column c, list k, group g at
//     g*145800 + k*48600 + c*5400 + r*72, as in removeItemFromInventory); the
//     first word of each record is zeroed. shopUnpackToGrid__Fiii is called with
//     the word at offset 52 of the first `inventory` record, 0, 0; then shopGrid
//     is set to buffer + 0x400000. Return 0.
//   1 (loading): if the byte loadFinished (a file-local static) is zero, return 0.
//     Otherwise take the buffer from textureLockCacheBuffer__Fv again; if it is
//     null a zero word is stored through the null pointer (the original does
//     this; it is a write to address 0 and is kept, not tested). Then
//     lumpAdd__FPvPCc(buffer, name), state = 2, inventoryLump = buffer,
//     invInitSharedInterfaceTextures__Fv(). Return 1.
//   2 (loaded): return 1.
//   any other value: return 0 after the closing _shopCheck__Fi(203).
// Names of the load states and the meaning of the two table-base offsets beyond
// what the arithmetic shows are not established. The callees and objects are not
// defined here.
extern "C" {
extern int inventoryLoadState;
extern void* inventoryLump;
extern unsigned char* invGrid;
extern unsigned char* shopGrid;
extern unsigned char inventory[];
void _shopCheck__Fi(int);
void* textureLockCacheBuffer__Fv();
int cdIsBusy__Fv();
void cdLoadAsync__FPcPvT0ii(char* name, void* buffer, void* done, int a, int b);
void shopUnpackToGrid__Fiii(int, int, int);
void lumpAdd__FPvPCc(void* buffer, const char* name);
void invInitSharedInterfaceTextures__Fv();
}

static char loadFinished;

extern "C" int inventoryLoad__Fv() {
    _shopCheck__Fi(154);
    if (inventoryLoadState == 1) {
        if (loadFinished != 0) {
            void* buffer = textureLockCacheBuffer__Fv();
            if (buffer == nullptr) *static_cast<volatile int*>(buffer) = 0;
            lumpAdd__FPvPCc(buffer, "inventry.lmp");
            inventoryLoadState = 2;
            inventoryLump = buffer;
            invInitSharedInterfaceTextures__Fv();
            return 1;
        }
    } else if (inventoryLoadState == 2) {
        return 1;
    } else if (inventoryLoadState == 0) {
        void* buffer = textureLockCacheBuffer__Fv();
        if (buffer != nullptr && cdIsBusy__Fv() == 0) {
            inventoryLoadState = 1;
            cdLoadAsync__FPcPvT0ii(const_cast<char*>("inventry.lmp"), buffer, &loadFinished, 0, 0);
            unsigned char* grid = static_cast<unsigned char*>(buffer) + 0x300000;
            invGrid = grid;
            for (int g = 0; g < 4; ++g)
                for (int r = 0; r < 75; ++r)
                    for (int c = 0; c < 9; ++c)
                        for (int k = 0; k < 3; ++k)
                            *reinterpret_cast<int*>(grid + g * 145800 + k * 48600 + c * 5400 + r * 72) = 0;
            shopUnpackToGrid__Fiii(*reinterpret_cast<int*>(inventory + 52), 0, 0);
            shopGrid = static_cast<unsigned char*>(buffer) + 0x400000;
        }
    }
    _shopCheck__Fi(203);
    return 0;
}
