// Observed act1 at 0x0042e020, 176 bytes of initialized data: 22 records of two words. Word one is a pointer
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
LevelRecord act1[22] = {
    {"kelethin", 30000},
    {"faydark1", 34000},
    {"faydark2", 34000},
    {"faydark3", 34000},
    {"goblin1a_0", 12000},
    {"goblin1a_2", 12000},
    {"goblin1a_4", 12000},
    {"goblin1a_6", 12000},
    {"goblin1_f0", 12000},
    {"goblin1_f1", 12000},
    {"goblin1_f2", 12000},
    {"goblin2_0", 11000},
    {"goblin2_1", 11000},
    {"goblin2_2", 11000},
    {"goblin2_3", 11000},
    {"goblin2_4", 12000},
    {"goblin2_6", 12000},
    {"orccave1", 34000},
    {"orccave2", 34000},
    {"orccave3", 34000},
    {"ruinint1", 24000},
    {"ruinint2", 24000},
};
}
