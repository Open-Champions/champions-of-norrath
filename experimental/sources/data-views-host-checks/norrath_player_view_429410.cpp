// Host comparison only. Not an EE build input.
extern "C" unsigned char norrath_player_view_429410[48];
int main() {
    for (unsigned i = 0; i < 48u; ++i) if (norrath_player_view_429410[i] != 0) return 1;
    return 0;
}
