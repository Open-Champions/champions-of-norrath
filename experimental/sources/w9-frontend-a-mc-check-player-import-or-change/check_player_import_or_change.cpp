// Observed: MC_CheckPlayerImportOrChange__Fv at 0x002586b0 is 2856 bytes. Records of player have
// stride 23984; tempPlayerStruct has the same stride. The record words used: the pointer at byte
// 21872 and the 12-byte point at byte 68, the name string at byte 36, a 4100-byte block at byte
// 17768, and an inventory at byte 9560 whose item count is the word at inventory + 4608 and whose
// items are 72 bytes each, starting at the inventory itself; the 64-bit word at item + 8 has a flag
// at bit 39 (0x0000008000000000). killPlayer is a signed byte. playerStatus is four words.
// 1. killPlayer nonzero (k = killPlayer - 1): for k up to nmPlayers - 2, in order,
//    convertPointersToIndicies__Fi(k + 1); player record k is overwritten by tempPlayerStruct record
//    k + 1; convertIndiciesToPointers__Fi(k). Then nmPlayers is decreased by one (a value of 1
//    afterwards also clears the byte netNetworkMode) and killPlayer becomes 0. Nothing else runs.
// 2. Otherwise nothing happens unless asyncMode is 7.
//    syncState equal to asyncMode (7): for each of the nmPlayers entries, the 12-byte point
//    playerChangeOldPos[i] is copied to playerStartPos[i] and the halfword playerChangeOldYaw[i] to
//    playerStartYaw[i]; nothing else. syncState other than 12 does nothing.
// 3. syncState 12 with MC_FrontEndImport nonzero: when nmPlayers is positive, for i below nmPlayers
//    (re-read each time): record i is overwritten by tempPlayerStruct record i, its pointer word at
//    21872 becomes 0, convertIndiciesToPointers__Fi(i) runs, "None" is copied to the name, and every
//    inventory item whose flag is set is taken out with removeItemFromInventory__Fiiii(i, index, 2, 0)
//    (the index is tried again after a removal, the count being re-read). Then gamePause and
//    particlesPaused become 0 and saveCheckPointCount 1. (Nothing else for nmPlayers not positive.)
// 4. syncState 12 with MC_FrontEndImport zero, with count the old nmPlayers: nmPlayers becomes 0.
//    first is the lowest pad index below 4 and below count with a nonzero playerStatus, or -1. For
//    each pad index s below 4 with a nonzero playerStatus: when s is below count and the status is 1
//    the start position and yaw take playerChangeOldPos[s] and playerChangeOldYaw[s]; otherwise a
//    point is taken from playerChangeOldPos[first] (read with first = -1 too, ahead of the array),
//    objectFindEmptySpot__FG6Point3iiPc(result, point, 20, 24, 0) runs, the start position takes the
//    result and the yaw is playerChangeOldYaw[first] + 8000 (halfword). They go to index nmPlayers of
//    playerStartPos and playerStartYaw. padChangeIndex__Fii(s, nmPlayers) runs and nmPlayers grows.
//    Then the flagged items of player 0's inventory are collected (72 bytes each, up to 100); the 4100
//    bytes at player 0 byte 17768, its name and its point at byte 68 are saved. For each pad index s
//    below 4 with a nonzero status, with d the number of nonzero statuses before s, and p the point
//    at byte 68 of record s read first: status 3: record d takes tempPlayerStruct record s, its
//    pointer word becomes 0, convertIndiciesToPointers__Fi(d), the point p is stored. Status 2:
//    playerInit__FiiPcScScScScT2(d, byte 32, name, bytes 33, 34, 36, 35, text at 37) with the record
//    of the 41-byte entry s of playerSelectInfo (name first), then p is stored. Status 1:
//    convertPointersToIndicies__Fi(s), record d takes tempPlayerStruct record s,
//    convertIndiciesToPointers__Fi(d), the pointer word becomes 0, p is stored. Any status: the saved
//    block and name are written back into record d. Then for each player below nmPlayers the flagged
//    items are removed as in step 3 and the collected items are added with
//    addItemToInventory__FiPQ24item11descriptioni(player, item, 1). debugClearStartMenu__Fv() and
//    Reset__6Cameraii(gameCam, 1, 0) run, then gamePause and particlesPaused become 0 and
//    saveCheckPointCount 1.
// Inferred: the structure copies through tempPlayerStruct are the player-pointer index conversions
// the two convert routines are named for; the flagged inventory items are shared between players.
// Unknown: the meaning of the item flag, of the 4100-byte block and of bytes 32 to 37 of the
// playerSelectInfo entries; the original also initializes the 100 collected-item slots with an
// inlined default (halfwords -1 at 36, 44, 52 and 60, bytes -1 at 32 and 33, halfwords 0 at 16..22
// and 24..30), which has no effect because used slots are overwritten whole.

extern "C" {
void* memcpy(void* to, const void* from, unsigned long size);
char* strcpy(char* to, const char* from);
void convertPointersToIndicies__Fi(int index);
void convertIndiciesToPointers__Fi(int index);
void removeItemFromInventory__Fiiii(int player, int index, int count, int mode);
int addItemToInventory__FiPQ24item11descriptioni(int player, void* item, int count);
void objectFindEmptySpot__FG6Point3iiPc(void* result, const void* point, int first, int second, char* third);
void padChangeIndex__Fii(int pad, int index);
void playerInit__FiiPcScScScScT2(int index, int second, char* name, signed char first_byte, signed char second_byte,
                                 signed char third_byte, signed char fourth_byte, char* tint);
void debugClearStartMenu__Fv();
void Reset__6Cameraii(void* camera, int a, int b);
extern int nmPlayers;
extern signed char killPlayer;
extern signed char netNetworkMode;
extern int asyncMode;
extern int syncState;
extern signed char MC_FrontEndImport;
extern int playerStatus[4];
extern unsigned char player[];
extern unsigned char tempPlayerStruct[];
extern unsigned char playerSelectInfo[];
extern float playerChangeOldPos[4][3];
extern unsigned short playerChangeOldYaw[4];
extern float playerStartPos[][3];
extern unsigned short playerStartYaw[];
extern void* gameCam;
extern int gamePause;
extern int saveCheckPointCount;
extern signed char particlesPaused;
}

namespace {
struct Point {
    float x;
    float y;
    float z;
};

struct Item {
    unsigned char bytes[72];
};

const int kPlayerSize = 23984;
const unsigned long long kItemFlag = 0x8000000000ull;

unsigned char* record(int index) { return player + index * kPlayerSize; }

unsigned char* inventory(int index) { return record(index) + 9560; }

bool flagged(const unsigned char* item) { return (*reinterpret_cast<const unsigned long long*>(item + 8) & kItemFlag) != 0; }

void remove_flagged(int index) {
    unsigned char* const items = inventory(index);
    int count = *reinterpret_cast<int*>(items + 4608);
    for (int i = 0; i < count; ++i) {
        if (flagged(items + i * 72)) {
            removeItemFromInventory__Fiiii(index, i, 2, 0);
            --i;
            count = *reinterpret_cast<int*>(items + 4608);
        }
    }
}

void finish() {
    gamePause = 0;
    saveCheckPointCount = 1;
    particlesPaused = 0;
}

void copy_record(int to, int from) { memcpy(record(to), tempPlayerStruct + from * kPlayerSize, kPlayerSize); }

void set_point(int index, const Point& value) { *reinterpret_cast<Point*>(record(index) + 68) = value; }
}

extern "C" void MC_CheckPlayerImportOrChange__Fv() {
    const int count = nmPlayers;
    if (killPlayer != 0) {
        int k = killPlayer - 1;
        if (k < count - 1) {
            do {
                const int next = k + 1;
                convertPointersToIndicies__Fi(next);
                memcpy(record(k), tempPlayerStruct + next * kPlayerSize, kPlayerSize);
                convertIndiciesToPointers__Fi(k);
                k = next;
            } while (k < nmPlayers - 1);
        }
        nmPlayers = nmPlayers - 1;
        if (nmPlayers == 1) {
            netNetworkMode = 0;
        }
        killPlayer = 0;
        return;
    }
    if (asyncMode != 7) {
        return;
    }
    if (syncState == asyncMode) {
        for (int i = 0; i < count; ++i) {
            *reinterpret_cast<Point*>(playerStartPos[i]) = *reinterpret_cast<Point*>(playerChangeOldPos[i]);
            playerStartYaw[i] = playerChangeOldYaw[i];
        }
        return;
    }
    if (syncState != 12) {
        return;
    }
    if (MC_FrontEndImport != 0) {
        if (count > 0) {
            for (int i = 0; i < nmPlayers; ++i) {
                copy_record(i, i);
                *reinterpret_cast<int*>(record(i) + 21872) = 0;
                convertIndiciesToPointers__Fi(i);
                strcpy(reinterpret_cast<char*>(record(i) + 36), "None");
                remove_flagged(i);
            }
        }
        finish();
        return;
    }
    nmPlayers = 0;
    int first = -1;
    for (int s = 0; s < 4; ++s) {
        if (playerStatus[s] != 0 && s < count) {
            first = s;
            break;
        }
    }
    float* const first_point = &playerChangeOldPos[0][0] + 3 * first;
    const unsigned short* const first_yaw = playerChangeOldYaw + first;
    for (int s = 0; s < 4; ++s) {
        const int status = playerStatus[s];
        if (status == 0) {
            continue;
        }
        if (s < count && status == 1) {
            *reinterpret_cast<Point*>(playerStartPos[nmPlayers]) = *reinterpret_cast<Point*>(playerChangeOldPos[s]);
            playerStartYaw[nmPlayers] = playerChangeOldYaw[s];
        } else {
            Point probe = *reinterpret_cast<Point*>(first_point);
            Point spot;
            objectFindEmptySpot__FG6Point3iiPc(&spot, &probe, 20, 24, 0);
            *reinterpret_cast<Point*>(playerStartPos[nmPlayers]) = spot;
            playerStartYaw[nmPlayers] = static_cast<unsigned short>(*first_yaw + 8000);
        }
        padChangeIndex__Fii(s, nmPlayers);
        nmPlayers = nmPlayers + 1;
    }
    Item collected[100];
    int collected_count = 0;
    {
        unsigned char* const items = inventory(0);
        int items_count = *reinterpret_cast<int*>(items + 4608);
        for (int i = 0; i < items_count; ++i) {
            if (flagged(items + i * 72)) {
                collected[collected_count] = *reinterpret_cast<Item*>(items + i * 72);
                ++collected_count;
                items_count = *reinterpret_cast<int*>(items + 4608);
            }
        }
    }
    struct Block {
        unsigned char bytes[4100];
    };
    Block block = *reinterpret_cast<Block*>(record(0) + 17768);
    char name[128];
    strcpy(name, reinterpret_cast<char*>(record(0) + 36));
    const Point saved_point = *reinterpret_cast<Point*>(record(0) + 68);
    for (int s = 0; s < 4; ++s) {
        const int status = playerStatus[s];
        if (status == 0) {
            continue;
        }
        int before = 0;
        for (int j = s - 1; j >= 0; --j) {
            if (playerStatus[j] == 0) {
                ++before;
            }
        }
        const int d = s - before;
        const Point p = *reinterpret_cast<Point*>(record(s) + 68);
        if (status == 3) {
            copy_record(d, s);
            *reinterpret_cast<int*>(record(d) + 21872) = 0;
            convertIndiciesToPointers__Fi(d);
            set_point(d, saved_point);
        } else if (status == 2) {
            unsigned char* const entry = playerSelectInfo + s * 41;
            playerInit__FiiPcScScScScT2(d, static_cast<signed char>(entry[32]), reinterpret_cast<char*>(entry),
                                        static_cast<signed char>(entry[33]), static_cast<signed char>(entry[34]),
                                        static_cast<signed char>(entry[36]), static_cast<signed char>(entry[35]),
                                        reinterpret_cast<char*>(entry + 37));
            set_point(d, saved_point);
        } else if (status == 1) {
            convertPointersToIndicies__Fi(s);
            copy_record(d, s);
            convertIndiciesToPointers__Fi(d);
            *reinterpret_cast<int*>(record(d) + 21872) = 0;
            set_point(d, p);
        }
        *reinterpret_cast<Block*>(record(d) + 17768) = block;
        strcpy(reinterpret_cast<char*>(record(d) + 36), name);
    }
    for (int p = 0; p < nmPlayers; ++p) {
        remove_flagged(p);
        for (int k = 0; k < collected_count; ++k) {
            addItemToInventory__FiPQ24item11descriptioni(p, &collected[k], 1);
        }
    }
    debugClearStartMenu__Fv();
    Reset__6Cameraii(gameCam, 1, 0);
    finish();
}
