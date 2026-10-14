// Observed setup_main__Fv at 0x01166b78, 456 bytes.
// It allocates a VISetup object of 0x153f0 bytes with __builtin_new and, when
// the allocation succeeded, constructs it (__7VISetup). All 16 words of
// sModuleId are set to -1 and initEzNetCnf runs. Init__7VISetupi(object, 1)
// is called and "ret = %i\n" printed with its result; a negative result
// returns -1 at once, leaving the object allocated and not freed.
// Otherwise Process__7VISetup is called until its result is nonzero. Result
// 3 calls RunPatcher__7VISetup; when that returns 4, RestartConnect__7VISetup
// runs and processing resumes. Any other RunPatcher result ends processing,
// as does any nonzero result other than 3.
// After processing: the word at object+86976 is saved, Free__7VISetup runs,
// ezNetCtlDownInterface(0) is called when the saved word was nonzero,
// exitEzNetCnf runs, "deleting setup\n" is printed, and a non-null object
// is destroyed with _$_7VISetup(object, 3). Then, by the processing result:
// 2 returns 2. Otherwise a nonzero saved word first runs unet_shutdown,
// sceSifMExitRpc and, for every sModuleId entry from index 15 down to 0 that
// is not negative, sceSifStopModule(id, 0, 0, &local) followed by
// sceSifUnloadModule(id) and a store of -1. Result 4 returns 4; any other
// result returns 5 for a nonzero saved word and 0 otherwise.
// Unknown: the meaning of the result codes and of the word at +86976.
extern "C" {
void* __builtin_new(unsigned size);
void* __7VISetup(void* object);
void _$_7VISetup(void* object, int flags);
int Init__7VISetupi(void* object, int value);
int Process__7VISetup(void* object);
int RunPatcher__7VISetup(void* object);
void RestartConnect__7VISetup(void* object);
void Free__7VISetup(void* object);
void initEzNetCnf();
void exitEzNetCnf();
int ezNetCtlDownInterface(int value);
void unet_shutdown__Fv();
void sceSifMExitRpc();
int sceSifStopModule(int id, int argument_length, const char* arguments, int* result);
int sceSifUnloadModule(int id);
int printf(const char* format, ...);
extern int sModuleId[16];
}

extern "C" int setup_main__Fv() {
    unsigned char* const object = static_cast<unsigned char*>(__builtin_new(0x153f0u));
    if (object != nullptr) __7VISetup(object);
    for (int index = 15; index >= 0; --index) sModuleId[index] = -1;
    initEzNetCnf();
    const int init_result = Init__7VISetupi(object, 1);
    printf("ret = %i\n", init_result);
    if (init_result < 0) return -1;
    int outcome;
    for (;;) {
        outcome = Process__7VISetup(object);
        if (outcome == 0) continue;
        if (outcome != 3) break;
        if (RunPatcher__7VISetup(object) != 4) break;
        RestartConnect__7VISetup(object);
    }
    const int saved = *reinterpret_cast<int*>(object + 86976);
    Free__7VISetup(object);
    if (saved != 0) ezNetCtlDownInterface(0);
    exitEzNetCnf();
    printf("deleting setup\n");
    if (object != nullptr) _$_7VISetup(object, 3);
    if (outcome == 2) return 2;
    if (saved != 0) {
        unet_shutdown__Fv();
        sceSifMExitRpc();
        for (int index = 15; index >= 0; --index) {
            if (sModuleId[index] >= 0) {
                int stop_result;
                sceSifStopModule(sModuleId[index], 0, nullptr, &stop_result);
                sceSifUnloadModule(sModuleId[index]);
                sModuleId[index] = -1;
            }
        }
    }
    if (outcome == 4) return 4;
    return saved != 0 ? 5 : 0;
}
