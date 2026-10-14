// Observed act2 at 0x0042e0d0, 80 bytes of initialized data: 10 records of two words. Word one is a pointer
// to a NUL-terminated lowercase text name (read from the executable's read-only text); word two is a
// positive whole number. The records are listed below in file order. allActs (card-progress package) holds
// a pointer to this array, and MC_Init reads the same two words from each record.
// Inferred: the text names look like level names; the second word is shared by neighbouring levels
//   (for example a run of identical values in one act). No field name is claimed.
// Unknown: what the number counts, whether any record follows the last one, and which source file
//   originally owned the array.
struct LevelRecord {
    const char* name;
    int number;
};

extern "C" {
LevelRecord act2[10] = {
    {"gnomehub", 30000},
    {"anthill1", 35000},
    {"anthill2", 35000},
    {"lava1", 35000},
    {"lava2", 35000},
    {"battle1", 35000},
    {"battle3", 35000},
    {"gothic1", 35000},
    {"gothic2", 42000},
    {"nas", 35000},
};
}
