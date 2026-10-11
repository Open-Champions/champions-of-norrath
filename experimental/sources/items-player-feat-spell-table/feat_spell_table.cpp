// Observed spellTable.1065 at 0x4278b8: 22 records of 104 bytes.
// Bytes 0..31 are the name. The remaining 72 bytes are the observed tail,
// including the fields give-feat copies from offsets 32, 34, 36, 44, 48, 64, 72, 80 and 96.
struct FeatSpellRecord {
    char name[32];
    unsigned char tail[72];
};
static_assert(sizeof(FeatSpellRecord) == 104, "feat spell record");
extern "C" const FeatSpellRecord norrath_player_feat_spell_table_view[] = {
#include "records.hpp"
};
