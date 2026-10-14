// Host comparison only. Not an EE build input.
extern "C" unsigned short norrath_player_view_484f38[4];
int main() {
    for (unsigned i = 0; i < 4u; ++i) if (norrath_player_view_484f38[i] != 0) return 1;
    return 0;
}
