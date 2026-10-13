// Observed: FlushDMABufferIfFull__8VIRaster at 0x0110fab0, 280 bytes. Raster words used:
// +19492 current DMA write pointer, +19496 start of the buffer being filled, +19484 and
// +19488 two buffer bases, +19480 a counter, +19504 a flag, +19500 a counter, +17880 a counter.
// Steps: (1) if write pointer == start, return 0. (2) Let n = (pointer - start) >> 4 (signed).
// If n <= 0x9200 and the word at +19480 is below 821, return 0. (3) If the flag at +19504
// is zero: VIEEDMASync(1), Flip__8VIRaster(raster), subtract 1 from +17880, and reload both
// pointers. (4) Store 1 in the flag. (5) If the reloaded pointers are equal, return 0.
// (6) Write a terminator tag at the pointer (byte 3 = 0x70, word 1 = 0, 16-bit field 0),
// advance the pointer by 16, FlushCache(0), VIEEDMASync(1), VIEEDMASend(1, start). (7) Switch
// buffers: if the start word differs from the word at +19484 both the pointer and the start
// become that word, otherwise both become the word at +19488. (8) Clear +19480, add 1 to
// +19500, return 0.
// Inferred: a double-buffered display-list flush: the terminator tag 0x70 closes the chain,
// the DMA is started, and the other of two buffers becomes current; the flag records that a
// send is outstanding so the first flush after a send waits for it and flips the frame.
// Unknown: the member names, what the three counters count, the exact meaning of 0x9200 and
// 821 (a size and a vertex/packet threshold are only guesses), and the callees' return types.
#if defined(NORRATH_HOST_CONTRACT)
// On the 64-bit host a pointer field is modelled as a 32-bit offset into an arena.
extern "C" unsigned char* norrath_host_load_pointer(const void* field);
extern "C" void norrath_host_store_pointer(void* field, unsigned char* value);
#define NORRATH_LOAD_POINTER(field) norrath_host_load_pointer(field)
#define NORRATH_STORE_POINTER(field, value) norrath_host_store_pointer(field, value)
#else
#define NORRATH_LOAD_POINTER(field) (*reinterpret_cast<unsigned char* const*>(field))
#define NORRATH_STORE_POINTER(field, value) (*reinterpret_cast<unsigned char**>(field) = (value))
#endif

extern "C" {
int VIEEDMASync__Fi(int channel);
int Flip__8VIRaster(void* raster);
int FlushCache(int mode);
int VIEEDMASend__FiPv(int channel, void* data);

int FlushDMABufferIfFull__8VIRaster(void* raster) {
    unsigned char* r = static_cast<unsigned char*>(raster);
    unsigned char* pointer = NORRATH_LOAD_POINTER(r + 19492);
    unsigned char* start = NORRATH_LOAD_POINTER(r + 19496);
    if (pointer == start) return 0;
    int quadwords = static_cast<int>(pointer - start) >> 4;
    if (quadwords <= 0x9200 && *reinterpret_cast<int*>(r + 19480) < 821) return 0;
    if (*reinterpret_cast<int*>(r + 19504) == 0) {
        VIEEDMASync__Fi(1);
        Flip__8VIRaster(raster);
        *reinterpret_cast<int*>(r + 17880) -= 1;
        pointer = NORRATH_LOAD_POINTER(r + 19492);
        start = NORRATH_LOAD_POINTER(r + 19496);
    }
    *reinterpret_cast<int*>(r + 19504) = 1;
    if (pointer == start) return 0;
    NORRATH_LOAD_POINTER(r + 19492)[3] = 0x70;
    reinterpret_cast<unsigned int*>(NORRATH_LOAD_POINTER(r + 19492))[1] = 0;
    unsigned char* tag = NORRATH_LOAD_POINTER(r + 19492);
    *reinterpret_cast<unsigned short*>(tag) = 0;
    NORRATH_STORE_POINTER(r + 19492, tag + 16);
    FlushCache(0);
    VIEEDMASync__Fi(1);
    VIEEDMASend__FiPv(1, NORRATH_LOAD_POINTER(r + 19496));
    unsigned char* current = NORRATH_LOAD_POINTER(r + 19496);
    unsigned char* first = NORRATH_LOAD_POINTER(r + 19484);
    if (current != first) {
        NORRATH_STORE_POINTER(r + 19492, first);
        NORRATH_STORE_POINTER(r + 19496, first);
    } else {
        unsigned char* second = NORRATH_LOAD_POINTER(r + 19488);
        NORRATH_STORE_POINTER(r + 19492, second);
        NORRATH_STORE_POINTER(r + 19496, second);
    }
    *reinterpret_cast<int*>(r + 19480) = 0;
    *reinterpret_cast<int*>(r + 19500) += 1;
    return 0;
}
}
