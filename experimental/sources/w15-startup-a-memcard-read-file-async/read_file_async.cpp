// Observed: VIMemCardReadFileAsync__FPCcPvi at 0x010d3920 is 284 bytes and returns an int.
// Arguments (name, buffer, size). It stores zero in the word sAsyncMode (0x011e8fec), then
// does the same first-use setup as VIMemCardStatus__FPi and VIMemCardReadFile__FPCcPvi
// (sMemCardInitialized zero: sAsyncFd = -1, SetTitle on sIconDefinition with sProductTitle,
// SetIcon with sListIcon and 0x1cdd8, flag = 1). It formats "/%s/%s" with sProductId and the
// name into a 256-byte stack buffer, calls sceMcOpen(0, 0, path, 1) and a blocking
// sceMcSync(0, &command, &result). A result of -4 returns -2, any other negative result
// returns -1. Otherwise the result is stored in sAsyncFd, sceMcRead(result, buffer, size) is
// started and the function returns 0 without waiting.
// Inferred: sAsyncMode records that a read is in flight for other card code; sProductId is
// the per-product directory name; the non-negative open result is the file handle.
// Unknown: what -4 means to the SDK here, what reads sAsyncMode, and the contents of
// sProductId (it is a string constant in the image, not authored here).
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
int sprintf(char* buffer, const char* format, ...);
int sceMcOpen(int port, int slot, const char* path, int mode);
int sceMcSync(int mode, int* command, int* result);
int sceMcRead(int fd, void* buffer, int size);

int VIMemCardReadFileAsync__FPCcPvi(const char* name, void* buffer, int size) {
    sAsyncMode = 0;
    if (sMemCardInitialized == 0) {
        sAsyncFd = -1;
        SetTitle__16VIIconDefinitionPCc(sIconDefinition, sProductTitle);
        SetIcon__16VIIconDefinitionPvi(sIconDefinition, sListIcon, 0x1cdd8);
        sMemCardInitialized = 1;
    }
    char path[256];
    sprintf(path, "/%s/%s", sProductId, name);
    sceMcOpen(0, 0, path, 1);
    int words[2];  // command, result
    sceMcSync(0, &words[0], &words[1]);
    const int opened = words[1];
    if (opened == -4) return -2;
    if (opened < 0) return -1;
    sAsyncFd = opened;
    sceMcRead(opened, buffer, size);
    return 0;
}
}
