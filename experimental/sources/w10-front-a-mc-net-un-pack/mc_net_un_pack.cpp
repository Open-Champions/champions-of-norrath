// Observed: MC_NetUnPack__Fv at 0x0025a4a0 is 912 bytes (frame 0x8f60). It undoes
// MC_NetPack__Fv on the slot image in ramSlot, returning nothing:
//  1. p = ramSlot + 112 + GetSaveFootPrint__6Camera(gameCam) + jumpGateSaveFootPrint__Fv() +
//     28102 (camera first); q = p + 71994. Both move up to a multiple of four relative to ramSlot;
//  2. the list of 36-byte entries at p, ending with and including the first entry whose first
//     byte is zero, is copied to a local buffer, and p moves past it and up to a multiple of
//     four, then past the int n stored there (n = that int);
//  3. when n is positive, n 24-byte entries from p are copied to a local buffer and their size
//     words (byte 20) are summed into total; p moves past the n entries and up to a multiple of
//     four;
//  4. the total bytes at p are copied from the last to the first to ramSlot + 0x1b898;
//  5. the 648 bytes at ramSlot + 0x1b610 are cleared and the n saved 24-byte entries are copied
//     back to its start;
//  6. the saved list of 36-byte entries is copied to q, up to and including its terminator.
// The first 28102 bytes of the area (the first player entry) stay where they were; the list is
// rebuilt 71994 bytes (three 23998-byte player entries) past the start of the packed data. The
// original reaches q with a three-pass loop that adds 23998 each time; it is written here as
// one addition of 71994.
// Inferred: this restores the layout read by loadRamSlot__Fv from the network form that
// MC_NetPack__Fv produces. The local buffers are the 656 bytes below the list buffer and a
// large list buffer; n is expected to be at most 26 and the list to fit the rest of the frame.
// Unknown: the exact capacity the original frame gives the buffers.

extern "C" {
extern unsigned char ramSlot[];
extern void* gameCam;
int GetSaveFootPrint__6Camera(void* camera);
int jumpGateSaveFootPrint__Fv();
void* memset(void* to, int value, __SIZE_TYPE__ size);
}

namespace {
unsigned char* alignUp(unsigned char* p) {
    while (((p - ramSlot) & 3) != 0) ++p;
    return p;
}
}

extern "C" void MC_NetUnPack__Fv() {
    unsigned char directory[656];
    unsigned char list[35500];
    unsigned char* p = ramSlot + 112 + GetSaveFootPrint__6Camera(gameCam);
    p += jumpGateSaveFootPrint__Fv();
    p += 28102;
    unsigned char* q = p + 71994;
    q = alignUp(q);
    p = alignUp(p);
    unsigned char* out = list;
    for (;;) {
        for (int i = 0; i < 36; ++i) out[i] = p[i];
        bool last = static_cast<signed char>(p[0]) == 0;
        p += 36;
        out += 36;
        if (last) break;
    }
    while ((reinterpret_cast<unsigned long>(p) & 3) != 0) ++p;
    p += 4;
    int n = *reinterpret_cast<int*>(p - 4);
    int total = 0;
    if (n > 0) {
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < 24; ++i) directory[24 * k + i] = p[24 * k + i];
            total += *reinterpret_cast<int*>(directory + 24 * k + 20);
        }
    }
    p += n * 24;
    while ((reinterpret_cast<unsigned long>(p) & 3) != 0) ++p;
    for (int i = total - 1; i >= 0; --i) ramSlot[0x1b898 + i] = p[i];
    memset(ramSlot + 0x1b610, 0, 648);
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < 24; ++i) ramSlot[0x1b610 + 24 * k + i] = directory[24 * k + i];
    }
    unsigned char* in = list;
    for (;;) {
        for (int i = 0; i < 36; ++i) q[i] = in[i];
        bool last = static_cast<signed char>(in[0]) == 0;
        q += 36;
        in += 36;
        if (last) break;
    }
}
