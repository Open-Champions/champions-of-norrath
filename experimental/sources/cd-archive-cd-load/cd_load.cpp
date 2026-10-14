// Observed cdLoad__FPcPvii at 0x00165120, 276 bytes. This is not
// norrath_cd_load: that draft stores 2 through a named word, while the
// failure path here stores 2 at address 0.
// cdWait runs first. Directory lookup writes the sector at frame+8 and the
// size at frame+4. A zero result prints the format at 0x004633f0 and stores 2
// at address 0. A hit uses the file size when the requested length is 0,
// adds the arithmetic sector shift of the byte offset, and fills command at
// 0x006d86b0. cdResult receives the frame address, issueCommand runs with
// command mode already 1, and cdBusy becomes 1. Both paths then push
// 0x00ffffff and call cdProcess plus engineSyncVBlank while cdBusy is nonzero.
extern "C" {
extern unsigned char cdBusy;
extern unsigned cdResult;
extern unsigned char command[];
extern const unsigned char norrath_archive_load_failure_format_view[];
void cdWait__Fv();
int cdDirectoryLookup__FPcPiT1i(char* name, int* sector, int* bytes, int mode);
int scePrintf(const char* format, ...);
void issueCommand__Fv();
void cdProcess__Fv();
void enginePushBorderColor__FUi(unsigned color);
void engineSyncVBlank__Fv();
void enginePopBorderColor__Fv();
#if defined(NORRATH_HOST_CONTRACT)
void norrath_host_store_absolute(unsigned address, unsigned value);
#endif
static void store_absolute(unsigned address, unsigned value) {
#if defined(NORRATH_HOST_CONTRACT)
    norrath_host_store_absolute(address, value);
#else
    *reinterpret_cast<volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(address)) = value;
#endif
}
static unsigned arithmetic_shift(unsigned value, unsigned bits) {
    const unsigned shifted = value >> bits;
    if ((value & 0x80000000u) == 0u) return shifted;
    return shifted | (0xffffffffu << (32u - bits));
}
void cdLoad__FPcPvii(char* name, void* destination, int requested, int byte_offset) {
    cdWait__Fv();
    int size_word = 0;
    int sector_word = 0;
    unsigned char completion = 0;
    const int found = cdDirectoryLookup__FPcPiT1i(name, &sector_word, &size_word, 0);
    if (found == 0) {
        scePrintf(reinterpret_cast<const char*>(norrath_archive_load_failure_format_view), name);
        store_absolute(static_cast<unsigned>(found), 2u);
    } else {
        unsigned length = static_cast<unsigned>(requested);
        if (requested == 0) length = static_cast<unsigned>(size_word);
        const unsigned sector = static_cast<unsigned>(sector_word) +
            arithmetic_shift(static_cast<unsigned>(byte_offset), 11u);
        const unsigned blocks = arithmetic_shift(length + 2047u, 11u);
        auto store_word = [](unsigned char* slot, unsigned value) {
            slot[0] = static_cast<unsigned char>(value);
            slot[1] = static_cast<unsigned char>(value >> 8);
            slot[2] = static_cast<unsigned char>(value >> 16);
            slot[3] = static_cast<unsigned char>(value >> 24);
        };
        store_word(command + 12, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(destination)));
        store_word(command + 4, sector);
        store_word(command + 8, blocks);
        cdResult = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(&completion));
        completion = 0;
        store_word(command, 1u);
        issueCommand__Fv();
        cdBusy = 1;
    }
    enginePushBorderColor__FUi(0x00ffffffu);
    while (cdBusy != 0) {
        cdProcess__Fv();
        engineSyncVBlank__Fv();
    }
    enginePopBorderColor__Fv();
}
}
