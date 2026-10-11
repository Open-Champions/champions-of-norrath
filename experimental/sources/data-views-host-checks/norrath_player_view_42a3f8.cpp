// Host comparison only. Not an EE build input.
extern "C" char norrath_player_view_42a3f8[32];
int main() {
    for (unsigned i = 0; i < 32u; ++i) if (norrath_player_view_42a3f8[i] != 0) return 1;
    return 0;
}
