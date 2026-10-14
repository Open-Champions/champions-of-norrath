// Observed picProcess__4itemv at 0x001005a8, 928 bytes (a static member of item).
// Storage used, with its ELF names: _4item$picsToLoad (byte, 0x00483200),
// _4item$picInProgress (word, 0x00483204), _4item$picLoadFinished (byte,
// 0x00483208), drawFrameNm (word), _4item$picList (64 records of 192 bytes,
// 0x004d7080), _4item$picLoadBuffer (0x004da080) and _4item$picBuffer (256 slots
// of 1280 bytes, 0x00487080). Record fields touched:
//   +0 byte (zero means the record is unused), +64 a halfword list, +124 a word,
//   +128 a 64-byte block with halfword +6 and +10, and words +16 and +20.
// Nothing happens unless picsToLoad is nonzero. Otherwise, in order:
// 1. With picInProgress >= 0, drawFrameNm is stored at +124 of that record.
// 2. When picLoadFinished is nonzero, the 64 bytes at the start of picLoadBuffer
//    are copied to +128 of the in-progress record, drawFrameNm is stored in the
//    word just below the block (+124), and the halfword at block +6 (a quadword
//    count) is split into chunks of at most 79 quadwords. Each chunk takes the
//    first slot of picBuffer whose first byte is zero; if none of the 256 is
//    free, picKickOneOut__4itemv is called and the search repeats while it
//    returns nonzero. A chunk slot gets its index appended to the halfword list at
//    +64, is linked (the first one into block word +20, later ones into word 1
//    of the previous slot), gets a first word of the chunk quadword count with
//    0x20000000 (more chunks follow) or 0x60000000 (last chunk), and receives the
//    chunk's bytes at slot +16 from picLoadBuffer + block word +16, advancing in
//    steps of the chunk size times 16. If the search fails, the first byte of
//    every slot assigned so far is zeroed, the first list entry is set to -1 and
//    the routine returns with the state unchanged. (When no slot had been assigned
//    the original stores that -1 through a register this routine does not set; the
//    list entry is used here, which is inferred intent, not observed behaviour.)
//    When all chunks are assigned the list gets a terminating -1, block halfword
//    +10 gets 0xa7fb, picInProgress becomes 0xfff0bdc0, block word +16 becomes 0 and
//    picLoadFinished is cleared. A zero count skips the chunks but runs the rest.
//    A negative picInProgress here would index before the table; the original
//    does the same.
// 3. If cdIsBusy__Fv is nonzero the routine returns. Otherwise picsToLoad is
//    cleared and the records are scanned in order: a record is skipped when its
//    first byte is zero or its halfword at +64 is not -1. For the first other
//    record picFind__4itemPc is called with the record. A null result stores -2 in
//    that halfword and the scan continues. Otherwise cdLoadAsync__FPcPvT0ii starts
//    reading "piccache.dat" into picLoadBuffer, with picLoadFinished as the
//    completion byte, and the two arguments are the result's halfwords at +4 and
//    +2 each shifted left by 11 (in that order); picsToLoad is set to 1,
//    picInProgress to the record number and +124 of the record to drawFrameNm.
//    The scan then stops.
// The meaning of the fields beyond that is not established. Pointers stored in
// record and slot words are the low 32 bits of the address.
extern "C" {
extern signed char _4item$picsToLoad;
extern int _4item$picInProgress;
extern signed char _4item$picLoadFinished;
extern int drawFrameNm;
extern unsigned char _4item$picList[];
extern unsigned char _4item$picLoadBuffer[];
extern unsigned char _4item$picBuffer[];
int picKickOneOut__4itemv();
const unsigned char* picFind__4itemPc(const char* name);
void cdLoadAsync__FPcPvT0ii(const char* file, void* buffer, void* done, int a, int b);
int cdIsBusy__Fv();
void* memcpy(void* dest, const void* src, unsigned size);
}

struct PicBlock {
    unsigned char bytes[64];
};

static unsigned address_word(const void* p) {
    return static_cast<unsigned>(reinterpret_cast<unsigned long>(p));
}

extern "C" void picProcess__4itemv() {
    if (_4item$picsToLoad == 0) return;
    const int current = _4item$picInProgress;
    unsigned char* record = _4item$picList + current * 192;
    if (current >= 0) *reinterpret_cast<int*>(record + 124) = drawFrameNm;
    if (_4item$picLoadFinished != 0) {
        unsigned char* block = record + 128;
        *reinterpret_cast<PicBlock*>(block) = *reinterpret_cast<const PicBlock*>(_4item$picLoadBuffer);
        *reinterpret_cast<int*>(block - 4) = drawFrameNm;
        unsigned remaining = *reinterpret_cast<unsigned short*>(block + 6);
        const unsigned char* source = _4item$picLoadBuffer + *reinterpret_cast<int*>(block + 16);
        short* list = reinterpret_cast<short*>(record + 64);
        int assigned = 0;
        unsigned* link = 0;
        if (remaining != 0) {
            do {
                unsigned chunk = remaining < 80 ? remaining : 79;
                remaining -= chunk;
                int slot_index;
                for (;;) {
                    slot_index = 0;
                    if (_4item$picBuffer[0] != 0) {
                        slot_index = 1;
                        while (slot_index < 256) {
                            if (_4item$picBuffer[slot_index * 1280] == 0) break;
                            ++slot_index;
                        }
                    }
                    if (slot_index != 256) break;
                    if (picKickOneOut__4itemv() == 0) {
                        for (int i = 0; i < assigned; ++i) _4item$picBuffer[list[i] * 1280] = 0;
                        list[0] = -1;
                        return;
                    }
                }
                unsigned char* slot = _4item$picBuffer + slot_index * 1280;
                list[assigned] = static_cast<short>(slot_index);
                ++assigned;
                if (link == 0) {
                    *reinterpret_cast<unsigned*>(block + 20) = address_word(slot);
                } else {
                    *link = address_word(slot);
                }
                *reinterpret_cast<unsigned*>(slot) = chunk | (remaining != 0 ? 0x20000000u : 0x60000000u);
                link = reinterpret_cast<unsigned*>(slot + 4);
                memcpy(slot + 16, source, chunk * 16);
                source += chunk * 16;
            } while (remaining != 0);
        }
        list[assigned] = -1;
        *reinterpret_cast<unsigned short*>(block + 10) = 0xa7fb;
        _4item$picInProgress = static_cast<int>(0xfff0bdc0u);
        *reinterpret_cast<int*>(block + 16) = 0;
        _4item$picLoadFinished = 0;
    }
    if (cdIsBusy__Fv() != 0) return;
    _4item$picsToLoad = 0;
    for (int i = 0; i < 64; ++i) {
        unsigned char* candidate = _4item$picList + i * 192;
        if (candidate[0] == 0) continue;
        short* state = reinterpret_cast<short*>(candidate + 64);
        if (*state != -1) continue;
        const unsigned char* found = picFind__4itemPc(reinterpret_cast<const char*>(candidate));
        if (found == 0) {
            *state = -2;
            continue;
        }
        cdLoadAsync__FPcPvT0ii("piccache.dat", _4item$picLoadBuffer, &_4item$picLoadFinished,
                               *reinterpret_cast<const unsigned short*>(found + 4) << 11,
                               *reinterpret_cast<const unsigned short*>(found + 2) << 11);
        _4item$picsToLoad = 1;
        _4item$picInProgress = i;
        *reinterpret_cast<int*>(candidate + 124) = drawFrameNm;
        return;
    }
}
