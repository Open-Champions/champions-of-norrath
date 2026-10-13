// Observed: SetGSScissoring__8VIRaster at 0x011129b8 is 288 bytes. Write x0, y0 for the words at
// offsets 17716 and 17720 of the raster, and x1 = x0 + word[17724] - 1, y1 = y0 + word[17728] - 1.
// With cursor the pointer stored at offset 19492, it writes an 80-byte packet there:
//   +0  : byte 3 = 16, word 1 = 0, halfword 0 = 4  (a four-quadword chain tag)
//   +16 : words 0, 1 = 0, word 2 = 0x10000000, word 3 = 0x50000003
//   +32 : doubleword 0x1000000000008002, doubleword 14
//   +48 : doubleword S, doubleword 64
//   +64 : doubleword S, doubleword 65
// where S = x0 | (x1 << 16) | (y0 << 32) | (y1 << 48), each term sign-extended to 64 bits
// first (no masking) and OR-ed. It then advances the pointer at 19492 by 80 bytes and returns
// zero.
// Inferred: S is a GS scissor register value (X0, X1, Y0, Y1 in the layout of fields at 0, 16,
// 32 and 48) and the register numbers 64 and 65 are the SCISSOR_1 and SCISSOR_2 registers; the
// +32 doubleword pair is a GIF tag with two A+D entries; the +16 words are two VIF commands.
// Unknown: what the four raster words at 17716 to 17728 are named (a rectangle by position and
// size).
extern "C" {
struct VIRaster;

int SetGSScissoring__8VIRaster(VIRaster* raster) {
    char* base = reinterpret_cast<char*>(raster);
    unsigned** cursorSlot = reinterpret_cast<unsigned**>(base + 19492);
    int x0 = *reinterpret_cast<int*>(base + 17716);
    int y0 = *reinterpret_cast<int*>(base + 17720);
    int x1 = x0 + *reinterpret_cast<int*>(base + 17724) - 1;
    int y1 = y0 + *reinterpret_cast<int*>(base + 17728) - 1;

    unsigned char* packet = reinterpret_cast<unsigned char*>(*cursorSlot);
    packet[3] = 16;
    reinterpret_cast<unsigned*>(packet)[1] = 0;
    *reinterpret_cast<unsigned short*>(packet) = 4;
    *cursorSlot = reinterpret_cast<unsigned*>(packet + 80);

    unsigned* vif = reinterpret_cast<unsigned*>(packet + 16);
    vif[2] = 0x10000000u;
    vif[3] = 0x50000003u;
    vif[0] = 0;
    vif[1] = 0;

    unsigned long long* gif = reinterpret_cast<unsigned long long*>(packet + 32);
    gif[1] = 14;
    gif[0] = 0x1000000000008002ull;

    unsigned long long scissor =
        static_cast<unsigned long long>(static_cast<long long>(x0)) |
        static_cast<unsigned long long>(static_cast<long long>(x1)) << 16 |
        static_cast<unsigned long long>(static_cast<long long>(y0)) << 32 |
        static_cast<unsigned long long>(static_cast<long long>(y1)) << 48;
    unsigned long long* first = reinterpret_cast<unsigned long long*>(packet + 48);
    first[1] = 64;
    first[0] = scissor;
    unsigned long long* second = reinterpret_cast<unsigned long long*>(packet + 64);
    second[1] = 65;
    second[0] = scissor;
    return 0;
}
}
