extern "C" unsigned char norrath_player_progression_records_view[95936];
int main() {
    if (sizeof norrath_player_progression_records_view != 95936u) return 1;
    if (norrath_player_progression_records_view[0] != 0 || norrath_player_progression_records_view[47968] != 0 || norrath_player_progression_records_view[95935] != 0) return 2;
    unsigned nonzero = 0;
    for (unsigned index = 0; index < 95936u; ++index) nonzero += norrath_player_progression_records_view[index];
    return nonzero == 0 ? 0 : 3;
}
