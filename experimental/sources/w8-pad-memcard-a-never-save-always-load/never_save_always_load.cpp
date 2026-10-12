// Observed: neverSaveAlwaysLoadFromLump__FPc at 0x00155b30 is 112 bytes. A null
// argument returns 0. Otherwise it walks the pointer table neverSave until a
// null entry, comparing the argument with each entry through strcasecmp. The
// first entry that compares equal returns 1; reaching the null entry returns 0.
// Inferred: nothing.
// Unknown: the policy the name describes; the table's owner.

extern "C" int strcasecmp(const char* a, const char* b);
extern "C" char* neverSave[];

extern "C" int neverSaveAlwaysLoadFromLump__FPc(char* name) {
    if (name != 0) {
        char** entry = neverSave;
        while (*entry != 0) {
            if (strcasecmp(name, *entry++) == 0) {
                return 1;
            }
        }
    }
    return 0;
}
