// Observed: MC_LoadLevelEntities__FPcT0 at 0x00250988 is 640 bytes. It takes name and full, and
// returns nothing. T is the 26-entry directory of 24-byte entries at ramSlot + 0x1b610 that
// MC_SaveLevelEntities__FPc fills (name in bytes 0 to 19, size word at 20); the saved blocks
// follow it from ramSlot + 0x1b898.
//  1. The directory is walked from entry 0 while the index is below 26 and the entry's first byte
//     is nonzero, stopping at the first entry whose name is strcmp-equal to full; the size
//     words of the entries passed are summed into sum. The entry reached counts as found when its
//     first byte (as a signed byte) is nonzero.
//  2. sprintf(path, "%s.lmp", name); lump = lumpLoad__FPc(path). With a lump, lumpFind__FPvPc
//     (lump, "script.scr") is called; when it is nonzero it is called again and, when the word
//     at byte 8 of that result is nonzero, called a third time and
//     scriptSetScript__FPv(word at byte 4 of the third result) runs. In every other case
//     scriptSetScript__FPv(0) runs.
//  3. When keepPlayerPositions is nonzero it becomes 0 and gotPlayerStartThisLevel becomes 3,
//     else gotPlayerStartThisLevel becomes 0.
//  4. Unless mcDisableStartupScript is nonzero, scriptStart__FPc("preStart") runs.
//  5. When found: with b = ramSlot + 0x1b898 + sum, size1 = the (unaligned) int at b and size2 =
//     the int at b + 8 + size1: size1 bytes from b + 8 are copied to loadBuffer, size2 bytes from
//     b + 16 + size1 to loadBuffer + size1, then readMap__3mapPUc(b + 16 + size1 + size2),
//     objectPutSaveData__FPci(loadBuffer, 0) and floorPutSaveInfo__FPc(loadBuffer + size1) run.
//     (The original reads size1 and size2 before each copy that needs them, from the slot.)
//  6. With a lump: lumpFind__FPvPc(lump, "objects.ob") supplies, as its word at byte 4, the
//     objects; when the word at byte 112 of the structure world points to is nonzero this goes to
//     megatileEntitize__FPci(objects, found), else to objectPutSaveData__FPci(objects, found).
//  7. Unless mcDisableStartupScript is nonzero, scriptStart__FPc("start") runs.
// Inferred: the blocks after the directory hold, per saved level, the object save data, the floor
// save info and the map data, in that order, each led by a size word; found says whether this
// level has saved data at all, and the saved data is applied on top of the freshly loaded lump.
// Unknown: what the 8 bytes before each block's data hold, and the type of world.

extern "C" {
extern unsigned char ramSlot[];
extern unsigned char loadBuffer[];
extern signed char keepPlayerPositions;
extern signed char gotPlayerStartThisLevel;
extern signed char mcDisableStartupScript;
extern unsigned char* world;
int strcmp(const char* left, const char* right);
int sprintf(char* out, const char* format, ...);
void* memcpy(void* to, const void* from, __SIZE_TYPE__ size);
void* lumpLoad__FPc(const char* name);
unsigned char* lumpFind__FPvPc(void* lump, const char* name);
void scriptSetScript__FPv(void* script);
void scriptStart__FPc(const char* name);
void readMap__3mapPUc(unsigned char* data);
void objectPutSaveData__FPci(void* data, int flag);
void floorPutSaveInfo__FPc(void* data);
void megatileEntitize__FPci(void* data, int flag);
}

namespace {
int readInt(const unsigned char* p) {
    int v;
    __builtin_memcpy(&v, p, 4);
    return v;
}
}

extern "C" void MC_LoadLevelEntities__FPcT0(char* name, char* full) {
    unsigned char* table = ramSlot + 0x1b610;
    int sum = 0;
    int i = 0;
    for (; i < 26 && static_cast<signed char>(table[i * 24]) != 0; ++i) {
        if (strcmp(reinterpret_cast<char*>(table + i * 24), full) == 0) break;
        sum += *reinterpret_cast<int*>(table + i * 24 + 20);
    }
    bool found = static_cast<signed char>(table[i * 24]) != 0;
    char path[80];
    sprintf(path, "%s.lmp", name);
    void* lump = lumpLoad__FPc(path);
    if (lump != nullptr && lumpFind__FPvPc(lump, "script.scr") != nullptr &&
        *reinterpret_cast<int*>(lumpFind__FPvPc(lump, "script.scr") + 8) != 0) {
        scriptSetScript__FPv(*reinterpret_cast<void**>(lumpFind__FPvPc(lump, "script.scr") + 4));
    } else {
        scriptSetScript__FPv(nullptr);
    }
    if (keepPlayerPositions) {
        keepPlayerPositions = 0;
        gotPlayerStartThisLevel = 3;
    } else {
        gotPlayerStartThisLevel = 0;
    }
    if (!mcDisableStartupScript) scriptStart__FPc("preStart");
    if (found) {
        unsigned char* b = ramSlot + 0x1b898 + sum;
        int size1 = readInt(b);
        memcpy(loadBuffer, b + 8, size1);
        int size2 = readInt(b + 8 + size1);
        memcpy(loadBuffer + size1, b + 16 + size1, size2);
        readMap__3mapPUc(b + 16 + size1 + size2);
        objectPutSaveData__FPci(loadBuffer, 0);
        floorPutSaveInfo__FPc(loadBuffer + size1);
    }
    if (lump != nullptr) {
        unsigned char* objects = lumpFind__FPvPc(lump, "objects.ob");
        if (*reinterpret_cast<int*>(world + 112) != 0) {
            megatileEntitize__FPci(*reinterpret_cast<void**>(objects + 4), found);
        } else {
            objectPutSaveData__FPci(*reinterpret_cast<void**>(objects + 4), found);
        }
    }
    if (!mcDisableStartupScript) scriptStart__FPc("start");
}
