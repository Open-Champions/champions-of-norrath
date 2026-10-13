// Observed lumpAdd__FPvPCc at 0x1664b0, extent 152. Registers a loaded lump under a
// name. Arguments: the lump address and the name. In order:
//   1. A null lump address makes the routine store zero through that null address
//      (reproduced with a volatile store, as the lump initializer does).
//   2. result = initLump__FPv(lump).
//   3. entry = lumps[nmLumps]; nmLumps = nmLumps + 1 (the counter is read after the
//      initializer returns and stored back before the name copy).
//   4. strcpy(entry.name, name); then entry.word20 = 0, entry.lump = lump,
//      entry.word36 = 0, entry.word24 = 0, entry.word28 = 0, entry.word32 = 0.
//   5. Returns the initializer's result.
// There is no capacity check on nmLumps. Unknown: what the zeroed words are for.
#include "../w9-animation-object-b-lumps/lumps.hpp"

extern "C" {
int initLump__FPv(void* lump);
char* strcpy(char* destination, const char* source);
extern int nmLumps;
}

extern "C" int lumpAdd__FPvPCc(void* lump, const char* name) {
    if (lump == nullptr) *static_cast<volatile int*>(lump) = 0;
    const int result = initLump__FPv(lump);
    LumpEntry* entry = &lumps[nmLumps];
    nmLumps = nmLumps + 1;
    strcpy(entry->name, name);
    entry->word20 = 0;
    entry->lump = lump;
    entry->word36 = 0;
    entry->word24 = 0;
    entry->word28 = 0;
    entry->word32 = 0;
    return result;
}
