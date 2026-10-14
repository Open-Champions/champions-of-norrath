// Observed cdProcess__Fv at 0x00165238, 436 bytes.
//
// While the cdBusy byte is 1, a nonzero streaming byte reads
// sceCdStRead when streamSizeLeft is signed-greater than zero. The block
// count is that word arithmetic-shifted by 11. The destination is
// streamOutput, the mode is 0, and the error word is a cleared stack slot
// that is not read afterward. streamSizeLeft decreases by the returned block
// count shifted left 11, and streamOutput advances by the same amount. A
// zero remainder stores 1 through the pointer in cdResult and clears cdBusy.
//
// A clear streaming byte calls sceCdSync(1). A zero result and a zero
// sceCdGetError complete the same way. A nonzero error calls issueCommand
// and leaves cdBusy set. A nonzero sync result does neither.
//
// The lump tail runs on every path. A zero lumpLoadLump word returns. A zero
// lumpLoadCompletionFlag byte returns. Otherwise soundGetFreeRam, initLump,
// and memRealloc run. The new size is lumpLoadSize minus the initLump result
// plus 256. The 40-byte record at lumps + nmLumps * 40 stores that size at
// +24, the difference of the two soundGetFreeRam results at +28, zero at +20
// and +32, then strcpy from lumpLoadName. nmLumps increases by one,
// lumpLoadLump and the completion flag are cleared, +16 receives the lump
// pointer, and +36 receives 1.
extern "C" {
extern unsigned char cdBusy;
extern unsigned char streaming;
extern int streamSizeLeft;
extern unsigned char* streamOutput;
extern unsigned cdResult;
extern unsigned char lumpLoadLump[4];
extern unsigned char lumpLoadCompletionFlag;
extern unsigned char lumpLoadSize[4];
extern unsigned char lumpLoadName[];
extern unsigned char lumps[];
extern unsigned nmLumps;

int sceCdSync(int mode);
int sceCdGetError();
int sceCdStRead(int blocks, void* destination, int mode, int* error);
void issueCommand__Fv();
int soundGetFreeRam__Fv();
int initLump__FPv(void* lump);
void* memRealloc__FPvi(void* lump, int size);
char* strcpy(char* destination, const char* source);

static unsigned load_word(const unsigned char* address) {
    return address[0] | (address[1] << 8) | (address[2] << 16) | (address[3] << 24);
}
static void store_word(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}
#if defined(NORRATH_HOST_CONTRACT)
void norrath_host_store_byte(unsigned address, unsigned char value);
#endif
static void store_result_byte() {
#if defined(NORRATH_HOST_CONTRACT)
    norrath_host_store_byte(cdResult, 1);
#else
    *reinterpret_cast<volatile unsigned char*>(static_cast<unsigned long>(cdResult)) = 1;
#endif
}
static void complete() {
    store_result_byte();
    cdBusy = 0;
}

void cdProcess__Fv() {
    if (cdBusy == 1) {
        if (streaming != 0) {
            const int left = streamSizeLeft;
            if (left > 0) {
                int error = 0;
                const int blocks = sceCdStRead(left >> 11, streamOutput, 0, &error);
                const int advanced = blocks << 11;
                streamSizeLeft = left - advanced;
                streamOutput += advanced;
                if (streamSizeLeft == 0) complete();
            }
        } else if (sceCdSync(1) == 0) {
            if (sceCdGetError() == 0) complete();
            else issueCommand__Fv();
        }
    }

    const unsigned lump = load_word(lumpLoadLump);
    if (lump == 0u || lumpLoadCompletionFlag == 0) return;
    const int before = soundGetFreeRam__Fv();
    const int consumed = initLump__FPv(reinterpret_cast<void*>(static_cast<unsigned long>(lump)));
    const int resized = static_cast<int>(load_word(lumpLoadSize)) - consumed + 256;
    memRealloc__FPvi(reinterpret_cast<void*>(static_cast<unsigned long>(lump)), resized);
    unsigned char* const record = lumps + nmLumps * 40u;
    store_word(record + 24, static_cast<unsigned>(resized));
    const int after = soundGetFreeRam__Fv();
    store_word(record + 28, static_cast<unsigned>(before - after));
    store_word(record + 20, 0u);
    store_word(record + 32, 0u);
    strcpy(reinterpret_cast<char*>(record), reinterpret_cast<const char*>(lumpLoadName));
    const unsigned count = nmLumps;
    nmLumps = count + 1u;
    store_word(lumpLoadLump, 0u);
    lumpLoadCompletionFlag = 0;
    store_word(record + 16, lump);
    store_word(record + 36, 1u);
}
}
