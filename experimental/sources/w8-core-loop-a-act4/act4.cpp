// Observed act4 at 0x0042e1b0, 72 bytes of initialized data: 9 records of two words. Word one is a pointer
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
LevelRecord act4[9] = {
    {"elfhub", 45000},
    {"elfhubn", 45000},
    {"khathuun", 45000},
    {"khathuun_katone", 45000},
    {"khathuun_kattwo", 45000},
    {"khathuun_katnn", 45000},
    {"khathuun_katne", 45000},
    {"arena1", 45000},
    {"garden", 45000},
};
}
