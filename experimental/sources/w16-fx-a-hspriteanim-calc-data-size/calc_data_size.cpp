// Observed: CalcDataSize__13VIHSpriteAnim at 0x0105ce40, 136 bytes, no calls. Offsets are bytes from this; F is the signed word at +8, N the
// word at +12, P the word at +16 and Q the word at +20. If F is neither 0 nor 1 it returns -1 and writes nothing. Otherwise it stores
// 0 at +36, N * 8 at +40, that plus N * P * (32 when F is 0, 16 when F is 1) at +44, and that plus Q * 8 at +48, then returns 0.
// All arithmetic is 32-bit and wraps.
// Inferred: the object is one block laid out as three tables whose offsets are written here: +36 an 8-byte record per node (Init__13VIHSpriteAnim
// walks 8-byte records starting at the +36 offset, one per node), +40 and +44 and the end size at +48 (Allocate__13VIHSpriteAnim
// takes +48 as the byte count). N is the node count and P the frames per node; F selects 32-byte or 16-byte frame records.
// Unknown: what Q counts, and the format names of F = 0 and F = 1.
extern "C" {
int CalcDataSize__13VIHSpriteAnim(unsigned char* self) {
    int format = *reinterpret_cast<int*>(self + 8);
    int nodes = *reinterpret_cast<int*>(self + 12);
    int frames = *reinterpret_cast<int*>(self + 16);
    int extra = *reinterpret_cast<int*>(self + 20);
    int frameShift;
    if (format == 0) frameShift = 5;
    else if (format == 1) frameShift = 4;
    else return -1;
    int nodeTable = nodes * 8;
    int frameTable = nodeTable + ((nodes * frames) << frameShift);
    *reinterpret_cast<int*>(self + 40) = nodeTable;
    *reinterpret_cast<int*>(self + 36) = 0;
    *reinterpret_cast<int*>(self + 44) = frameTable;
    *reinterpret_cast<int*>(self + 48) = frameTable + (extra << 3);
    return 0;
}
}
