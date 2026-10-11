// Observed launchSRS at 0x002d01e0, 1588 bytes. It returns.
// cdLoad__FPcPvii is the symbol at 0x00165120, extent 0x114. The drafted
// loader is norrath_cd_load and does not publish that address, so this
// import stays separate. Unnamed text addresses are unresolved objects.
extern "C" {
void lumpClear__Fi(int value);
void* memAlloc__FiPc(int bytes, char* name);
void memFree__FPv(void* block);
void __assert__FPCcUiT0i(const char* file, unsigned line, const char* text, int caller);
void cdLoad__FPcPvii(char* name, void* destination, int requested, int byte_offset);
void engineClear__Fv();
int sceSifQueryMaxFreeMemSize();
int sceSifQueryTotalFreeMemSize();
void scePrintf(char* text, ...);
void soundSleep__Fv();
void keyboardFree__Fv();
void textureFreeCacheBuffer__Fv();
void memSetMark__Fi(int value);
void mallinfo(void* info);
unsigned short* textGetString__FPc(char* key);
void ustrcpy__FPcPUs(char* destination, unsigned short* source);
int srs_main__FPCcPiT1PPcT1T1T3T1bT0T0i(
    const char* name, int* flag_word, int* count_word, char** first_slots, int* second_words,
    int* zero_word, char** third_slots, int* fourth_words, int low_bit_clear,
    const char* text_135, const char* copied_text, int launch_byte);
int sscanf(char* text, const char* format, int* first, int* second, int* third, int* fourth);
int sceSifSearchModuleByName(char* name);
int sceSifStopModule(int identifier, int second, int third, int* fourth);
void sceSifUnloadModule(int identifier);
void memClearMark__Fi(int value);
void memDeleteMarked__Fi(int value);
void memScrambleFree__Fv();
void textureAllocateCacheBuffer__Fv();
void keyboardRestart__Fv();
void engineRestore__Fv();
void soundRestore__Fb(int value);
void frontEndInit__Fi(int value);
void rejectForeignModules__Fv();
void sceMcInit();
void lumpLoad__FPc(char* name);
char* lumpFindResource__FPcT0(char* lump, char* name);
void lumpLock__Fi(int value);
void MC_Reset__Fv();

extern unsigned char coreBeenInSRS;
extern unsigned char coreLaunchSRS;
extern unsigned char enableMallocWrapper;
extern unsigned char netInterfaceIsUp;
extern unsigned char netGameLaunchInfo[];
extern unsigned char MC_ProtectRamSlot;
extern unsigned char fromSRSButEnteringGame;
extern unsigned char fromSRS;
extern int engineDisplayDisable;
extern unsigned char pad[];
extern char* haloTexture2;
extern char* haloTexture3;
extern char* haloTexture4;
extern char* haloTexture5;
extern char* haloTexture6;
extern char* haloTexture7;
extern void (*_srsctorsstart[])();
extern char _srsctorsend[];
extern char norrath_main_sdata_004866a0[];
extern char norrath_main_sdata_004867d0[];
extern char norrath_main_rodata_0047afe8[];
extern char norrath_main_rodata_0047aff8[];
extern char norrath_main_rodata_0047b008[];
extern char norrath_main_rodata_0047b018[];
extern char norrath_main_rodata_0047b028[];
extern char norrath_main_rodata_0047b038[];
extern char norrath_launchsrs_rodata_0047aeb8[];
extern char norrath_launchsrs_rodata_0047aed8[];
extern char norrath_launchsrs_sdata_004867b0[];
extern char norrath_launchsrs_rodata_0047af30[];
extern char norrath_launchsrs_rodata_0047af50[];
extern char norrath_launchsrs_rodata_0047af70[];
extern char norrath_launchsrs_rodata_0047af88[];
extern char norrath_launchsrs_rodata_0047afa0[];
extern char norrath_launchsrs_rodata_0047afb0[];
extern char norrath_launchsrs_rodata_0047afc0[];
extern char norrath_launchsrs_rodata_0047afd0[];
extern char norrath_launchsrs_sdata_004867b8[];
extern char norrath_launchsrs_sdata_004867c0[];
extern char norrath_launchsrs_sdata_004867c8[];

void launchSRS__Fv() {
    coreBeenInSRS = 1;
    lumpClear__Fi(50);
    coreLaunchSRS = 0;
    unsigned size = 0x400000u;
    void* reserved = 0;
    for (;;) {
        reserved = memAlloc__FiPc(static_cast<int>(size), norrath_launchsrs_rodata_0047aeb8);
        unsigned address = static_cast<unsigned>(reinterpret_cast<unsigned long>(reserved));
        if (static_cast<int>(0x01000000u) < static_cast<int>(address)) {
            __assert__FPCcUiT0i(norrath_main_sdata_004866a0, 2326u, norrath_launchsrs_rodata_0047aed8,
                                static_cast<int>(reinterpret_cast<unsigned long>(__builtin_return_address(0))));
        }
        unsigned end = address + size;
        size += 0x40000u;
        if (static_cast<int>(0x013590acu) < static_cast<int>(end)) break;
        memFree__FPv(reserved);
    }
    cdLoad__FPcPvii(norrath_launchsrs_sdata_004867b0,
                    reinterpret_cast<void*>(static_cast<unsigned long>(0x01000000u)), 0, 0);
    engineClear__Fv();
    unsigned shifted = static_cast<unsigned>(sceSifQueryMaxFreeMemSize()) >> 10;
    unsigned total = static_cast<unsigned>(sceSifQueryTotalFreeMemSize()) >> 10;
    scePrintf(norrath_launchsrs_rodata_0047af30, shifted, total);
    soundSleep__Fv();
    shifted = static_cast<unsigned>(sceSifQueryMaxFreeMemSize()) >> 10;
    total = static_cast<unsigned>(sceSifQueryTotalFreeMemSize()) >> 10;
    scePrintf(norrath_launchsrs_rodata_0047af50, shifted, total);
    keyboardFree__Fv();
    textureFreeCacheBuffer__Fv();
    shifted = static_cast<unsigned>(sceSifQueryMaxFreeMemSize()) >> 10;
    total = static_cast<unsigned>(sceSifQueryTotalFreeMemSize()) >> 10;
    scePrintf(norrath_launchsrs_rodata_0047af70, shifted, total);
    enableMallocWrapper = 1;
    memSetMark__Fi(0);
    scePrintf(norrath_launchsrs_rodata_0047af88);
    void (**cursor)() = _srsctorsstart;
    void (**ctors_end)() = reinterpret_cast<void (**)()>(&_srsctorsend);
    while (cursor != ctors_end) {
        void (*call)() = *cursor;
        ++cursor;
        call();
    }
    unsigned char before_info[4];
    unsigned char after_info[4];
    mallinfo(before_info);
    int flag_word = (netGameLaunchInfo[0] & 1u) != 0 ? 1 : 2;
    int count_word = 4;
    // The original pointer lists address stack slots 32 bytes apart.
    // What srs_main writes through them is unknown.
    char first_storage[4][32];
    char third_storage[4][32];
    char* first_slots[4] = {first_storage[0], first_storage[1], first_storage[2], first_storage[3]};
    char* third_slots[4] = {third_storage[0], third_storage[1], third_storage[2], third_storage[3]};
    int second_words[4];
    int zero_word = 0;
    int fourth_words[4];
    unsigned short* looked_up = textGetString__FPc(reinterpret_cast<char*>(netGameLaunchInfo + 199));
    char copied_text[80];
    ustrcpy__FPcPUs(copied_text, looked_up);
    scePrintf(norrath_launchsrs_rodata_0047afa0);
    int result = srs_main__FPCcPiT1PPcT1T1T3T1bT0T0i(
        reinterpret_cast<const char*>(netGameLaunchInfo + 7), &flag_word, &count_word, first_slots,
        second_words, &zero_word, third_slots, fourth_words, netInterfaceIsUp == 0 ? 1 : 0,
        reinterpret_cast<const char*>(netGameLaunchInfo + 135), copied_text, netGameLaunchInfo[295]);
    scePrintf(norrath_launchsrs_sdata_004867b8);
    engineDisplayDisable = 10000;
    int parsed_words[4];
    if (result == 100) {
        netGameLaunchInfo[4] = static_cast<unsigned char>(count_word);
        int spin = count_word;
        while (spin > 0) {
            volatile int step = spin;
            spin = step - 1;
        }
        netGameLaunchInfo[6] = static_cast<unsigned char>(zero_word);
        unsigned char* record = netGameLaunchInfo + 312;
        for (int index = 0; index < count_word; ++index) {
            sscanf(first_slots[index], norrath_launchsrs_rodata_0047afb0, &parsed_words[0], &parsed_words[1],
                   &parsed_words[2], &parsed_words[3]);
            record[-49] = static_cast<unsigned char>(parsed_words[0]);
            record[-48] = static_cast<unsigned char>(parsed_words[1]);
            record[-47] = static_cast<unsigned char>(parsed_words[2]);
            record[-46] = static_cast<unsigned char>(parsed_words[3]);
            int fields = sscanf(third_slots[index], norrath_launchsrs_rodata_0047afb0, &parsed_words[0],
                                &parsed_words[1], &parsed_words[2], &parsed_words[3]);
            record[-33] = static_cast<unsigned char>(parsed_words[0]);
            record[-32] = static_cast<unsigned char>(parsed_words[1]);
            record[-31] = static_cast<unsigned char>(parsed_words[2]);
            record[-30] = static_cast<unsigned char>(parsed_words[3]);
            *reinterpret_cast<int*>(record) = fields == 4 ? fourth_words[index] : second_words[index];
            record += 4;
        }
    }
    scePrintf(norrath_launchsrs_sdata_004867c0);
    int module = sceSifSearchModuleByName(norrath_launchsrs_rodata_0047afc0);
    if (module >= 0) {
        sceSifStopModule(module, 0, 0, &parsed_words[3]);
        sceSifUnloadModule(module);
    }
    scePrintf(norrath_launchsrs_sdata_004867c8);
    shifted = static_cast<unsigned>(sceSifQueryMaxFreeMemSize()) >> 10;
    total = static_cast<unsigned>(sceSifQueryTotalFreeMemSize()) >> 10;
    scePrintf(norrath_launchsrs_rodata_0047afd0, shifted, total);
    enableMallocWrapper = 0;
    memClearMark__Fi(0);
    memDeleteMarked__Fi(0);
    memFree__FPv(reserved);
    memScrambleFree__Fv();
    mallinfo(after_info);
    textureAllocateCacheBuffer__Fv();
    keyboardRestart__Fv();
    engineRestore__Fv();
    soundRestore__Fb(1);
    if (result == 100) {
        MC_ProtectRamSlot = 1;
        netInterfaceIsUp = 1;
        fromSRSButEnteringGame = 1;
    } else {
        frontEndInit__Fi(0);
        rejectForeignModules__Fv();
        netInterfaceIsUp = 0;
    }
    fromSRS = 1;
    sceMcInit();
    lumpLoad__FPc(norrath_main_sdata_004867d0);
    haloTexture2 = lumpFindResource__FPcT0(norrath_main_sdata_004867d0, norrath_main_rodata_0047afe8);
    haloTexture3 = lumpFindResource__FPcT0(norrath_main_sdata_004867d0, norrath_main_rodata_0047aff8);
    haloTexture4 = lumpFindResource__FPcT0(norrath_main_sdata_004867d0, norrath_main_rodata_0047b008);
    haloTexture5 = lumpFindResource__FPcT0(norrath_main_sdata_004867d0, norrath_main_rodata_0047b018);
    haloTexture6 = lumpFindResource__FPcT0(norrath_main_sdata_004867d0, norrath_main_rodata_0047b028);
    haloTexture7 = lumpFindResource__FPcT0(norrath_main_sdata_004867d0, norrath_main_rodata_0047b038);
    lumpLock__Fi(50);
    MC_Reset__Fv();
    unsigned char* pad_word = pad + 268;
    for (int step = 0; step < 4; ++step) {
        *reinterpret_cast<unsigned*>(pad_word) = 120u;
        pad_word -= 88;
    }
}
}
