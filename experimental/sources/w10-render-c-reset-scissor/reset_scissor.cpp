// Observed resetScissor__Fv at 0x0018bf30, 328 bytes. No arguments, no result.
// Does nothing while the signed byte coreLoadingScreenRunning (0x004866cc) is
// nonzero. Otherwise a 64-byte packet is built at glistp (0x004844c8, a pointer
// word):
//   +0   64-bit DMA tag, written last: quadword count 3, id 1 (id changed to 6 in
//        place when the id field is 1, which it always is here);
//   +8   VIF words 0x11000000 and 0x50000003;
//   +16  GIF tag 0x1000000000008000 with 2 added to its low word, then register
//        word 0xe (A+D);
//   +32  data 0x01bf000004ff0000, register 0x40;
//   +48  the same data, register 0x41.
// Registers 0x40 and 0x41 are the two scissor registers by the GS register
// numbering (right edge 0x4ff, bottom edge 0x1bf); that is inferred from the
// numbers, not from a name in the image. The data value is the same for both. A
// tag id other than 1 would leave the tag unchanged and append a quadword
// (0x60000000, 0); that branch is present in the
// original but unreachable. glistp is advanced past the packet, then
// drawAddToList is called with the packet address, list 4 and three zeros; its
// result is ignored.
extern "C" unsigned long glistp;
extern "C" volatile signed char coreLoadingScreenRunning;
extern "C" void* drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int list, void* texture,
                                                              void* record, int flags);

extern "C" void resetScissor__Fv() {
    if (coreLoadingScreenRunning != 0) return;
    unsigned char* packet = reinterpret_cast<unsigned char*>(glistp);
    unsigned long long* quads = reinterpret_cast<unsigned long long*>(packet);
    unsigned* words = reinterpret_cast<unsigned*>(packet);
    const unsigned long long scissor = 0x1bf000004ff0000ull;
    quads[2] = 0x1000000000008000ull;
    quads[3] = 0xeull;
    quads[4] = scissor;
    quads[5] = 0x40ull;
    quads[6] = scissor;
    quads[7] = 0x41ull;
    unsigned char* end = packet + 64;
    words[2] = 0x11000000u;
    words[3] = 0x50000000u | static_cast<unsigned>(((end - (packet + 12)) - 4) >> 4);
    unsigned long long count = ((static_cast<int>(end - (packet + 16)) >> 3) - 2) >> 1;
    quads[2] = count | quads[2];
    unsigned long long tag = (static_cast<unsigned long long>(
                                  static_cast<long long>(((static_cast<int>(end - packet) >> 3) - 2) >> 1))) |
                             0x10000000ull;
    quads[0] = tag;
    if (((static_cast<long long>(tag) >> 28) & 7) == 1) {
        quads[0] = (tag & 0x8fffffffull) | 0x60000000ull;
    } else {
        unsigned long long* extra = reinterpret_cast<unsigned long long*>(end);
        extra[0] = 0x60000000ull;
        extra[1] = 0ull;
        end += 16;
    }
    glistp = reinterpret_cast<unsigned long>(end);
    drawAddToList__FPviP8_textureP11_drawRecordi(packet, 4, 0, 0, 0);
}
