// Observed getBestSlotName__FPc at 0x0025ba20, 228 bytes, in the pinned SLUS_205.65.
// Writes into the given buffer a save slot name made of the string "BASLUS-20565" (read
// from the image's string data), "_slot" and the lowest slot number 0..63 not yet used by
// the sorted directory (sortedDir, 64-byte records at 0x007f7d40, count in
// sortedDirCount). A record's name text starts 45 bytes into the record; it is copied to a
// scratch buffer and read with sscanf(text, "slot%d", &number). The number is used when
// it is equal to the candidate slot. If every number 0..63 is taken, the name is built
// with 64. The result is written with sprintf(out, "%s_slot%d", "BASLUS-20565", slot).
// Inferred: the 45-byte offset is 33 plus 12 as the image computes it; the scratch buffer
// is 256 bytes here (the image keeps stack room for it). When a record's text does not
// match, the previous number stays in the variable, as in the image.
// Not executed on hardware; the libc callees and the directory are defined elsewhere.
extern "C" {
extern char sortedDir[];
extern int sortedDirCount;
char* strcpy(char* dest, const char* src);
int sscanf(const char* input, const char* format, ...);
int sprintf(char* out, const char* format, ...);

void getBestSlotName__FPc(char* out) {
    char text[256];
    int number;
    int slot = 0;
    for (;;) {
        int index = 0;
        const char* record = sortedDir + 45;
        for (; index < sortedDirCount; ++index, record += 64) {
            strcpy(text, record);
            sscanf(text, "slot%d", &number);
            if (number == slot)
                break;
        }
        if (index == sortedDirCount)
            break;
        ++slot;
        if (slot >= 64)
            break;
    }
    sprintf(out, "%s_slot%d", "BASLUS-20565", slot);
}
}
