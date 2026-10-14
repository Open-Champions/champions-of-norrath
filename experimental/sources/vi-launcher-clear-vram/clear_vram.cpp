// Observed ClearVRAM at 0x0101b1c8, 716 bytes. It returns. The four
// arguments are masked to 8 bits. 160 is stored at 0x12000000, then
// sceGsSyncV(0) runs five times. sceDmaGetChan(2) is stored in dmaGIF
// before memalign(64, 240). A null allocation prints the viglobalee text
// at line 50 and calls exit(0). The GIF packet calls still run if exit
// returns. sceGszbufaddr(0, 640, 224) is combined with 0x31000000 and
// with 0x8000 shifted left 17. The color word is 0xfe00 shifted left 46,
// with the four bytes in ascending argument order. After the packet is
// terminated, FlushCache(0) and sceGifPkSize run. sceDmaSendN receives
// the current dmaGIF word, the packet word at offset 4 masked with
// 0x0fffffff, and that size. sceGsSyncPath(0, 0) and __wrap_free follow.
// 161 is stored at 0x12000000, then sceGsSyncV(0) runs five more times.
// What the GS stores and the packet program is unknown.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_store64(unsigned address, unsigned long long value);
#define NORRATH_STORE64(address, value) norrath_host_store64((address), (value))
#else
#define NORRATH_STORE64(address, value)                                          \
    do {                                                                         \
        *reinterpret_cast<volatile unsigned long long*>(address) = (value);     \
    } while (0)
#endif

extern "C" {
void sceGsSyncV(int mode);
void* sceDmaGetChan(int identifier);
extern void* dmaGIF;
void* __wrap_memalign(unsigned alignment, unsigned size);
int printf(const char* text, ...);
void exit(int code);
struct GifPacket {
    unsigned first;
    unsigned second;
};
void sceGifPkInit(GifPacket* packet, void* buffer);
void sceGifPkReset(GifPacket* packet);
void sceGifPkAddGsData(GifPacket* packet, unsigned long long value);
unsigned sceGszbufaddr(int index, int width, int height);
void sceGifPkTerminate(GifPacket* packet);
void FlushCache(int mode);
int sceGifPkSize(GifPacket* packet);
void sceDmaSendN(void* channel, void* packet, int qwc);
void sceGsSyncPath(int mode, int timeout);
void __wrap_free(void* pointer);

void ClearVRAM__FUcUcUcUc(unsigned first, unsigned second, unsigned third, unsigned fourth) {
    unsigned char byte_a = static_cast<unsigned char>(first & 0xffu);
    unsigned char byte_b = static_cast<unsigned char>(second & 0xffu);
    unsigned char byte_c = static_cast<unsigned char>(third & 0xffu);
    unsigned char byte_d = static_cast<unsigned char>(fourth & 0xffu);
    NORRATH_STORE64(0x12000000u, 160ull);
    for (int repeat = 0; repeat < 5; ++repeat) sceGsSyncV(0);
    dmaGIF = sceDmaGetChan(2);
    void* buffer = __wrap_memalign(64, 240);
    GifPacket packet;
    if (buffer == nullptr) {
        printf("ERROR: Not enough memory! line:(%d), file:(%s)\n", 50,
               "../ee_support_dll/viglobalee.cpp");
        exit(0);
    }
    sceGifPkInit(&packet, buffer);
    sceGifPkReset(&packet);
    unsigned long long gif_header = 0x10000000ull << 32;
    gif_header |= 0x800aull;
    sceGifPkAddGsData(&packet, gif_header);
    sceGifPkAddGsData(&packet, 14ull);
    sceGifPkAddGsData(&packet, 1ull);
    sceGifPkAddGsData(&packet, 26ull);
    sceGifPkAddGsData(&packet, 0x100000ull);
    sceGifPkAddGsData(&packet, 76ull);
    unsigned long long zbuf = sceGszbufaddr(0, 640, 224);
    zbuf |= 0x31000000ull;
    zbuf |= 0x8000ull << 17;
    sceGifPkAddGsData(&packet, zbuf);
    sceGifPkAddGsData(&packet, 78ull);
    sceGifPkAddGsData(&packet, 0ull);
    sceGifPkAddGsData(&packet, 24ull);
    unsigned long long frame = 0x400ull;
    frame <<= 16;
    frame |= 0x400ull;
    frame <<= 16;
    sceGifPkAddGsData(&packet, frame);
    sceGifPkAddGsData(&packet, 64ull);
    sceGifPkAddGsData(&packet, 0x30000ull);
    sceGifPkAddGsData(&packet, 71ull);
    sceGifPkAddGsData(&packet, 6ull);
    sceGifPkAddGsData(&packet, 0ull);
    unsigned long long color = 0xfe00ull << 46;
    color |= byte_a;
    color |= static_cast<unsigned long long>(byte_b) << 8;
    color |= static_cast<unsigned long long>(byte_c) << 16;
    color |= static_cast<unsigned long long>(byte_d) << 24;
    sceGifPkAddGsData(&packet, color);
    sceGifPkAddGsData(&packet, 1ull);
    sceGifPkAddGsData(&packet, 0ull);
    sceGifPkAddGsData(&packet, 4ull);
    sceGifPkAddGsData(&packet, 0x40000000ull | 0x4000ull);
    sceGifPkAddGsData(&packet, 4ull);
    sceGifPkTerminate(&packet);
    FlushCache(0);
    int size = sceGifPkSize(&packet);
    unsigned masked = packet.second & 0x0fffffffu;
    sceDmaSendN(dmaGIF, reinterpret_cast<void*>(static_cast<unsigned long>(masked)), size);
    sceGsSyncPath(0, 0);
    __wrap_free(buffer);
    NORRATH_STORE64(0x12000000u, 161ull);
    for (int repeat = 0; repeat < 5; ++repeat) sceGsSyncV(0);
}
}
