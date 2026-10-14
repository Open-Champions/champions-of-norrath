// Observed playerRecallSpell__Fv at 0x001a4e78, 1244 bytes (pinned SLUS_205.65).
// Per-frame step of the recall effect that debugRecall__Fi starts by setting
// the byte recallOn to 60. Nothing happens while recallOn is zero.
//  1. Frame 60 (first frame): for each of the first nmPlayers slots (count read
//     once) haloColor[i] = {100, 200, 255}, castLightColor[i] = {86, 126, 250,
//     -96}, lightIntensity[i] = 0 and lightInc[i] = the float 0x3d4ccccc
//     (about 0.05). Words, one 16-byte record per slot in both color tables.
//  2. recallOn is decremented. While it is still nonzero the routine ends.
//  3. When it reaches zero: for each slot, lightIntensity[i] and lightInc[i]
//     are zeroed and, if the entity pointer at record+21872 is set,
//     lightDeactivate(entity + 1268) is called (nmPlayers is re-read each
//     pass). The routine ends here when cutMode is nonzero or nmPlayersAlive
//     is zero.
//  4. If the string at levelName+81 is one of "kelethin", "gnomehub",
//     "beachhub", "elfhub" or "airhub" (tested in that order with strcmp):
//     playerRecalled = 1, the 12 bytes at record+68 of each slot are copied
//     to recallPos[i] (12-byte entries, count read once), and levelName is
//     set from the string at record 0 + 36 with strcpy.
//  5. Otherwise, for each slot: when the entity pointer at record+21872 is set
//     and the float at record+172 is above zero, the 12 bytes at entity+16
//     replace those at record+68. In every other case the 12 bytes at
//     entity+16 of the first alive player (playersAlive[0]) are passed to
//     objectFindEmptySpot(point, 72, 24, 0) and the returned point replaces
//     record+68. Then gameGetFullLevelName(record+36) is called for the slot.
//     After the loop recallEnter = 1 and the first of these flag tests that
//     passes on the word levelFlags copies a name to levelName: bit 0x20000
//     "kelethin", 0x10000 "gnomehub", 0x8000 "beachhub", 0x400 "elfhub",
//     0x200 "airhub".
//  6. In both the hub and the other case the signed byte levelName[80]
//     is then set to -3.
// Inferred: record+68 is the player's position, record+172 a health-like
// float, record+36 the name of the level the player was last in, and the
// routine is the arrival half of the recall spell. Unknown: the meaning of
// levelName[80] = -3, of 72 and 24 in the empty-spot call, and whether the
// alive-player lookup in step 5 may be out of range (it is not checked).
// The empty-spot call is declared as in the ant/lantern constructors
// (hidden result pointer, then a pointer to a copy of the point).
extern "C" {
struct Point3 {
    float x, y, z;
};
extern unsigned char player[];
extern int nmPlayers;
extern int nmPlayersAlive;
extern int playersAlive[4];
extern unsigned char recallOn;
extern signed char recallEnter;
extern signed char playerRecalled;
extern signed char cutMode;
extern int levelFlags;
extern char levelName[];
extern Point3 recallPos[4];
extern int haloColor[4][4];
extern int castLightColor[4][4];
extern int lightIntensity[4];
extern float lightInc[4];

int strcmp(const char* a, const char* b);
char* strcpy(char* destination, const char* source);
void lightDeactivate__FP5Light(void* light);
void objectFindEmptySpot__FG6Point3iiPc(void* result, const void* point, int first, int second, char* third);
void gameGetFullLevelName__FPc(char* name);

static float float_from_bits(unsigned bits) {
    union {
        unsigned u;
        float f;
    } v;
    v.u = bits;
    return v.f;
}

static unsigned char* entity_at(const unsigned char* record) {
    return *reinterpret_cast<unsigned char* const*>(record + 21872);
}

static void copy_point(void* destination, const void* source) {
    __builtin_memcpy(destination, source, sizeof(Point3));
}

void playerRecallSpell__Fv() {
    int step = static_cast<signed char>(recallOn);
    if (step == 0) return;
    if (step == 60) {
        int count = nmPlayers;
        for (int i = 0; i < count; ++i) {
            haloColor[i][0] = 100;
            haloColor[i][1] = 200;
            haloColor[i][2] = 255;
            lightIntensity[i] = 0;
            castLightColor[i][0] = 86;
            castLightColor[i][1] = 126;
            castLightColor[i][2] = 250;
            castLightColor[i][3] = -96;
            lightInc[i] = float_from_bits(0x3d4cccccu);
        }
    }
    recallOn = static_cast<unsigned char>(step - 1);
    if (static_cast<signed char>(recallOn) != 0) return;

    for (int i = 0; i < nmPlayers; ++i) {
        unsigned char* entity = entity_at(player + i * 23984);
        lightIntensity[i] = 0;
        lightInc[i] = 0;
        if (entity) lightDeactivate__FP5Light(entity + 1268);
    }
    if (cutMode != 0) return;
    if (nmPlayersAlive == 0) return;

    const char* current = levelName + 81;
    if (strcmp("kelethin", current) == 0 || strcmp("gnomehub", current) == 0 ||
        strcmp("beachhub", current) == 0 || strcmp("elfhub", current) == 0 ||
        strcmp("airhub", current) == 0) {
        playerRecalled = 1;
        int count = nmPlayers;
        for (int i = 0; i < count; ++i) copy_point(&recallPos[i], player + i * 23984 + 68);
        strcpy(levelName, reinterpret_cast<const char*>(player + 36));
    } else {
        for (int i = 0; i < nmPlayers; ++i) {
            unsigned char* record = player + i * 23984;
            unsigned char* entity = entity_at(record);
            float health = 0;
            if (entity) {
                __builtin_memcpy(&health, record + 172, sizeof health);
            }
            if (entity && 0.0f < health) {
                copy_point(record + 68, entity + 16);
            } else {
                Point3 from;
                Point3 spot;
                copy_point(&from, entity_at(player + playersAlive[0] * 23984) + 16);
                objectFindEmptySpot__FG6Point3iiPc(&spot, &from, 72, 24, 0);
                copy_point(record + 68, &spot);
            }
            gameGetFullLevelName__FPc(reinterpret_cast<char*>(record + 36));
        }
        recallEnter = 1;
        if (levelFlags & 0x20000) {
            __builtin_memcpy(levelName, "kelethin", 9);
        } else if (levelFlags & 0x10000) {
            __builtin_memcpy(levelName, "gnomehub", 9);
        } else if (levelFlags & 0x8000) {
            __builtin_memcpy(levelName, "beachhub", 9);
        } else if (levelFlags & 0x400) {
            __builtin_memcpy(levelName, "elfhub", 7);
        } else if (levelFlags & 0x200) {
            __builtin_memcpy(levelName, "airhub", 7);
        }
    }
    levelName[80] = -3;
}
}
