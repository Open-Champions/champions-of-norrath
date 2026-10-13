// Observed generateSortedListAndInfoHeaders__Fv at 0x00256088, 592 bytes in SLUS_205.65.
// Takes no argument and returns nothing. Builds the list of memory-card save slots shown to
// the player, newest first, and loads their headers. The directory is MCA_DirContents
// (0x0042e400), MCA_LastDirCount (0x00485c0c) records of 64 bytes whose text name is at
// +32.
//  1. Every record whose name (record + 32) contains the text "slot" (strstr) is inserted
//     into a local index list ordered by moreRecent__Fii: the new index goes in front of
//     the first listed index for which moreRecent(new, listed) answers nonzero, else at the
//     end. The count of records is read again at each step.
//  2. The list length is capped at 10.
//  3. For each of those entries, in order: the 64-byte record is copied to sortedDir[k]
//     (0x007f7d40); then the memory-card command routine is called four ways:
//     MCA_Cmd__Fiiii(0, &sortedDir[k].name, 1, 0), MCA_Cmd__Fiiii(3, &slotInfoHeaders[k], 112, 0)
//     (the headers are 112 bytes each, from 0x008d0c08), loadPlayerInfo__FiPiP12playerStruct(2,
//     sortedPlayerRace[k], 0) (16 bytes each, from 0x007f7ca0), and MCA_Cmd__Fiiii(1, 0, 0, 0).
//  4. sortedDirCount (0x00485be8) is set to the capped length.
// Inferred: the 112-byte block is a slot's info header, the 16-byte block the player/race
// summary, and the MCA_Cmd codes open, read and close a file; those meanings are taken from the
// names and the argument shapes only. Unknown: the local list size in the image (this source
// uses room for the 64 directory records).
extern "C" char* strstr(const char* haystack, const char* needle);
extern "C" int moreRecent__Fii(int left, int right);
extern "C" int MCA_Cmd__Fiiii(int command, const void* argument, int size, int extra);
extern "C" void loadPlayerInfo__FiPiP12playerStruct(int command, int* out, void* player);

struct DirRecord { unsigned char bytes[64]; };
struct SlotHeader { unsigned char bytes[112]; };
struct PlayerRace { int words[4]; };
extern "C" DirRecord MCA_DirContents[];
extern "C" DirRecord sortedDir[];
extern "C" SlotHeader slotInfoHeaders[];
extern "C" PlayerRace sortedPlayerRace[];
extern "C" int MCA_LastDirCount;
extern "C" int sortedDirCount;

extern "C" void generateSortedListAndInfoHeaders__Fv() {
    int sorted[64];
    int count = 0;
    for (int i = 0; i < MCA_LastDirCount; ++i) {
        if (strstr(reinterpret_cast<const char*>(MCA_DirContents[i].bytes + 32), "slot") == 0) continue;
        int at = 0;
        while (at < count) {
            if (moreRecent__Fii(i, sorted[at]) != 0) break;
            ++at;
        }
        if (at < count) {
            for (int k = count; k > at; --k) sorted[k] = sorted[k - 1];
        }
        sorted[at] = i;
        ++count;
    }
    if (!(count < 11)) count = 10;
    for (int k = 0; k < count; ++k) {
        sortedDir[k] = MCA_DirContents[sorted[k]];
        MCA_Cmd__Fiiii(0, sortedDir[k].bytes + 32, 1, 0);
        MCA_Cmd__Fiiii(3, &slotInfoHeaders[k], 112, 0);
        loadPlayerInfo__FiPiP12playerStruct(2, sortedPlayerRace[k].words, 0);
        MCA_Cmd__Fiiii(1, 0, 0, 0);
    }
    sortedDirCount = count;
}
