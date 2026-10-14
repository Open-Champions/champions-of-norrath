// Observed: VIMemCardPadAsync__Fi at 0x010d33a0 is 632 bytes and returns an int.
// The argument is a count. It stores 2 in sAsyncMode, takes an 1152-byte block
// from __wrap_memalign(64, 1152) and does the first-use setup shared with
// VIMemCardReadFileAsync (sMemCardInitialized zero: sAsyncFd = -1, SetTitle
// on sIconDefinition with sProductTitle, SetIcon with sListIcon and 0x1cdd8,
// flag = 1). It formats "/%s/*" with sProductId, calls
// sceMcGetDir(0, 0, path, 0, 18, block) and a blocking sceMcSync; the GetDir
// return is stored in the result word first. A negative result frees the block
// and returns -1. Otherwise the result is an entry count n over 64-byte table
// entries; the word at entry offset 16 of each is rounded up to 1024 and
// divided by 1024, summed, then n/2 (signed) and 2 are added, and that sum is
// subtracted from the argument. The difference shifted left 10 is the size of
// a buffer taken from __builtin_vec_new and cleared bytewise when positive.
// "/%s/%s" with sProductId and "pad.dat" is opened with sceMcOpen(0, 0, path,
// 514) and a blocking sceMcSync. Result -3: free the block, delete the buffer
// when non-null, return -2. Another negative result: same cleanup, return -1.
// Otherwise the result goes to sAsyncFd, sceMcWrite(result, buffer, size) is
// started, the block is freed, the buffer deleted when non-null, and 0 is
// returned without waiting.
// Inferred: this reserves the rest of a save-card allocation by writing a
// padding file; the argument is a total in 1 KiB units and the entry word at
// offset 16 is a file size in bytes.
// Unknown: the unit of the argument, why n/2 + 2 is added, and what -3 means
// to the SDK here. The buffer is not checked for null before the clear or write.
extern "C" {
extern int sMemCardInitialized;
extern int sAsyncFd;
extern int sAsyncMode;
extern unsigned char sIconDefinition[];
extern char sProductTitle[];
extern char sProductId[];
extern unsigned char sListIcon[];
void SetTitle__16VIIconDefinitionPCc(void* definition, const char* title);
void SetIcon__16VIIconDefinitionPvi(void* definition, void* icon, int size);
void* __wrap_memalign(unsigned alignment, unsigned size);
void __wrap_free(void* pointer);
void* __builtin_vec_new(unsigned size);
void __builtin_vec_delete(void* pointer);
int sprintf(char* buffer, const char* format, ...);
int sceMcGetDir(int port, int slot, const char* name, unsigned mode, int max_entries, void* table);
int sceMcOpen(int port, int slot, const char* path, int mode);
int sceMcWrite(int fd, const void* buffer, int size);
int sceMcSync(int mode, int* command, int* result);

int VIMemCardPadAsync__Fi(int total) {
    sAsyncMode = 2;
    unsigned char* table = static_cast<unsigned char*>(__wrap_memalign(64, 1152));
    if (sMemCardInitialized == 0) {
        sAsyncFd = -1;
        SetTitle__16VIIconDefinitionPCc(sIconDefinition, sProductTitle);
        SetIcon__16VIIconDefinitionPvi(sIconDefinition, sListIcon, 0x1cdd8);
        sMemCardInitialized = 1;
    }
    char path[256];
    sprintf(path, "/%s/*", sProductId);
    int words[2];  // command, result
    words[1] = sceMcGetDir(0, 0, path, 0, 18, table);
    sceMcSync(0, &words[0], &words[1]);
    int count = words[1];
    if (count < 0) {
        __wrap_free(table);
        return -1;
    }
    int used = 0;
    for (int i = 0; i < count; ++i)
        used += static_cast<int>((*reinterpret_cast<unsigned*>(table + 16 + 64 * i) + 1023) >> 10);
    used += count / 2 + 2;
    int size = (total - used) << 10;
    char* buffer = static_cast<char*>(__builtin_vec_new(size));
    for (int i = 0; i < size; ++i) buffer[i] = 0;
    sprintf(path, "/%s/%s", sProductId, "pad.dat");
    sceMcOpen(0, 0, path, 514);
    sceMcSync(0, &words[0], &words[1]);
    int opened = words[1];
    if (opened == -3) {
        __wrap_free(table);
        if (buffer != 0) __builtin_vec_delete(buffer);
        return -2;
    }
    if (opened < 0) {
        __wrap_free(table);
        if (buffer != 0) __builtin_vec_delete(buffer);
        return -1;
    }
    sAsyncFd = opened;
    sceMcWrite(opened, buffer, size);
    __wrap_free(table);
    if (buffer != 0) __builtin_vec_delete(buffer);
    return 0;
}
}
