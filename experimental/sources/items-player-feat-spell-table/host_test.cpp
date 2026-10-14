#include <cstring>
struct FeatSpellRecord {
    char name[32];
    unsigned char tail[72];
};
extern "C" const FeatSpellRecord norrath_player_feat_spell_table_view[];
int main() {
    const FeatSpellRecord* rows = norrath_player_feat_spell_table_view;
    if (std::strcmp(rows[0].name, "Accuracy") != 0) return 1;
    if (rows[0].tail[0] != 0x1d || rows[0].tail[1] != 0x00) return 2;
    unsigned identity = rows[0].tail[40] | (rows[0].tail[41] << 8) | (rows[0].tail[42] << 16) | (rows[0].tail[43] << 24);
    if (identity != 4u) return 3;
    if (std::strcmp(rows[21].name, "END") != 0) return 4;
    for (unsigned index = 0; index < 72; ++index)
        if (rows[21].tail[index] != 0) return 5;
    return 0;
}
