/* Observed at 0x0018fac0, 832 bytes in the pinned image. Packet stores
 * use the uncached alias; the DMA call receives the ordinary symbol address.
 * Complete packet construction, not a byte-match or hardware execution claim. */
using U64 = unsigned long long;
extern "C" U64 frameStatePacket[];
extern "C" int engineDrawBufferStartAddress, engineYOffset, engineScreenSlide;
extern "C" int engineScissory1, engineScissory2;
extern "C" void* dmaGif;
extern "C" void sceDmaSend(void*, void*);
extern "C" int sceGsSyncPath(int, int);
extern "C" void frameStartPacket__FUibi(unsigned color, int full, int field) {
#ifdef NORRATH_HOST_CONTRACT
    U64* p = frameStatePacket;
#else
    U64* p = reinterpret_cast<U64*>(reinterpret_cast<unsigned long>(frameStatePacket) | 0x30000000u);
#endif
    unsigned n = 4;
    p[1] = 0; p[2] = 0x1000000000008000ull; p[3] = 14;
    auto put = [&](U64 value, U64 reg) { p[n++] = value; p[n++] = reg; };
    int base = engineDrawBufferStartAddress >> 13;
    put(static_cast<U64>(static_cast<long long>(base)) | 0x2140000, 76);
    put(static_cast<U64>(static_cast<long long>(base + 140)) | 0x2000000, 78);
    put(0x1bf04ff0000ull, 64); put(0x1bf04ff0000ull, 65);
    U64 offset = (static_cast<U64>(engineYOffset) << 32) | 0x5800;
    put(offset,24); put(offset,25);
    engineYOffset = (field ? 0x7208 : 0x7200) + (full ? 0 : static_cast<int>(static_cast<unsigned>(engineScreenSlide) << 4));
    put(1,69); put(0,73); put(1,70); put(1,26);
    if (color) {
        put(color,1); put(0x387f4,71); put(6,0);
        for (unsigned i=0; i<20; ++i) {
            put((0x5800ull + i*0x400) | 0x72000000ull,5);
            int y = static_cast<int>(static_cast<unsigned>(engineScreenSlide + 2272) << 4);
            put((0x5c00ull + i*0x400) | (static_cast<U64>(static_cast<long long>(y)) << 16),5);
        }
    }
    if (!full) {
        put((static_cast<U64>(engineScissory1)<<32) | (static_cast<U64>(engineScissory2)<<48) | 0x4ff0000,64);
        put((static_cast<U64>(engineScissory1)<<32) | (static_cast<U64>(engineScissory2)<<48) | 0x4ff0000,65);
    }
    p[0] = 0x60000000u | (n/2-1);
    p[2] |= n/2-2;
    sceDmaSend(dmaGif,frameStatePacket);
    sceGsSyncPath(0,0);
}
