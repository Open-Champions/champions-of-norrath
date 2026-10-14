// Observed findCutDialog__Fv (a local function, symbol type "t") at 0x00187a58, 220 bytes in
// SLUS_205.65. It uses three 80-byte stack buffers.
//  1. lumpName = engineCurrentLevelName + ".lmp" ("%s.lmp" at 0x00484370).
//  2. The string `cutSceneName` (0x00790f10) is copied to a second buffer; if it contains a '.',
//     the buffer is cut at the last one (an extension is removed).
//  3. The dialog name is that buffer + ".dbin" ("%s.dbin" at 0x00484378).
//  4. lumpFindResource(lumpName, dialogName) is called. When it returns null, the global
//     nmDialogNodes (0x00484364) is set to 0 and null is returned.
//  5. Otherwise lumpLoad(lumpName) and lumpFind(that result, dialogName) are called, and
//     nmDialogNodes is set to the unsigned quotient of the word at +8 of lumpFind's result
//     divided by 1096. The first call's result is returned.
// Inferred: cut-scene dialog data lives in a "dbin" entry of the level lump, as an array of
// 1096-byte records whose byte size is the word at +8; the quotient is a record count. The
// buffers' 80-byte sizes are the stack spacing. Unknown: the record layout and the exact types
// of the lump routine results (the return value of this function is used as a pointer by its
// caller).
extern "C" {
extern char engineCurrentLevelName[];
extern char cutSceneName[];
extern unsigned nmDialogNodes;
int sprintf(char*, const char*, ...);
char* strcpy(char*, const char*);
char* strrchr(const char*, int);
unsigned char* lumpFindResource__FPcT0(char* lump, char* name);
unsigned char* lumpLoad__FPc(char* lump);
unsigned* lumpFind__FPvPc(void* lump, char* name);

unsigned char* findCutDialog__Fv() {
    char lumpName[80];
    char baseName[80];
    char dialogName[80];
    sprintf(lumpName, "%s.lmp", engineCurrentLevelName);
    strcpy(baseName, cutSceneName);
    if (strrchr(baseName, '.') != 0) *strrchr(baseName, '.') = 0;
    sprintf(dialogName, "%s.dbin", baseName);
    unsigned char* found = lumpFindResource__FPcT0(lumpName, dialogName);
    if (found != 0) {
        unsigned* entry = lumpFind__FPvPc(lumpLoad__FPc(lumpName), dialogName);
        nmDialogNodes = entry[2] / 1096u;
    } else {
        nmDialogNodes = 0;
    }
    return found;
}
}
