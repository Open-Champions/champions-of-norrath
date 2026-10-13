// Observed: MC_NetPack__Fv at 0x0025a280 is 540 bytes and returns an int. It compacts the saved
// slot image in ramSlot (the layout read by loadRamSlot__Fv) in place:
//  1. c = getPackChecksum__Fv(); the halfword at ramSlot + 110 becomes the low 16 bits of c;
//  2. d = ramSlot + 112 + GetSaveFootPrint__6Camera(gameCam) + jumpGateSaveFootPrint__Fv() +
//     28102 (28102 = 4104 + 23998, the end of the first saved-player entry; the call order is
//     camera first). s = d + 71994 (three more 23998-byte entries). Both are then moved up to a
//     multiple of four relative to ramSlot;
//  3. the list of 36-byte entries at s (script variables, ending with the first entry whose first
//     byte is zero, which is copied too) is copied to d, entry by entry, and d moves past it and
//     up to a multiple of four;
//  4. n counts the leading entries of the 24-byte directory at ramSlot + 0x1b610 whose first byte
//     is nonzero, stopping at 26; the int n is stored at d (d += 4);
//  5. when the first directory entry is used, entries are copied to d (24 bytes each) while
//     their first byte is nonzero, without the stop at 26, and the size words (at byte 20) of the
//     copied entries are summed into total; d moves past the copied entries and up to a
//     multiple of four;
//  6. memcpy(d, ramSlot + 0x1b898, total); d += total. The result is d - ramSlot.
// The original also runs a three-pass counting loop with an empty body before step 2; it has no
// effect and is not reproduced.
// Inferred: this builds the network-game form of the slot image, which keeps the first player
// only (players 1 to 3 are dropped from the image) and the level directory and blocks packed
// behind it; MC_NetUnPack__Fv is the inverse.
// Unknown: what getPackChecksum__Fv computes, and what the two-byte value at ramSlot + 110 is
// compared with.

extern "C" {
extern unsigned char ramSlot[];
extern void* gameCam;
int getPackChecksum__Fv();
int GetSaveFootPrint__6Camera(void* camera);
int jumpGateSaveFootPrint__Fv();
void* memcpy(void* to, const void* from, __SIZE_TYPE__ size);
}

namespace {
unsigned char* alignUp(unsigned char* p) {
    while (((p - ramSlot) & 3) != 0) ++p;
    return p;
}
}

extern "C" int MC_NetPack__Fv() {
    int checksum = getPackChecksum__Fv();
    *reinterpret_cast<unsigned short*>(ramSlot + 110) = static_cast<unsigned short>(checksum);
    unsigned char* d = ramSlot + 112 + GetSaveFootPrint__6Camera(gameCam);
    d += jumpGateSaveFootPrint__Fv();
    d += 28102;
    unsigned char* s = d + 71994;
    s = alignUp(s);
    d = alignUp(d);
    for (;;) {
        for (int i = 0; i < 36; ++i) d[i] = s[i];
        bool last = static_cast<signed char>(s[0]) == 0;
        d += 36;
        if (last) break;
        s += 36;
    }
    d = alignUp(d);
    unsigned char* table = ramSlot + 0x1b610;
    int n = 0;
    if (static_cast<signed char>(table[0]) != 0) {
        n = 1;
        unsigned char* e = table;
        while (n < 26) {
            e += 24;
            if (static_cast<signed char>(e[0]) == 0) break;
            ++n;
        }
    }
    *reinterpret_cast<int*>(d) = n;
    d += 4;
    int total = 0;
    if (static_cast<signed char>(table[0]) != 0) {
        unsigned char* e = table;
        do {
            for (int i = 0; i < 24; ++i) d[i] = e[i];
            d += 24;
            total += *reinterpret_cast<int*>(e + 20);
            e += 24;
        } while (static_cast<signed char>(e[0]) != 0);
    }
    d = alignUp(d);
    memcpy(d, ramSlot + 0x1b898, total);
    d += total;
    return static_cast<int>(d - ramSlot);
}
