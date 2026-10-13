#include "poke_state.cpp"
#include <cstdio>

int main() {
    int bad = 0;
    bad |= sizeof(pokeMessageAlpha) != 16 || sizeof(pokeItemMagic) != 16;
    for (int i = 0; i < 4; ++i) bad |= pokeMessageAlpha[i] != 0 || pokeItemMagic[i] != 0;
    std::printf(bad ? "FAIL\n" : "ok: four zero slots each\n");
    return bad;
}
