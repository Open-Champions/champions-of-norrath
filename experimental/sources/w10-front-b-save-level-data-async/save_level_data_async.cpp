// Observed: saveLevelDataASync__FPci at 0x0025b5a8 is 160 bytes, returns nothing, and is a static
// (file-local) function. With (destination, size) it:
//   1. calls gameGetFullLevelName__FPc into a 256-byte stack buffer; the name is not read again.
//   2. computes room = size - map::autoMapBitsSize (word at 0x00483e88) - 24.
//   3. calls objectGetSaveData__FPci(destination + 8, room), keeps its result in a file-local
//      word (symbol bufferBytes, 0x00486c7c, read only here) and copies that word, byte-wise
//      unaligned, to destination[0..3].
//   4. room -= result; p = destination + result; calls floorGetSaveInfo__FPci(p + 16, room + 8);
//      the result goes unaligned to p[8..11] and p advances by 8 plus that result.
//   5. calls map::writeMap(uchar*) with p + 8 (for the advanced p).
// Offsets seen: word A at +0, object data requested at +8, word B at +A+8, floor info requested
// at +A+16, automap bits at +A+B+16 (B counted from +A+8).
// Inferred: A and B are byte counts of the preceding data, from their use as advances.
// Unknown: the contents of the first 8 bytes at destination+A and what is stored at +4..+7 of
// destination (nothing is written there here).

extern "C" {
extern int norrath_map_auto_map_bits_size __asm__("_3map$autoMapBitsSize");
void gameGetFullLevelName__FPc(char* buffer);
int objectGetSaveData__FPci(char* destination, int room);
int floorGetSaveInfo__FPci(char* destination, int room);
void writeMap__3mapPUc(unsigned char* destination);
}

struct UnalignedWord {
    int value;
} __attribute__((packed));

extern "C" void saveLevelDataASync__FPci(char* destination, int size) {
    static volatile int bufferBytes;
    char name[256];
    gameGetFullLevelName__FPc(name);
    int room = size - norrath_map_auto_map_bits_size - 24;
    bufferBytes = objectGetSaveData__FPci(destination + 8, room);
    reinterpret_cast<UnalignedWord*>(destination)->value = bufferBytes;
    room -= bufferBytes;
    char* p = destination + bufferBytes;
    volatile int info = floorGetSaveInfo__FPci(p + 16, room + 8);
    p += 8;
    reinterpret_cast<UnalignedWord*>(p)->value = info;
    p += info;
    writeMap__3mapPUc(reinterpret_cast<unsigned char*>(p + 8));
}
