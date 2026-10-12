// Observed fontEnd__Fv at 0x00172f88, 256 bytes.
// Finishes the packet opened by fontStart__FP4FontiUii.
// 1. When scissored (0x004869a4) is nonzero, fontScissor__Fiiii(0,
//    engineScissory1, 640, engineScissory2) is called first, which also flushes
//    the pending glyph count.
// 2. The 16-bit field at vifDirectQWordCount (0x00486990) becomes the low 16 bits
//    of nmDirectQWords (0x0048699c) plus twice nmChars (0x00486998).
// 3. The 16-bit loop count of the tag at reglistLoopCount (0x00486994) grows by
//    twice nmChars.
// 4. The 64-bit word at the packet start (p+4 of the object p at 0x006e93d0, the
//    eight bytes reserved by fontStart) is set to 0x10000000 or'd with the
//    sign-extended value ((cursor - start) / 8 - 2) / 2, using arithmetic shifts,
//    where cursor is p+0. When bits 28..30 of that word equal 1, which is
//    always the case for counts below 2^28, the word is then rewritten as
//    (word & 0x8fffffff) | 0x60000000. Otherwise p+4 is cleared, p+0 moves to
//    cursor + 16, and a 16-byte terminator whose first word is 0x60000000 is
//    written at the old cursor. The second branch is kept because the original
//    contains it.
// 5. Unless the byte coreLoadingScreenRunning (0x004866cc) is nonzero, the old
//    value of glistp (0x004844c8) is read, glistp becomes p+0, and
//    drawAddToList__FPviP8_textureP11_drawRecordi is called with the old glistp,
//    drawBucket (0x004869a8), the word at offset 16 of curFont (0x0048698c), a null
//    record and 1. When the byte is nonzero the packet is not queued and glistp
//    is left unchanged.
// The meaning of the bucket and of the font's word at +16 is not established
// here. All objects and the callees are defined elsewhere.
extern "C" {
struct FontPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* first_tag;
};
extern FontPacketState p;
extern unsigned char* glistp;
extern void* curFont;
extern int drawBucket;
extern int nmChars;
extern int scissored;
extern unsigned char* vifDirectQWordCount;
extern unsigned char* reglistLoopCount;
extern int nmDirectQWords;
extern int engineScissory1;
extern int engineScissory2;
extern signed char coreLoadingScreenRunning;
void fontScissor__Fiiii(int x0, int y0, int x1, int y1);
void drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int bucket, void* texture, void* record, int flag);
}

extern "C" void fontEnd__Fv() {
    if (scissored != 0) fontScissor__Fiiii(0, engineScissory1, 640, engineScissory2);
    unsigned char* cursor = p.cursor;
    unsigned char* start = p.start;
    *reinterpret_cast<unsigned short*>(vifDirectQWordCount) =
        static_cast<unsigned short>(static_cast<unsigned short>(nmDirectQWords) + nmChars * 2);
    unsigned short* loops = reinterpret_cast<unsigned short*>(reglistLoopCount);
    *loops = static_cast<unsigned short>(*loops + nmChars * 2);
    int quadwords = (static_cast<int>(cursor - start) >> 3) - 2;
    quadwords >>= 1;
    const long long tag = static_cast<long long>(quadwords) | 0x10000000LL;
    *reinterpret_cast<long long*>(start) = tag;
    if (((tag >> 28) & 7) == 1) {
        *reinterpret_cast<long long*>(start) = (tag & 0x8fffffffLL) | 0x60000000LL;
    } else {
        p.start = 0;
        p.cursor = cursor + 16;
        *reinterpret_cast<long long*>(cursor + 8) = 0;
        *reinterpret_cast<long long*>(cursor) = 0x60000000LL;
    }
    if (coreLoadingScreenRunning == 0) {
        unsigned char* packet = glistp;
        void* texture = *reinterpret_cast<void**>(static_cast<unsigned char*>(curFont) + 16);
        glistp = p.cursor;
        drawAddToList__FPviP8_textureP11_drawRecordi(packet, drawBucket, texture, 0, 1);
    }
}
