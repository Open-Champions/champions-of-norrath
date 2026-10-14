// Observed picKickOneOut__4itemv at 0x001004a8, 256 bytes (a file-local member of item).
// Evicts the least recently stamped picture record. _4item$picList is 64 records
// of 192 bytes; byte +0 is zero for an unused record, the word at +124 is a frame
// stamp, and +64 starts a list of halfword picBuffer slot numbers ended by a
// negative value. A record is a candidate when it is in use, is not record
// _4item$picInProgress, and its stamp is below drawFrameNm - 8; among candidates
// the smallest stamp wins (the first one on a tie). With no candidate the result
// is 0. Otherwise the winner's first byte is cleared and, when its first list
// entry is not negative, the first byte of each listed 1280-byte picBuffer slot
// (_4item$picBuffer) is cleared, the last one being the entry read just before
// the negative value. The result is then 1.
// Unknown: whether stamps may wrap; they are compared as signed values.
extern "C" {
extern int _4item$picInProgress;
extern int drawFrameNm;
extern unsigned char _4item$picList[];
extern unsigned char _4item$picBuffer[];
}

extern "C" int picKickOneOut__4itemv() {
    int threshold = drawFrameNm - 8;
    int oldest = -1;
    for (int index = 0; index < 64; ++index) {
        if (index == _4item$picInProgress) continue;
        const unsigned char* record = _4item$picList + index * 192;
        if (static_cast<signed char>(record[0]) == 0) continue;
        const int stamp = *reinterpret_cast<const int*>(record + 124);
        if (stamp < threshold) {
            threshold = stamp;
            oldest = index;
        }
    }
    if (oldest < 0) return 0;
    unsigned char* record = _4item$picList + oldest * 192;
    record[0] = 0;
    const short* list = reinterpret_cast<const short*>(record + 64);
    if (list[0] < 0) return 1;
    int at = 0;
    short next;
    do {
        const int slot = list[at];
        ++at;
        next = list[at];
        _4item$picBuffer[slot * 1280] = 0;
    } while (next >= 0);
    return 1;
}
