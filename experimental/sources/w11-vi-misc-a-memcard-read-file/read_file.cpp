// Observed: VIMemCardReadFile__FPCcPvi at 0x010d3a40 is 276 bytes.
// First use: if sMemCardInitialized (0x01192c98) is zero it stores -1 in sAsyncFd
// (0x011e8ff0), calls SetTitle__16VIIconDefinitionPCc(&sIconDefinition,
// sProductTitle) with the title store done before the call, then
// SetIcon__16VIIconDefinitionPvi(&sIconDefinition, sListIcon, 0x1cdd8), and sets
// sMemCardInitialized to 1 (sIconDefinition is at 0x011e8c20; sProductTitle at
// 0x011afa98 holds a 64-byte string; sListIcon at 0x01192ca0 is 0x1cdd8 bytes).
// Then v = VIMemCardReadFileAsync__FPCcPvi(path, buffer, size). If v < 0 that value
// is returned. Otherwise sceMcSync(0, &cmd, &result) is called with the result
// word previously holding v, r = result. r == -3 returns -2; r < 0 returns -1;
// otherwise sceMcClose(sAsyncFd), a second sceMcSync(0, &cmd, &result), and the
// return value is r if the new result is not negative, else -1.
// Inferred: result -3 is a distinct card error mapped to -2. The title/icon
// definition is built once on first read. Unknown: what the stored bytes of
// sIconDefinition represent and the callers' meaning of -1 and -2.
extern "C" {
extern int sMemCardInitialized;
extern int sAsyncFd;
extern char sIconDefinition[];
extern char sProductTitle[];
extern char sListIcon[];

void SetTitle__16VIIconDefinitionPCc(void* definition, const char* title);
void SetIcon__16VIIconDefinitionPvi(void* definition, void* icon, int size);
int VIMemCardReadFileAsync__FPCcPvi(const char* path, void* buffer, int size);
int sceMcSync(int mode, int* command, int* result);
int sceMcClose(int fd);

int VIMemCardReadFile__FPCcPvi(const char* path, void* buffer, int size) {
    if (sMemCardInitialized == 0) {
        sAsyncFd = -1;
        SetTitle__16VIIconDefinitionPCc(sIconDefinition, sProductTitle);
        SetIcon__16VIIconDefinitionPvi(sIconDefinition, sListIcon, 0x1cdd8);
        sMemCardInitialized = 1;
    }
    int started = VIMemCardReadFileAsync__FPCcPvi(path, buffer, size);
    if (started < 0) return started;
    alignas(16) int slots[2];
    slots[1] = started;
    sceMcSync(0, &slots[0], &slots[1]);
    int result = slots[1];
    if (result == -3) return -2;
    if (result < 0) return -1;
    sceMcClose(sAsyncFd);
    sceMcSync(0, &slots[0], &slots[1]);
    return slots[1] < 0 ? -1 : result;
}
}
