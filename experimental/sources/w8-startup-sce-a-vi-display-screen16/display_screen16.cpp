/* Observed VIDisplayScreen16__FPCcib at 0x0106bee8, 704 bytes, in the
 * 0x01000000 module image. Arguments: a text pointer, an integer, and a flag.
 *
 * Three buffers come from __wrap_memalign(64, ...): 0x8c000 bytes (the image,
 * 640 x 448 x 2), 1024 bytes (the GIF packet area) and 0xa000 bytes. After
 * sceCdDiskReady(0), the name "\DATA2\%s%d.16;1" is formatted from the text
 * and the integer. sceCdSearchFile(gCdFile, name) repeats, with the text
 * "sceCdSearchFile FAIL\n" printed after each failure, until it returns 1.
 * The four bytes of sCdRmode are set to 0, 1, 0, 0 (the 1 is that search
 * result). sceCdRead(first word of gCdFile, 280, image, sCdRmode) repeats,
 * printing "CD Retry!\n" after each failure, until it returns 1, then
 * sceCdSync(0). The third buffer is cleared with memset and is never read.
 *
 * sceGsSyncV(0) runs. When the flag is nonzero, sceGsResetGraph(0, 1, 2, 0)
 * runs and 32544 is stored at 0x12000000. A GIF packet is built in a local
 * record with sceGifPkInit and sceGifPkReset; sceGifPkRefLoadImage receives
 * the packet, 0, 10, 10, the image, 0x8c00, 0, 0, 640, 448 (positions
 * observed; their meaning is not established here). sceGifPkEnd(0, 0, 0) and
 * sceGifPkTerminate follow. sceDmaGetChan(2), FlushCache(0), sceGsSyncV(0),
 * sceDmaSend with the channel and the packet record's second word,
 * sceGsSyncPath(0, 0), three __wrap_free calls (packet area, image, third
 * buffer), and sceGsSyncV(0) follow. Six 64-bit stores then go to
 * 0x12000000 (32547), 0x12000090 and 0x12000070 (0x51400 each), 0x120000a0
 * (0x1bf9ff0183227c), 0x12000080 (0x1bf9ff0183327d) and 0x120000d0 (0), and
 * a last sceGsSyncV(0) runs. Allocation results are not tested. What the
 * register stores program is unknown. Compiled output is not claimed to
 * match the original.
 */
#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_store64(unsigned address, unsigned long long value);
#define NORRATH_STORE64(address, value) norrath_host_store64((address), (value))
#else
#define NORRATH_STORE64(address, value)                                       \
    do {                                                                      \
        *reinterpret_cast<volatile unsigned long long*>(address) = (value);  \
    } while (0)
#endif

extern "C" {
struct GifPacket {
    unsigned first;
    unsigned second;
};
extern unsigned char sCdRmode[];
extern unsigned gCdFile[];

void* __wrap_memalign(unsigned alignment, unsigned size);
void __wrap_free(void* pointer);
int sceCdDiskReady(int mode);
int sceCdSearchFile(void* file, const char* name);
int sceCdRead(unsigned sector, unsigned count, void* buffer, void* mode);
int sceCdSync(int mode);
int sprintf(char* buffer, const char* format, ...);
int printf(const char* format, ...);
void* memset(void* destination, int value, unsigned count);
int sceGsSyncV(int mode);
int sceGsResetGraph(int mode, int interlace, int output, int field);
void sceGifPkInit(GifPacket* packet, void* buffer);
void sceGifPkReset(GifPacket* packet);
void sceGifPkRefLoadImage(GifPacket* packet, int a1, int a2, int a3, void* image, int quadwords,
                          int a6, int a7, int width, int height);
void sceGifPkEnd(GifPacket* packet, int a1, int a2, int a3);
void sceGifPkTerminate(GifPacket* packet);
void* sceDmaGetChan(int identifier);
void FlushCache(int mode);
int sceDmaSend(void* channel, unsigned address);
int sceGsSyncPath(int mode, int timeout);

void VIDisplayScreen16__FPCcib(const char* text, int index, bool reset_graph) {
    char name[256];
    void* const image = __wrap_memalign(64, 0x8c000u);
    void* const packet_area = __wrap_memalign(64, 1024u);
    void* const spare = __wrap_memalign(64, 0xa000u);
    sceCdDiskReady(0);
    sprintf(name, "\\DATA2\\%s%d.16;1", text, index);
    while (sceCdSearchFile(gCdFile, name) != 1) printf("sceCdSearchFile FAIL\n");
    sCdRmode[1] = 1;
    sCdRmode[3] = 0;
    sCdRmode[0] = 0;
    sCdRmode[2] = 0;
    while (sceCdRead(gCdFile[0], 280u, image, sCdRmode) != 1) printf("CD Retry!\n");
    sceCdSync(0);
    memset(spare, 0, 0xa000u);
    sceGsSyncV(0);
    if (reset_graph) {
        sceGsResetGraph(0, 1, 2, 0);
        NORRATH_STORE64(0x12000000u, 32544ull);
    }
    GifPacket packet;
    sceGifPkInit(&packet, packet_area);
    sceGifPkReset(&packet);
    sceGifPkRefLoadImage(&packet, 0, 10, 10, image, 0x8c00, 0, 0, 640, 448);
    sceGifPkEnd(&packet, 0, 0, 0);
    sceGifPkTerminate(&packet);
    void* const channel = sceDmaGetChan(2);
    FlushCache(0);
    sceGsSyncV(0);
    sceDmaSend(channel, packet.second);
    sceGsSyncPath(0, 0);
    __wrap_free(packet_area);
    __wrap_free(image);
    __wrap_free(spare);
    sceGsSyncV(0);
    NORRATH_STORE64(0x12000000u, 32547ull);
    NORRATH_STORE64(0x12000090u, 0x51400ull);
    NORRATH_STORE64(0x120000a0u, 0x1bf9ff0183227cull);
    NORRATH_STORE64(0x12000070u, 0x51400ull);
    NORRATH_STORE64(0x12000080u, 0x1bf9ff0183327dull);
    NORRATH_STORE64(0x120000d0u, 0ull);
    sceGsSyncV(0);
}
}
