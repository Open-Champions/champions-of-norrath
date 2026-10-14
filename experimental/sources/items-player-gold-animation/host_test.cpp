extern "C" unsigned char norrath_player_view_600d20[16];
int main() {
    if (sizeof norrath_player_view_600d20 != 16u) return 1;
    if (norrath_player_view_600d20[0] != 0 || norrath_player_view_600d20[8] != 0 || norrath_player_view_600d20[15] != 0) return 2;
    unsigned nonzero = 0;
    for (unsigned index = 0; index < 16u; ++index) nonzero += norrath_player_view_600d20[index];
    return nonzero == 0 ? 0 : 3;
}
