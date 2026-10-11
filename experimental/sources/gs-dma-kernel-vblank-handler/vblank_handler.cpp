/* Observed vblankHandler__Fi at 0x0018e4a0, 512 bytes. The cause argument is
 * not read. A nonzero engineDisplayDisable stores 0 at 0x12000000; otherwise
 * it stores 2. A nonzero specialMpegMode skips the draw walk. Otherwise two
 * words at draw+36 and draw+52 are compared with the signed word at draw+56.
 * A value that is greater and below 0x7fffffff replaces that word and stores
 * its index at draw+60. The last such index wins. The word at draw+28 plus
 * index*16 then selects the rest of the stores. A nonzero word stores 1 at
 * 0x12000020 and builds the display words from displayBuffer1StartAddress.
 * A zero word stores 3 at 0x12000020 and uses displayBuffer2StartAddress when
 * the index is nonzero. Both paths then take bit 13 of the 64-bit word at
 * 0x12001000, store that bit in vblankField, add 1 to vblankCount, signal
 * vblankSemaphore and then vblankSemaphore2, execute sync and ei, and return
 * 0. This source is not an original-byte match.
 */
extern "C" int engineDisplayDisable;
extern "C" unsigned char specialMpegMode;
extern "C" unsigned char draw[];
extern "C" unsigned displayBuffer1StartAddress;
extern "C" unsigned displayBuffer2StartAddress;
extern "C" int vblankCount;
extern "C" unsigned char vblankField;
extern "C" int vblankSemaphore;
extern "C" int vblankSemaphore2;
extern "C" int iSignalSema(int id);

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_gs_store(unsigned address, unsigned long long value);
extern "C" unsigned long long norrath_host_gs_load(unsigned address);
extern "C" void norrath_host_sync();
extern "C" void norrath_host_ei();
static void norrath_gs_store(unsigned address, unsigned long long value) {
    norrath_host_gs_store(address, value);
}
static unsigned long long norrath_gs_load(unsigned address) { return norrath_host_gs_load(address); }
static void norrath_sync() { norrath_host_sync(); }
static void norrath_ei() { norrath_host_ei(); }
#else
static void norrath_gs_store(unsigned address, unsigned long long value) {
    *reinterpret_cast<volatile unsigned long long*>(address) = value;
}
static unsigned long long norrath_gs_load(unsigned address) {
    return *reinterpret_cast<volatile unsigned long long*>(address);
}
static void norrath_sync() { asm volatile("sync" ::: "memory"); }
static void norrath_ei() { asm volatile("ei" ::: "memory"); }
#endif

static unsigned norrath_load32(int offset) {
    return static_cast<unsigned>(draw[offset]
        | (draw[offset + 1] << 8)
        | (draw[offset + 2] << 16)
        | (draw[offset + 3] << 24));
}

static void norrath_store32(int offset, unsigned value) {
    draw[offset] = static_cast<unsigned char>(value);
    draw[offset + 1] = static_cast<unsigned char>(value >> 8);
    draw[offset + 2] = static_cast<unsigned char>(value >> 16);
    draw[offset + 3] = static_cast<unsigned char>(value >> 24);
}

static void norrath_finish_vblank() {
    const unsigned long long csr = norrath_gs_load(0x12001000u);
    vblankField = static_cast<unsigned char>((csr >> 13) & 1ull);
    vblankCount += 1;
    iSignalSema(vblankSemaphore);
    iSignalSema(vblankSemaphore2);
    norrath_sync();
    norrath_ei();
}

extern "C" int vblankHandler__Fi(int cause) {
    (void)cause;
    norrath_gs_store(0x12000000u, engineDisplayDisable != 0 ? 0ull : 2ull);
    if (specialMpegMode == 0) {
        int found = -1;
        unsigned chosen = 0;
        const int threshold = static_cast<int>(norrath_load32(56));
        int limit = 0x7fffffff;
        for (int index = 0; index < 2; ++index) {
            const int value = static_cast<int>(norrath_load32(36 + index * 16));
            if (threshold < value && value < limit) {
                chosen = static_cast<unsigned>(value);
                found = index;
                limit = value;
            }
        }
        if (found >= 0) {
            norrath_store32(56, chosen);
            norrath_store32(60, static_cast<unsigned>(found));
            const unsigned flag = norrath_load32(28 + found * 16);
            if (flag != 0u) {
                norrath_gs_store(0x12000020u, 1ull);
                const unsigned long long high = 0x8000ull << 28;
                const unsigned frame = (displayBuffer1StartAddress >> 13) | 0x1400u;
                const unsigned long long marked = static_cast<unsigned long long>(frame) | high;
                const unsigned long long display = 0x1be9ff0183227cull;
                norrath_gs_store(0x12000070u, marked);
                norrath_gs_store(0x12000090u, frame);
                norrath_gs_store(0x12000080u, display);
                norrath_gs_store(0x120000a0u, display);
            } else {
                norrath_gs_store(0x12000020u, 3ull);
                const unsigned source = found != 0
                    ? displayBuffer2StartAddress
                    : displayBuffer1StartAddress;
                const unsigned frame = (source >> 13) | 0x1400u;
                norrath_gs_store(0x12000090u, frame);
            }
        }
    }
    norrath_finish_vblank();
    return 0;
}
