#include <cstdint>
#include <cstring>
extern "C" void launchSRS__Fv();

static int mode;
static int allocs, frees, asserts, clears, sleeps, keys_free, tex_free, marks, ctors;
static int fronts, rejects, locks, resets, stops, unloads, scrambles;
static int result_code = 99;
static int module_id = -1;
static void* reserved_seen;
static unsigned char* been_at_clear;
static unsigned char launch_at_clear;

unsigned char coreBeenInSRS;
unsigned char coreLaunchSRS;
unsigned char enableMallocWrapper;
unsigned char netInterfaceIsUp;
unsigned char netGameLaunchInfo[400];
unsigned char MC_ProtectRamSlot;
unsigned char fromSRSButEnteringGame;
unsigned char fromSRS;
int engineDisplayDisable;
alignas(4) unsigned char pad[400];
char* haloTexture2;
char* haloTexture3;
char* haloTexture4;
char* haloTexture5;
char* haloTexture6;
char* haloTexture7;

extern "C" void ctor_a() { ++ctors; }
extern "C" void ctor_b() { ++ctors; }
extern "C" void (*_srsctorsstart[])();
extern "C" char _srsctorsend;
asm(
    ".section __DATA,__data\n"
    ".globl __srsctorsstart\n"
    "__srsctorsstart:\n"
    ".quad _ctor_a\n"
    ".quad _ctor_b\n"
    ".globl __srsctorsend\n"
    "__srsctorsend:\n"
    ".byte 0\n");

char norrath_main_sdata_004866a0[] = "site";
char norrath_main_sdata_004867d0[] = "lump";
char norrath_main_rodata_0047afe8[] = "h2";
char norrath_main_rodata_0047aff8[] = "h3";
char norrath_main_rodata_0047b008[] = "h4";
char norrath_main_rodata_0047b018[] = "h5";
char norrath_main_rodata_0047b028[] = "h6";
char norrath_main_rodata_0047b038[] = "h7";
char norrath_launchsrs_rodata_0047aeb8[] = "alloc";
char norrath_launchsrs_rodata_0047aed8[] = "expr";
char norrath_launchsrs_sdata_004867b0[] = "cd";
char norrath_launchsrs_rodata_0047af30[] = "p1";
char norrath_launchsrs_rodata_0047af50[] = "p2";
char norrath_launchsrs_rodata_0047af70[] = "p3";
char norrath_launchsrs_rodata_0047af88[] = "p4";
char norrath_launchsrs_rodata_0047afa0[] = "p5";
char norrath_launchsrs_rodata_0047afb0[] = "fmt";
char norrath_launchsrs_rodata_0047afc0[] = "mod";
char norrath_launchsrs_rodata_0047afd0[] = "p6";
char norrath_launchsrs_sdata_004867b8[] = "b8";
char norrath_launchsrs_sdata_004867c0[] = "c0";
char norrath_launchsrs_sdata_004867c8[] = "c8";
static char parsed_text[] = "parsed";
static char other_text[] = "other";
static unsigned short wide_text[] = {1, 0};

static int fail(int code) { return code; }

extern "C" void lumpClear__Fi(int value) {
    if (value != 50 || coreBeenInSRS != 1) return;
    been_at_clear = &coreBeenInSRS;
    launch_at_clear = coreLaunchSRS;
    ++clears;
}
extern "C" void* memAlloc__FiPc(int bytes, char* name) {
    if (name != norrath_launchsrs_rodata_0047aeb8) return nullptr;
    ++allocs;
    if (mode == 2 && allocs == 1) {
        if (bytes != 0x400000) return nullptr;
        return reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x00100000u));
    }
    if (mode == 3 && allocs == 1) {
        return reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x02000000u));
    }
    if (bytes != (mode == 2 ? 0x440000 : 0x400000)) return nullptr;
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x01000000u));
}
extern "C" void memFree__FPv(void* block) {
    if (frees == 0 && mode == 2 && block != reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x00100000u))) return;
    reserved_seen = block;
    ++frees;
}
extern "C" void __assert__FPCcUiT0i(const char* file, unsigned line, const char* text, int) {
    if (file != norrath_main_sdata_004866a0 || line != 2326u || text != norrath_launchsrs_rodata_0047aed8) return;
    ++asserts;
}
extern "C" void cdLoad__FPcPvii(char* name, void* destination, int requested, int byte_offset) {
    if (name != norrath_launchsrs_sdata_004867b0 || requested || byte_offset) fail(1);
    if (destination != reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x01000000u))) fail(1);
}
extern "C" void engineClear__Fv() {}
extern "C" int sceSifQueryMaxFreeMemSize() { return 2048; }
extern "C" int sceSifQueryTotalFreeMemSize() { return 4096; }
extern "C" void scePrintf(char*, ...) {}
extern "C" void soundSleep__Fv() { ++sleeps; }
extern "C" void keyboardFree__Fv() { ++keys_free; }
extern "C" void textureFreeCacheBuffer__Fv() { ++tex_free; }
extern "C" void memSetMark__Fi(int value) { if (value == 0 && enableMallocWrapper == 1) ++marks; }
extern "C" void mallinfo(void*) {}
extern "C" unsigned short* textGetString__FPc(char* key) {
    if (key != reinterpret_cast<char*>(netGameLaunchInfo + 199)) return nullptr;
    return wide_text;
}
extern "C" void ustrcpy__FPcPUs(char* destination, unsigned short* source) {
    if (source != wide_text || destination == nullptr) fail(1);
}
extern "C" int srs_main__FPCcPiT1PPcT1T1T3T1bT0T0i(
    const char* name, int* flag_word, int* count_word, char** first_slots, int* second_words,
    int* zero_word, char** third_slots, int* fourth_words, int low_bit_clear,
    const char* text_135, const char* copied_text, int launch_byte) {
    if (name != reinterpret_cast<char*>(netGameLaunchInfo + 7)) return 1;
    if (text_135 != reinterpret_cast<char*>(netGameLaunchInfo + 135)) return 1;
    if (copied_text == nullptr || launch_byte != netGameLaunchInfo[295]) return 1;
    if (flag_word == nullptr || count_word == nullptr || zero_word == nullptr) return 1;
    int expect_flag = (netGameLaunchInfo[0] & 1u) ? 1 : 2;
    if (*flag_word != expect_flag || *count_word != 4 || *zero_word != 0) return 1;
    if (low_bit_clear != (netInterfaceIsUp == 0)) return 1;
    if (mode == 4) {
        first_slots[0] = parsed_text;
        third_slots[0] = other_text;
        second_words[0] = 11;
        fourth_words[0] = 22;
        *count_word = 1;
        *zero_word = 7;
    }
    (void)second_words;
    (void)fourth_words;
    return result_code;
}
extern "C" int sscanf(char* text, const char* format, int* first, int* second, int* third, int* fourth) {
    if (format != norrath_launchsrs_rodata_0047afb0) return 0;
    *first = 1; *second = 2; *third = 3; *fourth = 4;
    return text == other_text ? 4 : 1;
}
extern "C" int sceSifSearchModuleByName(char* name) {
    if (name != norrath_launchsrs_rodata_0047afc0) return -1;
    return module_id;
}
extern "C" int sceSifStopModule(int identifier, int second, int third, int* fourth) {
    if (identifier != module_id || second || third || fourth == nullptr) return 1;
    ++stops;
    return 0;
}
extern "C" void sceSifUnloadModule(int identifier) { if (identifier == module_id) ++unloads; }
extern "C" void memClearMark__Fi(int value) { if (value == 0 && enableMallocWrapper == 0) ++marks; }
extern "C" void memDeleteMarked__Fi(int) {}
extern "C" void memScrambleFree__Fv() { ++scrambles; }
extern "C" void textureAllocateCacheBuffer__Fv() {}
extern "C" void keyboardRestart__Fv() {}
extern "C" void engineRestore__Fv() {}
extern "C" void soundRestore__Fb(int value) { if (value != 1) fail(1); }
extern "C" void frontEndInit__Fi(int value) { if (value == 0) ++fronts; }
extern "C" void rejectForeignModules__Fv() { ++rejects; }
extern "C" void sceMcInit() {}
extern "C" void lumpLoad__FPc(char* name) { if (name != norrath_main_sdata_004867d0) fail(1); }
extern "C" char* lumpFindResource__FPcT0(char* lump, char* name) {
    if (lump != norrath_main_sdata_004867d0) return nullptr;
    return name;
}
extern "C" void lumpLock__Fi(int value) { if (value == 50) ++locks; }
extern "C" void MC_Reset__Fv() { ++resets; }

static void reset() {
    std::memset(netGameLaunchInfo, 0, sizeof netGameLaunchInfo);
    std::memset(pad, 0, sizeof pad);
    coreBeenInSRS = coreLaunchSRS = enableMallocWrapper = netInterfaceIsUp = 0;
    MC_ProtectRamSlot = fromSRSButEnteringGame = fromSRS = 0;
    engineDisplayDisable = 0;
    allocs = frees = asserts = clears = sleeps = keys_free = tex_free = marks = ctors = 0;
    fronts = rejects = locks = resets = stops = unloads = scrambles = 0;
    haloTexture2 = haloTexture3 = haloTexture4 = haloTexture5 = haloTexture6 = haloTexture7 = nullptr;
    result_code = 99;
    module_id = -1;
    coreLaunchSRS = 9;
}

int main() {
    if (reinterpret_cast<char*>(&_srsctorsend) != reinterpret_cast<char*>(_srsctorsstart + 2)) return 30;
    reset();
    mode = 1;
    netGameLaunchInfo[295] = 5;
    launchSRS__Fv();
    if (clears != 1 || launch_at_clear != 9 || coreLaunchSRS != 0) return 2;
    if (allocs != 1 || frees < 1 || asserts != 0 || sleeps != 1 || keys_free != 1 || tex_free != 1) return 3;
    if (marks < 1 || ctors != 2 || fronts != 1 || rejects != 1 || fromSRS != 1) return 4;
    if (netInterfaceIsUp != 0 || MC_ProtectRamSlot != 0 || fromSRSButEnteringGame != 0) return 5;
    if (engineDisplayDisable != 10000 || locks != 1 || resets != 1 || enableMallocWrapper != 0) return 6;
    if (haloTexture2 != norrath_main_rodata_0047afe8 || haloTexture7 != norrath_main_rodata_0047b038) return 7;
    if (*reinterpret_cast<unsigned*>(pad + 268) != 120u || *reinterpret_cast<unsigned*>(pad + 4) != 120u) return 8;
    if (stops != 0 || scrambles != 1) return 9;

    reset();
    mode = 2;
    launchSRS__Fv();
    if (allocs != 2 || frees < 2) return 10;

    reset();
    mode = 3;
    launchSRS__Fv();
    if (asserts != 1 || allocs != 1) return 11;

    reset();
    mode = 4;
    result_code = 100;
    module_id = 7;
    netGameLaunchInfo[0] = 1;
    netGameLaunchInfo[295] = 9;
    netInterfaceIsUp = 0;
    launchSRS__Fv();
    if (fronts != 0 || rejects != 0 || stops != 1 || unloads != 1) return 12;
    if (MC_ProtectRamSlot != 1 || netInterfaceIsUp != 1 || fromSRSButEnteringGame != 1 || fromSRS != 1) return 13;
    if (netGameLaunchInfo[4] != 1 || netGameLaunchInfo[6] != 7) return 14;
    unsigned char* record = netGameLaunchInfo + 312;
    if (record[-49] != 1 || record[-46] != 4 || record[-33] != 1 || record[-30] != 4) return 15;
    if (*reinterpret_cast<int*>(record) != 22) return 16;
    return 0;
}
