// Host check for playerClearSpellEffect. Not an EE plan input.
#include "player_clear_spell_effect.cpp"
extern "C" {
int nmPlayers;
int lightIntensity[4], spellAlpha[4], lightInc[4], spellAlphaInc[4];
}
int main() {
    for (int count = -1; count <= 4; ++count) {
        for (int i = 0; i < 4; ++i) lightIntensity[i] = spellAlpha[i] = lightInc[i] = spellAlphaInc[i] = 9 + i;
        nmPlayers = count;
        playerClearSpellEffect__Fv();
        for (int i = 0; i < 4; ++i) {
            int want = i < count ? 0 : 9 + i;
            if (lightIntensity[i] != want || spellAlpha[i] != want || lightInc[i] != want || spellAlphaInc[i] != want)
                return 1 + count;
        }
    }
    return 0;
}
