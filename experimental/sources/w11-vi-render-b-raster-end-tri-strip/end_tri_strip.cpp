// Observed: EndTriStrip__8VIRaster at 0x0110c980 is 260 bytes. Let cursor be the pointer at offset
// 52 of the raster, header the pointer at offset 56, and count the word at 48. In order:
//   1. The quadword at cursor gets words 0 to 2 = 0 and word 3 = (the static word named
//      _8VIRaster$RStripOffset) | 0x14000000.
//   2. The quadword at header gets words 0 to 2 = 0 and word 3 = ((n - 2) << 16) | 0x6c008000,
//      where n = (cursor + 16 - header) >> 4 (arithmetic shift of the byte difference).
//   3. The quadword after the header gets word 0 = count and words 1 to 3 = 0.
//   4. Offset 52 becomes header + 32.
//   5. Offset 17884 is set to zero. The DMA tag at the pointer stored at offset 19492 is
//      written: byte 3 = 16, word 1 = 0, halfword 0 = n. That pointer then advances by
//      (n << 4) + 16 bytes and FlushDMABufferIfFull__8VIRaster(raster) is called.
// The result is zero.
// Inferred: this closes the packet BeginTriStrip opened (a DMA tag at the old 19492 pointer,
// a header quadword at the strip start, a count quadword, vertex quadwords written by Vertex);
// the high bits 0x6c008000 and 0x14000000 look like VIF command words but are left as constants.
// Unknown: the meaning of the commands in those words and of the symbol's value.
extern "C" {
struct VIRaster;

extern unsigned norrath_raster_strip_offset __asm__("_8VIRaster$RStripOffset");
int FlushDMABufferIfFull__8VIRaster(VIRaster* raster);

int EndTriStrip__8VIRaster(VIRaster* raster) {
    char* base = reinterpret_cast<char*>(raster);
    unsigned*& cursor = *reinterpret_cast<unsigned**>(base + 52);
    unsigned* header = *reinterpret_cast<unsigned**>(base + 56);
    unsigned** packet = reinterpret_cast<unsigned**>(base + 19492);

    cursor[0] = 0;
    cursor[1] = 0;
    cursor[2] = 0;
    cursor[3] = norrath_raster_strip_offset | 0x14000000u;

    int quads = static_cast<int>(reinterpret_cast<char*>(cursor) + 16 - reinterpret_cast<char*>(header)) >> 4;
    header[0] = 0;
    header[1] = 0;
    header[2] = 0;
    header[3] = (static_cast<unsigned>(quads - 2) << 16) | 0x6c008000u;

    header[4] = *reinterpret_cast<unsigned*>(base + 48);
    header[5] = 0;
    header[6] = 0;
    header[7] = 0;
    cursor = header + 8;

    unsigned char* tag = reinterpret_cast<unsigned char*>(*packet);
    *reinterpret_cast<int*>(base + 17884) = 0;
    tag[3] = 16;
    reinterpret_cast<unsigned*>(tag)[1] = 0;
    *reinterpret_cast<unsigned short*>(tag) = static_cast<unsigned short>(quads);
    *packet = reinterpret_cast<unsigned*>(tag + (quads << 4) + 16);
    FlushDMABufferIfFull__8VIRaster(raster);
    return 0;
}
}
