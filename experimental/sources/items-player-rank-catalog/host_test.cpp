extern "C" unsigned char norrath_player_feat_rank_catalog_view[17960];
int main() {
    if (sizeof norrath_player_feat_rank_catalog_view != 17960u) return 1;
    if (norrath_player_feat_rank_catalog_view[0] != 0 || norrath_player_feat_rank_catalog_view[8980] != 0 || norrath_player_feat_rank_catalog_view[17959] != 0) return 2;
    unsigned nonzero = 0;
    for (unsigned index = 0; index < 17960u; ++index) nonzero += norrath_player_feat_rank_catalog_view[index];
    return nonzero == 0 ? 0 : 3;
}
