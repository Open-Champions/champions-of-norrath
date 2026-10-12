// Host check for netRestoreButtonConfigs. Not an EE plan input.
#include "net_restore_button_configs.cpp"
extern "C" {
alignas(4) unsigned char MC_GameConfig[252];
unsigned attackButton[4], spellButton1[4], spellButton2[4], actionButton[4], blockButton[4];
unsigned healthPotionButton[4], energyPotionButton[4], automapButton[4], weaponToggleButton[4];
}
int main() {
    for (int i = 0; i < 252; ++i) MC_GameConfig[i] = (unsigned char)(i * 7 + 1);
    unsigned* all[9] = {attackButton, spellButton1, spellButton2, actionButton, blockButton,
                        healthPotionButton, energyPotionButton, automapButton, weaponToggleButton};
    for (auto* t : all) for (int p = 0; p < 4; ++p) t[p] = 0xffffffffu;
    netRestoreButtonConfigs__Fv();
    for (int field = 0; field < 9; ++field)
        for (int p = 0; p < 4; ++p) {
            const unsigned char* b = MC_GameConfig + 52 + 36 * p + 4 * field;
            unsigned want = b[0] | (b[1] << 8) | (b[2] << 16) | ((unsigned)b[3] << 24);
            if (all[field][p] != want) return 1 + field;
        }
    return 0;
}
