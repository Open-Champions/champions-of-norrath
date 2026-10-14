// Observed megatileEntitize__FPci at 0x00268f58, 1668 bytes. Arguments: a pointer
// to a megatile object block and a flag (a bool in the callers' sense) that makes
// every creation ask userFilterFunction first. Returns nothing. Nothing here was
// executed on the EE.
//
// Block layout as read: a signed half count at +0, a half flags word at +2, a
// word at +4 that is an offset from the block to a relocation base, and records
// from +8. Each record: a name pointer word at +0, a signed half at +4 (offset
// from this record to the next), a signed half at +6 (type), three floats at +8,
// +12, +16 (position), and from +20 a zero-terminated list of argument pointers.
//
// Steps:
// 1. Resets five counters (kittencount, levsuffix, pmlistptrsused, pmlistcharsused,
//    dynlevelcount) to zero and writes nine words into lightStart: 32, 32, 32,
//    68, 68, 68, 0, -1.0f, 1.0f.
// 2. Flag bit 2 of the block is remembered and then set. When it was clear, the
//    name word and every argument word of each record are relocated by adding the
//    block base (block + the word at +4); when it was already set they are left.
// 3. For each record in order:
//    - a name containing "zzTileType" or "EdgeMarker" is dropped;
//    - a name containing "Directional_Light", "Ambient_Light" or "clearColor", or
//      equal to "customVars", is created at once: with the filter flag set and
//      userFilterFunction null, or returning zero for the name, it is dropped;
//      otherwise objectCreateByName(name, position copy, type half, arguments);
//    - any other name is placed by tile: x = floor(posX / 720), y = floor(posY /
//      720) (ints). The 10-byte rows of mt_tiledata are scanned until a row whose
//      signed byte at +2 is -1 or 32 rows are done; a row matches when
//      row[0] <= x < row[0] + row[6] and row[1] <= y < row[1] + row[5], and then
//      doEntityDispatch(record, row index, delayed records, &delayed count, filter)
//      is called. Delayed records are 28 bytes each in a local block of 16.
// 4. For each delayed record: specialparse(arguments at +20, the signed byte at
//    +24, 1, the 12-byte point at +0, name at +12) returns an argument list; with
//    the filter set the same userFilterFunction test applies; otherwise
//    objectCreateByName(name, copy of the point, half at +16, that list).
// 5. When the word mgrg points to has a half at +50 equal to exactly 1 and the
//    first float of mt_ebx is not -2.0: groups of mt_ebx floats ended by -1.0 are
//    turned into one "Line" object each. For a group of n floats it queues, in
//    pmlistptrs, the pm strings "name=barrier", "continous=1", "norotate=1",
//    "pcount=-n" and, for each float i with the matching mt_eby float,
//    sprintf("p=%f,%f,0", dx, dy) where dx = (mt_ebx[i] - 12.0 + 6 *
//    instmap_bounds[0]) * 30.0 * 4.0 (as doubles) and dy likewise from mt_eby and
//    instmap_bounds[1]. The queue then goes to addpmlistptrscap and
//    objectCreateByName("Line", uninitialised point, 0, that list) is called. The
//    walk ends at the group whose next mt_ebx float is -2.0.
// 6. objectProcessDeleteList().
// Inferred: the 720 divisor is the tile size; the 32 rows are map regions.
// Unknown: the units of the lightStart words, what mgrg's +50 half selects, and
// the meaning of the strings beyond their use as pm-list entries.
typedef unsigned long long Double;
typedef int __attribute__((may_alias)) AliasedInt;
typedef unsigned __attribute__((may_alias)) AliasedUnsigned;
typedef short __attribute__((may_alias)) AliasedShort;
typedef unsigned short __attribute__((may_alias)) AliasedUShort;
typedef float __attribute__((may_alias)) AliasedFloat;

struct Point3 {
    float x, y, z;
};

extern "C" {
extern int kittencount;
extern int levsuffix;
extern int pmlistptrsused;
extern int pmlistcharsused;
extern int dynlevelcount;
extern int lightStart[9];
extern signed char mt_tiledata[];
extern float mt_ebx[];
extern float mt_eby[];
extern int instmap_bounds[];
extern char* pmlistptrs[];
extern int (*userFilterFunction)(char*);
extern unsigned char* mgrg;

char* strstr(const char*, const char*);
int strcmp(const char*, const char*);
int sprintf(char*, const char*, ...);
float floorf(float);
Double fptodp(float);
char* addpmlist__FPc(char*);
char** addpmlistptrscap__Fv();
char** specialparse__FPPciiG6Point3Pc(char** arguments, int selector, int mode, const Point3* point, char* name);
void doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(unsigned char* record, int index, unsigned char* delayed, int* count, bool filter);
void* objectCreateByName__FPcG6Point3sPPc(char* name, const Point3* point, short type, char** arguments);
void objectProcessDeleteList__Fv();
}

namespace {
inline char* pointerFrom(unsigned word) { return reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(word)); }
inline unsigned wordOf(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }
inline bool allowed(char* name) {
    return userFilterFunction != nullptr && userFilterFunction(name) != 0;
}
}

extern "C" void megatileEntitize__FPci(unsigned char* block, int filter) {
    kittencount = 0;
    levsuffix = 0;
    pmlistptrsused = 0;
    pmlistcharsused = 0;
    dynlevelcount = 0;
    lightStart[0] = 32;
    lightStart[1] = 32;
    lightStart[2] = 32;
    lightStart[3] = 68;
    lightStart[4] = 68;
    lightStart[5] = 68;
    lightStart[6] = 0;
    lightStart[7] = static_cast<int>(0xbf800000u);
    lightStart[8] = 0x3f800000;

    unsigned char* cursor = block + 8;
    const unsigned flags = *reinterpret_cast<AliasedUShort*>(block + 2);
    const unsigned alreadyRelocated = flags & 2u;
    const unsigned base = wordOf(block) + *reinterpret_cast<AliasedUnsigned*>(block + 4);
    const int count = *reinterpret_cast<AliasedShort*>(block);
    *reinterpret_cast<AliasedUShort*>(block + 2) = static_cast<unsigned short>(flags | 2u);

    unsigned char delayed[448];
    int delayedCount = 0;
    const float tile = __builtin_bit_cast(float, 0x3ab60b60u);

    for (int left = count; left > 0; --left) {
        unsigned char* record = cursor;
        cursor = record + *reinterpret_cast<AliasedShort*>(record + 4);
        if (!alreadyRelocated) *reinterpret_cast<AliasedUnsigned*>(record) += base;
        unsigned* argument = reinterpret_cast<unsigned*>(record + 20);
        if (*argument != 0) {
            for (unsigned* a = argument;; ++a) {
                if (!alreadyRelocated) *a += base;
                if (a[1] == 0) break;
            }
        }
        char* name = pointerFrom(*reinterpret_cast<AliasedUnsigned*>(record));
        if (strstr(name, "zzTileType") != nullptr) continue;
        if (strstr(name, "EdgeMarker") != nullptr) continue;
        const bool immediate = strstr(name, "Directional_Light") != nullptr || strstr(name, "Ambient_Light") != nullptr ||
                               strstr(name, "clearColor") != nullptr || strcmp(name, "customVars") == 0;
        if (immediate) {
            Point3 position;
            position.x = *reinterpret_cast<AliasedFloat*>(record + 8);
            position.y = *reinterpret_cast<AliasedFloat*>(record + 12);
            position.z = *reinterpret_cast<AliasedFloat*>(record + 16);
            if (filter != 0 && !allowed(name)) continue;
            objectCreateByName__FPcG6Point3sPPc(name, &position, *reinterpret_cast<AliasedShort*>(record + 6),
                                                reinterpret_cast<char**>(argument));
            continue;
        }
        const int tileX = static_cast<int>(floorf(*reinterpret_cast<AliasedFloat*>(record + 8) * tile));
        const int tileY = static_cast<int>(floorf(*reinterpret_cast<AliasedFloat*>(record + 12) * tile));
        if (mt_tiledata[2] == -1) continue;
        const signed char* row = mt_tiledata;
        int index = 0;
        for (;;) {
            if (tileX >= row[0] && tileY >= row[1] && tileX < row[0] + row[6] && tileY < row[1] + row[5]) {
                doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(record, index, delayed, &delayedCount, filter != 0);
            }
            ++index;
            row += 10;
            if (index >= 32) break;
            if (row[2] == -1) break;
        }
    }

    for (int i = 0; i < delayedCount; ++i) {
        unsigned char* entry = delayed + 28 * i;
        Point3 point;
        __builtin_memcpy(&point, entry, 12);
        char* name = pointerFrom(*reinterpret_cast<AliasedUnsigned*>(entry + 12));
        char** arguments = specialparse__FPPciiG6Point3Pc(
            reinterpret_cast<char**>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<AliasedUnsigned*>(entry + 20))),
            *reinterpret_cast<signed char*>(entry + 24), 1, &point, name);
        if (filter != 0 && !allowed(name)) continue;
        Point3 created;
        __builtin_memcpy(&created, entry, 12);
        objectCreateByName__FPcG6Point3sPPc(name, &created, *reinterpret_cast<AliasedShort*>(entry + 16), arguments);
    }

    if (*reinterpret_cast<AliasedShort*>(mgrg + 50) == 1 && mt_ebx[0] != -2.0f) {
        int first = 0;
        const float* ebx = mt_ebx;
        int step = 0;
        char buffer[64];
        do {
            int last = first;
            char* className = addpmlist__FPc(const_cast<char*>("Line"));
            if (*ebx != -1.0f) {
                const float* scan = mt_ebx + first;
                do {
                    ++scan;
                    ++last;
                } while (*scan != -1.0f);
            }
            char* barrier = addpmlist__FPc(const_cast<char*>("name=barrier"));
            pmlistptrs[pmlistptrsused++] = barrier;
            char* continuous = addpmlist__FPc(const_cast<char*>("continous=1"));
            pmlistptrs[pmlistptrsused++] = continuous;
            char* norotate = addpmlist__FPc(const_cast<char*>("norotate=1"));
            pmlistptrs[pmlistptrsused++] = norotate;
            sprintf(buffer, "pcount=-%d", last - first);
            char* pcount = addpmlist__FPc(buffer);
            pmlistptrs[pmlistptrsused++] = pcount;
            while (first < last) {
                ++first;
                const Double dx = fptodp(((*ebx - 12.0f) + static_cast<float>(instmap_bounds[0] * 6)) * 30.0f * 4.0f);
                ++ebx;
                const Double dy = fptodp(((mt_eby[step] - 12.0f) + static_cast<float>(instmap_bounds[1] * 6)) * 30.0f * 4.0f);
                ++step;
                sprintf(buffer, "p=%f,%f,0", dx, dy);
                char* point = addpmlist__FPc(buffer);
                pmlistptrs[pmlistptrsused++] = point;
            }
            char** arguments = addpmlistptrscap__Fv();
            ++ebx;
            Point3 origin;
            objectCreateByName__FPcG6Point3sPPc(className, &origin, 0, arguments);
            ++step;
            ++first;
        } while (*ebx != -2.0f);
    }
    objectProcessDeleteList__Fv();
}
