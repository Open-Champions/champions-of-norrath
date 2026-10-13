// Observed: Flush__8VIRaster at 0x011142d0 is 180 bytes. Let cursor be the pointer at offset 19492
// of the raster and start the pointer at 19496. If they are equal it returns zero. Otherwise it
// stores the byte 112 at cursor+3, zero at cursor+4 and a zero halfword at cursor+0, advances the
// cursor word by 16, and calls FlushCache(0), VIEEDMASync__Fi(1) and
// VIEEDMASend__FiPv(1, start). Then, if the word at 19496 differs from the word at 19484, both the
// cursor and the 19496 word become the word at 19484; otherwise both become the word at 19488.
// It zeroes the word at 19480, increments the word at 19500 and returns zero.
// Inferred: 19484 and 19488 are two DMA buffer starts used alternately (double buffering); 19496
// is the start of the packet being sent; 19500 counts sends. The byte 112 at +3 and the zero
// quadword count are the end-of-chain tag the packet is closed with.
// Unknown: the exact DMA tag field naming (the 112 is 0x70 in the tag's top byte), and what the
// word at 19480 counts.
extern "C" {
struct VIRaster;

int FlushCache(int mode);
int VIEEDMASync__Fi(int channel);
int VIEEDMASend__FiPv(int channel, void* start);

int Flush__8VIRaster(VIRaster* raster) {
    char* base = reinterpret_cast<char*>(raster);
    unsigned* cursor = reinterpret_cast<unsigned*>(base + 19492);
    unsigned begin = *cursor;
    if (begin == *reinterpret_cast<unsigned*>(base + 19496)) return 0;
    reinterpret_cast<unsigned char*>(begin)[3] = 112;
    *reinterpret_cast<unsigned*>(*cursor + 4) = 0;
    unsigned char* tail = reinterpret_cast<unsigned char*>(*cursor);
    *reinterpret_cast<unsigned short*>(tail) = 0;
    *cursor = reinterpret_cast<unsigned>(tail) + 16;
    FlushCache(0);
    VIEEDMASync__Fi(1);
    VIEEDMASend__FiPv(1, reinterpret_cast<void*>(*reinterpret_cast<unsigned*>(base + 19496)));
    unsigned next;
    if (*reinterpret_cast<unsigned*>(base + 19496) != *reinterpret_cast<unsigned*>(base + 19484))
        next = *reinterpret_cast<unsigned*>(base + 19484);
    else
        next = *reinterpret_cast<unsigned*>(base + 19488);
    *cursor = next;
    *reinterpret_cast<unsigned*>(base + 19496) = next;
    *reinterpret_cast<int*>(base + 19480) = 0;
    *reinterpret_cast<int*>(base + 19500) += 1;
    return 0;
}
}
