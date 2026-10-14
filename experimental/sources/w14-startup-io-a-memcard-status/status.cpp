// Observed: VIMemCardStatus__FPi at 0x010d2ce8 is 360 bytes and returns an int code.
// The argument is an optional output word. First, if the word sMemCardInitialized is
// zero: sAsyncFd = -1, SetTitle__16VIIconDefinitionPCc(&sIconDefinition,
// sProductTitle), SetIcon__16VIIconDefinitionPvi(&sIconDefinition, sListIcon,
// 0x1cdd8), then the flag becomes 1. Then sceMcGetInfo(0, 0, &type, &free, &format)
// is repeated until it returns 0, and sceMcSync(1, 0, &result) is polled, calling
// sceGsSyncV(0) after each poll that returns 0, until it returns nonzero.
// Code: 5 if type == 1 or the sync return is negative. Otherwise the output word,
// if given, receives free. Then by result (the word sceMcSync stored):
//   below -9: 1;  -2: 2;  -5: 5;  other values from -9 to -3: 0;
//   -1: 4 if format == 0, else 3;  0: 2 if format == 0, else 0;  above 0: 0.
// Inferred: type, free and format are the three output words of sceMcGetInfo as
// documented by the SDK; result is the command result of the polled command.
// Unknown: the meaning of the codes (they are only compared by callers), and
// whether type == 1 is a particular card kind.
extern "C" {
extern int sMemCardInitialized;
extern int sAsyncFd;
extern unsigned char sIconDefinition[];
extern char sProductTitle[];
extern unsigned char sListIcon[];
void SetTitle__16VIIconDefinitionPCc(void* definition, const char* title);
void SetIcon__16VIIconDefinitionPvi(void* definition, void* icon, int size);
int sceMcGetInfo(int port, int slot, int* type, int* free, int* format);
int sceMcSync(int mode, int* command, int* result);
void sceGsSyncV(int field);

int VIMemCardStatus__FPi(int* freeOut) {
    if (sMemCardInitialized == 0) {
        sAsyncFd = -1;
        SetTitle__16VIIconDefinitionPCc(sIconDefinition, sProductTitle);
        SetIcon__16VIIconDefinitionPvi(sIconDefinition, sListIcon, 0x1cdd8);
        sMemCardInitialized = 1;
    }
    int info[4];  // type, free, format, result
    while (sceMcGetInfo(0, 0, &info[0], &info[1], &info[2]) != 0) {
    }
    int synced;
    while ((synced = sceMcSync(1, 0, &info[3])) == 0) sceGsSyncV(0);
    if (info[0] == 1 || synced < 0) return 5;
    if (freeOut != 0) *freeOut = info[1];
    int result = info[3];
    if (result < -9) return 1;
    if (result == -2) return 2;
    if (result < -1) return result == -5 ? 5 : 0;
    if (result == -1) return info[2] == 0 ? 4 : 3;
    if (result != 0) return 0;
    return info[2] != 0 ? 0 : 2;
}
}
