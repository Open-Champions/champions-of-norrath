// Observed: VIMemCardWriteFile__FPCcPCvi at 0x010d3c98 is 284 bytes and returns an int.
// Same first-use setup as VIMemCardReadFile__FPCcPvi (sMemCardInitialized zero:
// sAsyncFd = -1, SetTitle on sIconDefinition with sProductTitle, SetIcon with
// sListIcon and 0x1cdd8, flag = 1). It then calls
// VIMemCardWriteFileAsync__FPCcPCvi(name, buffer, size). A negative return
// is returned unchanged. Otherwise that value is stored in the word that
// sceMcSync(0, &command, &result) receives as result, sceMcFlush(sAsyncFd)
// runs first, and then: result == -3 returns -2; another negative result
// returns -1; else sceMcClose(sAsyncFd) and a second blocking sceMcSync follow,
// returning -1 if the second result is negative and 0 otherwise.
// Inferred: a successful write returns 0 rather than the byte count.
// Unknown: what -3 means to the SDK here, and the async function's own behavior.
extern "C" {
extern int sMemCardInitialized;
extern int sAsyncFd;
extern unsigned char sIconDefinition[];
extern char sProductTitle[];
extern unsigned char sListIcon[];
void SetTitle__16VIIconDefinitionPCc(void* definition, const char* title);
void SetIcon__16VIIconDefinitionPvi(void* definition, void* icon, int size);
int VIMemCardWriteFileAsync__FPCcPCvi(const char* name, const void* buffer, int size);
int sceMcSync(int mode, int* command, int* result);
int sceMcFlush(int fd);
int sceMcClose(int fd);

int VIMemCardWriteFile__FPCcPCvi(const char* name, const void* buffer, int size) {
    if (sMemCardInitialized == 0) {
        sAsyncFd = -1;
        SetTitle__16VIIconDefinitionPCc(sIconDefinition, sProductTitle);
        SetIcon__16VIIconDefinitionPvi(sIconDefinition, sListIcon, 0x1cdd8);
        sMemCardInitialized = 1;
    }
    int started = VIMemCardWriteFileAsync__FPCcPCvi(name, buffer, size);
    int words[2];  // command, result
    words[1] = started;
    if (started < 0) return started;
    sceMcFlush(sAsyncFd);
    sceMcSync(0, &words[0], &words[1]);
    int got = words[1];
    if (got == -3) return -2;
    if (got < 0) return -1;
    sceMcClose(sAsyncFd);
    sceMcSync(0, &words[0], &words[1]);
    return words[1] < 0 ? -1 : 0;
}
}
