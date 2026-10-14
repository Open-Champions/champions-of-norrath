// Observed: MC_SaveLevelEntities__FPc at 0x002507b0 is 468 bytes. It returns 0 on every path.
// It returns at once when asyncMode is 3 or when levelFlags has bit 0x800 set. The original also
// reads the coprocessor-0 count register once near the start and never uses the value; that
// read is not reproduced here.
// The size to record is 0x88b8, unless gameOver (byte) is zero. Then getSaveTableEntry__FPc(name)
// is called: -1 returns; otherwise the size is word 1 of the 8-byte entry allActs[curAct] +
// 8 * index.
// A table of 24-byte entries starts at ramSlot + 0x1b610 (0x00839850). Entry bytes 0 to 19 hold a
// name and word 20 a size. The walk runs index 0 to 25, accumulating the size words, and stops
// early at an entry whose first byte is zero or whose name is strcmp-equal to the argument. If
// the entry reached (index 26 when the walk runs out, which lies past the table) has a zero first
// byte, the name is strcpy'd to it and the size stored at +20. The entry's size word is then
// added to the accumulated sum plus 0x1b898; when that exceeds 0xb29c8 (unsigned) nothing more
// happens (the original then scans the table for an empty entry and discards the result).
// Otherwise saveLevelDataASync__FPci(ramSlot + 0x1b898 + sum, entry size) runs.
// Inferred: word 1 of an allActs entry is the byte size of that level's entity data, as it is
// the value recorded in the table; the table is a directory of per-level save blocks.
// Unknown: whether the index-26 case is reachable, and the meaning of the c0_count read.

extern "C" {
extern int asyncMode;
extern int levelFlags;
extern unsigned char gameOver;
extern int curAct;
extern unsigned char* allActs[];
extern unsigned char ramSlot[];
int getSaveTableEntry__FPc(char* name);
int strcmp(const char* left, const char* right);
char* strcpy(char* destination, const char* source);
int saveLevelDataASync__FPci(char* destination, int size);
}

extern "C" int MC_SaveLevelEntities__FPc(char* name) {
    if (asyncMode == 3) {
        return 0;
    }
    if ((levelFlags & 0x800) != 0) {
        return 0;
    }
    int size = 0x88b8;
    if (gameOver == 0) {
        const int index = getSaveTableEntry__FPc(name);
        if (index == -1) {
            return 0;
        }
        size = *reinterpret_cast<int*>(allActs[curAct] + index * 8 + 4);
    }
    unsigned char* table = ramSlot + 0x1b610;
    int sum = 0;
    int i = 0;
    for (; i < 26; ++i) {
        unsigned char* entry = table + i * 24;
        if (*reinterpret_cast<signed char*>(entry) == 0) {
            break;
        }
        if (strcmp(reinterpret_cast<char*>(entry), name) == 0) {
            break;
        }
        sum += *reinterpret_cast<int*>(entry + 20);
    }
    unsigned char* entry = table + i * 24;
    if (*reinterpret_cast<signed char*>(entry) == 0) {
        strcpy(reinterpret_cast<char*>(entry), name);
        *reinterpret_cast<int*>(entry + 20) = size;
    }
    const int entry_size = *reinterpret_cast<int*>(entry + 20);
    if (0xb29c8u < static_cast<unsigned>(0x1b898 + sum + entry_size)) {
        return 0;
    }
    saveLevelDataASync__FPci(reinterpret_cast<char*>(ramSlot + 0x1b898 + sum), entry_size);
    return 0;
}
