// Observed: getPackChecksum__Fv at 0x0025a7f8 is 612 bytes and returns a 16-bit value
// (every update re-extends the low halfword; the result register holds it
// sign-extended). It folds three regions of ramSlot into one running short sum:
// 1. n = GetSaveFootPrint__6Camera(gameCam) + jumpGateSaveFootPrint__Fv() + 28102
//    bytes from ramSlot + 112: sum += (signed byte) - index, for index from 0.
// 2. The next region starts at ramSlot + 112 + n + 71994 (three additions of 23998),
//    moved up one byte at a time until its offset from ramSlot is a multiple of 4.
//    Its length is 36 * (k + 1), where k counts the records of 36 bytes, from that
//    start, whose first byte is nonzero before the first zero one. It is folded the
//    same way as region 1, index from 0.
// 3. The directory of 26 entries of 24 bytes at ramSlot + 0x1b610, as in
//    MC_LoadLevelEntities__FPcT0: while the entry's first byte is nonzero (at most
//    26 entries), each name byte contributes index * (signed byte) (the length is
//    recomputed with strlen each step), then the halfword at entry offset 20 is
//    subtracted and the word at offset 20 is added to a running size. Then that size
//    of bytes from ramSlot + 0x1b898 is folded as sum += index * (signed byte).
// Inferred: this is a checksum of the saved-pack region the memory-card code writes,
// region 3 being the saved-level directory and its blocks; the two footprint calls
// give the lengths of the camera and jump-gate blocks.
// Unknown: what regions 1 and 2 hold, why 71994 is added, and the record meaning of
// the 36-byte stride.
extern "C" {
extern unsigned char ramSlot[];
extern unsigned char* gameCam;
int GetSaveFootPrint__6Camera(void* camera);
int jumpGateSaveFootPrint__Fv();
unsigned strlen(const char* text);

short getPackChecksum__Fv() {
    short sum = 0;
    int length = GetSaveFootPrint__6Camera(gameCam);
    length += jumpGateSaveFootPrint__Fv();
    length += 28102;
    const unsigned char* region = ramSlot + 112;
    for (int index = 0; index < length; ++index) {
        sum = static_cast<short>(sum + (static_cast<signed char>(region[index]) - index));
    }
    for (int round = 0; round < 3; ++round) length += 23998;
    region += length;
    while (((region - ramSlot) & 3) != 0) ++region;
    int span = 36;
    const unsigned char* scan = region;
    if (static_cast<signed char>(*scan) != 0) {
        do {
            scan += 36;
            span += 36;
        } while (static_cast<signed char>(*scan) != 0);
    }
    for (int index = 0; index < span; ++index) {
        sum = static_cast<short>(sum + (static_cast<signed char>(region[index]) - index));
    }
    const unsigned char* directory = ramSlot + 0x1b610;
    int total = 0;
    if (static_cast<signed char>(directory[0]) != 0) {
        int entry = 0;
        for (;;) {
            const unsigned char* name = directory + 24 * entry;
            for (int index = 0; index < static_cast<int>(strlen(reinterpret_cast<const char*>(name))); ++index) {
                sum = static_cast<short>(index * static_cast<signed char>(name[index]) + sum);
            }
            sum = static_cast<short>(sum - *reinterpret_cast<const unsigned short*>(name + 20));
            total += *reinterpret_cast<const int*>(name + 20);
            ++entry;
            if (entry >= 26) break;
            if (static_cast<signed char>(directory[24 * entry]) == 0) break;
        }
    }
    const unsigned char* blocks = ramSlot + 0x1b898;
    for (int index = 0; index < total; ++index) {
        sum = static_cast<short>(index * static_cast<signed char>(blocks[index]) + sum);
    }
    return sum;
}
}
