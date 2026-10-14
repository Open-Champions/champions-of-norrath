// Observed: VIMemCardReadFile__FPCcPvi at 0x010d3a40 is 276 bytes and returns an int.
// Arguments (name, buffer, size) are forwarded to VIMemCardReadFileAsync__FPCcPvi
// after the same first-use setup as VIMemCardStatus__FPi (sMemCardInitialized
// zero: sAsyncFd = -1, SetTitle on sIconDefinition with sProductTitle, SetIcon with
// sListIcon and 0x1cdd8, flag = 1). If the async call returns a negative value
// that value is returned and nothing else runs. Otherwise that value is stored in
// the word that sceMcSync(0, &command, &result) receives as result, so it is also
// the value seen if sceMcSync does not write it. After the blocking sync:
// result == -3 returns -2; any other negative result returns -1; otherwise
// sceMcClose(sAsyncFd) runs, a second blocking sceMcSync(0, &command, &result)
// follows, and the return is -1 if the second result is negative, else the first
// result.
// Inferred: the first result is the number of bytes read, the second the close
// status; the async function's non-negative return is a file handle that is also
// left in sAsyncFd for the close.
// Unknown: what -3 means to the SDK here, and the async function's own behavior.
extern "C" {
extern int sMemCardInitialized;
extern int sAsyncFd;
extern unsigned char sIconDefinition[];
extern char sProductTitle[];
extern unsigned char sListIcon[];
void SetTitle__16VIIconDefinitionPCc(void* definition, const char* title);
void SetIcon__16VIIconDefinitionPvi(void* definition, void* icon, int size);
int VIMemCardReadFileAsync__FPCcPvi(const char* name, void* buffer, int size);
int sceMcSync(int mode, int* command, int* result);
int sceMcClose(int fd);

int VIMemCardReadFile__FPCcPvi(const char* name, void* buffer, int size) {
    if (sMemCardInitialized == 0) {
        sAsyncFd = -1;
        SetTitle__16VIIconDefinitionPCc(sIconDefinition, sProductTitle);
        SetIcon__16VIIconDefinitionPvi(sIconDefinition, sListIcon, 0x1cdd8);
        sMemCardInitialized = 1;
    }
    int started = VIMemCardReadFileAsync__FPCcPvi(name, buffer, size);
    int words[2];  // command, result
    words[1] = started;
    if (started < 0) return started;
    sceMcSync(0, &words[0], &words[1]);
    int got = words[1];
    if (got == -3) return -2;
    if (got < 0) return -1;
    sceMcClose(sAsyncFd);
    sceMcSync(0, &words[0], &words[1]);
    return words[1] < 0 ? -1 : got;
}
}
