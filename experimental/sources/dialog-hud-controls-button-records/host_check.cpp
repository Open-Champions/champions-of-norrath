#include <cstring>

extern "C" unsigned char attackButton[16];
extern "C" unsigned char spellButton1[16];
extern "C" unsigned char spellButton2[16];
extern "C" unsigned char actionButton[16];
extern "C" unsigned char blockButton[16];
extern "C" unsigned char automapButton[16];
extern "C" unsigned char healthPotionButton[16];
extern "C" unsigned char energyPotionButton[16];
extern "C" unsigned char weaponToggleButton[16];

static int zero16(unsigned char* object) {
    unsigned char want[16] = {};
    if (sizeof want != 16) return 1;
    return std::memcmp(object, want, 16) == 0 ? 0 : 1;
}

int main() {
    if (zero16(attackButton) || zero16(spellButton1) || zero16(spellButton2)) return 1;
    if (zero16(actionButton) || zero16(blockButton) || zero16(automapButton)) return 2;
    if (zero16(healthPotionButton) || zero16(energyPotionButton) || zero16(weaponToggleButton)) return 3;
    attackButton[0] = 1;
    if (attackButton[0] != 1 || spellButton1[0] != 0) return 4;
    attackButton[0] = 0;
    return 0;
}
