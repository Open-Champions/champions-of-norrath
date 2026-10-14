// Observed engineFrameStart__FUib at 0x00191200, 252 bytes.
// simpleDrawLoop calls it with coreClearColor and 0. The second argument is
// passed through to frameStartPacket; the instructions do not mask it to 0
// or 1. Fresh compilation is not an original-byte match. No EE path has run.
//
// `draw` is the BSS symbol at 0x00790ff0. Word 8 is passed to
// frameStartPacket before FlushCache(0) and drawStart. Those calls return
// before the later reads of words 0, 4, and 8.
//
// A nonzero word 0 copies the reloaded words 8 and 4 to words 20 and 16.
// A zero word 0 stores zero in both. Word 12 becomes 1. Word 0 is stored as
// 0 and then 1 before any further call; the 1 is the value that remains.
//
// gstart and glistp both receive 0x0038f9e0 plus (drawFrameNm & 1) * 0x445c0,
// with bit 0x30000000 set. 0x0038f9e0 is packetBuffer (0x0037f9e0) plus
// 0x10000. The instruction uses the immediate, not a relocation. 0x445c0 is
// not GLISTSIZE; that word is 35000. Naming 0x30000000 as the uncached
// accelerated window is inferred from the public EE map.
//
// fullFrameMode is loaded after those stores. Zero replaces word 8 with the
// sign-extended vblankField byte. Any other value stores 0 there. Word 4
// then receives fullFrameMode.

extern "C" {
extern unsigned draw[];
extern unsigned drawFrameNm;
extern int fullFrameMode;
extern volatile unsigned char vblankField;
extern unsigned gstart;
extern unsigned glistp;

void lightVU0StoppedProcessing__Fv();
void frameStartPacket__FUibi(unsigned color, int second, int word);
void FlushCache(int mode);
void drawStart__Fv();

void engineFrameStart__FUib(unsigned color, int second) {
    lightVU0StoppedProcessing__Fv();
    frameStartPacket__FUibi(color, second, static_cast<int>(draw[2]));
    FlushCache(0);
    drawStart__Fv();

    draw[3] = 1u;
    if (draw[0] != 0u) {
        draw[5] = draw[2];
        draw[4] = draw[1];
    } else {
        draw[5] = 0u;
        draw[4] = 0u;
    }
    const unsigned parity = drawFrameNm & 1u;
    draw[0] = 1u;
    unsigned address = 0x0038f9e0u + parity * 0x000445c0u;
    address |= 0x30000000u;
    gstart = address;
    glistp = address;

    const int mode = fullFrameMode;
    unsigned stored = 0u;
    if (mode == 0) {
        const unsigned byte = vblankField;
        stored = byte;
        if ((byte & 0x80u) != 0u) stored -= 256u;
    }
    draw[2] = stored;
    draw[1] = static_cast<unsigned>(mode);
}
}
