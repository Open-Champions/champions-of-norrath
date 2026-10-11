// Observed initSupersample__Fv at 0x0018f7d0, 752 bytes. It builds the
// 192-byte DMA/GS packet at packetBuffer (0x0037f9e0) and masks the word at
// dispEnv+16 with 0xfffffe00. engineEnvTexAddress, shifted right by 8, is
// combined into the qword at packet offset 0x90. engineDrawBufferStartAddress,
// shifted right by 13, is combined into offset 0x20. That shift plus 140 is
// combined into offset 0x30, zwritesOn, and zwritesOff. The tag at offset 0
// is 0x1000000b and then (tag & 0x8fffffff) | 0x60000000, because bits 30:28
// of that tag compare equal to 1. The other arm, which stores 0x60000000 and
// zero at offset 192, is in the image and is not entered for this tag.
// FlushCache(0), sceDmaSend(dmaGif, packetBuffer), sceGsSyncPath(0, 0), and
// makeSamplePacket__Fb(fullFrameMode) follow. dmaGif is the word at
// 0x00486a3c. Fresh compilation is not an original-byte match.

extern "C" {
extern unsigned char packetBuffer[];
extern unsigned char dispEnv[];
extern unsigned engineEnvTexAddress;
extern unsigned engineDrawBufferStartAddress;
extern unsigned long long zwritesOn;
extern unsigned long long zwritesOff;
extern unsigned* dmaGif;
extern int fullFrameMode;
void FlushCache(int mode);
void sceDmaSend(unsigned* channel, void* packet);
void sceGsSyncPath(int mode, int timeout);
void makeSamplePacket__Fb(int full_frame);
}

static int shift_right(unsigned bits, int count) {
    if ((bits & 0x80000000u) == 0u) return static_cast<int>(bits >> count);
    const unsigned filled = 0xffffffffu << (32 - count);
    return static_cast<int>((bits >> count) | filled);
}

static void store_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}

static unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

static void store_u64(unsigned char* address, unsigned long long value) {
    for (int index = 0; index < 8; ++index)
        address[index] = static_cast<unsigned char>(value >> (8 * index));
}

extern "C" void initSupersample__Fv() {
    unsigned char* const packet = packetBuffer;
    const unsigned long long env =
        static_cast<unsigned long long>(shift_right(engineEnvTexAddress, 8));
    const unsigned long long env_qword =
        env | 0x1c008000ull | (1ull << 61) | 0x1c0000000ull;
    store_u32(dispEnv + 16, load_u32(dispEnv + 16) & 0xfffffe00u);

    const int draw_shift = shift_right(engineDrawBufferStartAddress, 13);
    const unsigned long long draw =
        static_cast<unsigned long long>(draw_shift);
    const unsigned long long draw_plus =
        static_cast<unsigned long long>(draw_shift + 140);
    const unsigned long long draw_qword = draw | 0x02140000ull;
    const unsigned long long plus_qword = draw_plus | 0x02000000ull;
    zwritesOff = plus_qword | 0x100000000ull;
    zwritesOn = plus_qword;

    store_u64(packet + 0x28, 77ull);
    store_u64(packet + 0x38, 79ull);
    store_u64(packet + 0x48, 64ull);
    store_u64(packet + 0x58, 65ull);
    store_u64(packet + 0x68, 25ull);
    store_u64(packet + 0x78, 67ull);
    store_u64(packet + 0x88, 72ull);
    store_u64(packet + 0xa8, 21ull);
    store_u64(packet + 0xb8, 9ull);
    store_u64(packet + 0x10, 0x100000000000800aull);
    store_u64(packet + 0x18, 14ull);
    store_u64(packet + 0x20, draw_qword);
    store_u64(packet + 0x30, plus_qword);
    store_u64(packet + 0x40, 0x01bf000004ff0000ull);
    store_u64(packet + 0x50, 0x01bf000004ff0000ull);
    store_u64(packet + 0x60, 0x0000720000005800ull);
    store_u64(packet + 0x70, 42ull);
    store_u64(packet + 0x80, 0x50013ull);
    store_u64(packet + 0x90, env_qword);
    store_u64(packet + 0x98, 7ull);
    store_u64(packet + 0xa0, 97ull);
    store_u64(packet + 0xb0, 0ull);
    store_u64(packet + 0x08, 0ull);

    const unsigned long long tag = 0x1000000bull;
    const unsigned long long high = (tag >> 28) & 7ull;
    store_u64(packet + 0x00, tag);
    if (high == 1ull) {
        store_u64(packet + 0x00, (tag & 0x8fffffffull) | 0x60000000ull);
    } else {
        store_u64(packet + 192, 0x60000000ull);
        store_u64(packet + 200, 0ull);
    }

    FlushCache(0);
    sceDmaSend(dmaGif, packet);
    sceGsSyncPath(0, 0);
    makeSamplePacket__Fb(fullFrameMode);
}
