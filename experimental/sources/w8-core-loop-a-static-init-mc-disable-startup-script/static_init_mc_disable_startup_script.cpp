// Observed _GLOBAL_$I$mcDisableStartupScript at 0x0025c018, 32 bytes. It calls the file-local helper at
// 0x0025ab70 (208 bytes) with arguments 1 and 0xffff. The helper does nothing unless those are its arguments.
// For them it walks tempPlayerStruct (0x007e05e0; the gap to sortedPlayerRace is 4 records of 23984 bytes,
// the same stride gameCheckPlayerAlive uses for the player array). Each record is taken from offset 328 as
// three groups of 4616 bytes, and each group as 64 entries of 72 bytes. Per entry it writes:
//   halfword -1 at +36, +44, +52, +60; byte -1 at +32 and +33;
//   halfword 0 at +16, +18, +20, +22, +24, +26, +28, +30.
// The 72-byte, 64-entry groups match the item copies that cleanUp__Fv reads from player inventories.
// Inferred: the writes are an inlined constructor of a 72-byte element type, for 4 x 3 x 64 elements.
// Unknown: the element type, the meaning of the fields, the 328-byte header before the groups, and the bytes
//   of each entry that the constructor leaves alone (they are not touched here).
extern "C" {
extern unsigned char tempPlayerStruct[4 * 23984];
}

namespace {
void store_half(unsigned char* where, short value) {
    where[0] = static_cast<unsigned char>(value & 0xff);
    where[1] = static_cast<unsigned char>((value >> 8) & 0xff);
}

void construct_entry(unsigned char* entry) {
    store_half(entry + 60, -1);
    store_half(entry + 52, -1);
    store_half(entry + 44, -1);
    store_half(entry + 36, -1);
    entry[32] = 0xff;
    entry[33] = 0xff;
    for (int i = 0; i < 4; ++i) {
        store_half(entry + 16 + 2 * i, 0);
        store_half(entry + 24 + 2 * i, 0);
    }
}

void initialize(int phase, int mask) {
    if (mask != 0xffff || phase != 1) return;
    for (int player = 0; player < 4; ++player) {
        unsigned char* group = tempPlayerStruct + player * 23984 + 328;
        for (int g = 0; g < 3; ++g, group += 4616) {
            for (int e = 0; e < 64; ++e) construct_entry(group + e * 72);
        }
    }
}
}

extern "C" void _GLOBAL_$I$mcDisableStartupScript() { initialize(1, 0xffff); }
