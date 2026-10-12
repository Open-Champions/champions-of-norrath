// Observed netRestoreButtonConfigs at 0x0012f710, 300 bytes. For each of the
// four players it copies nine words from the 36-byte record at
// MC_GameConfig + 52 + 36 * player into the live button tables, in this
// field order: attackButton, spellButton1, spellButton2, actionButton,
// blockButton, healthPotionButton, energyPotionButton, automapButton,
// weaponToggleButton. Each table is four words, one per player. This is the
// reverse of the comparison made by the maybe-save-config routine.
// Inferred: the config record is the saved copy of the button assignment.
// Unknown: why the network code restores it; MC_GameConfig is only touched
// at the offsets above here.
extern "C" {
extern unsigned char MC_GameConfig[];
extern unsigned attackButton[4];
extern unsigned spellButton1[4];
extern unsigned spellButton2[4];
extern unsigned actionButton[4];
extern unsigned blockButton[4];
extern unsigned healthPotionButton[4];
extern unsigned energyPotionButton[4];
extern unsigned automapButton[4];
extern unsigned weaponToggleButton[4];
}

extern "C" void netRestoreButtonConfigs__Fv() {
    for (int player = 0; player < 4; ++player) {
        const unsigned* saved = reinterpret_cast<const unsigned*>(MC_GameConfig + 52 + player * 36);
        attackButton[player] = saved[0];
        spellButton1[player] = saved[1];
        spellButton2[player] = saved[2];
        actionButton[player] = saved[3];
        blockButton[player] = saved[4];
        healthPotionButton[player] = saved[5];
        energyPotionButton[player] = saved[6];
        automapButton[player] = saved[7];
        weaponToggleButton[player] = saved[8];
    }
}
