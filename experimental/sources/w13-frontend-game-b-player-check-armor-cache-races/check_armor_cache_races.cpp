// Observed playerCheckArmorCacheRaces__Fv at 0x00195560, extent 132. Returns 1 if
// every one of the nmPlayers (0x00484538) slots agrees with the armour cache, else
// 0 at the first disagreement. Slot i is compared in order:
//   1. the signed byte at armorCache + 80 + 88*i + 5 must equal the word at
//      player + 84 + 23984*i, plus one (player is 0x00408600);
//   2. the signed byte at armorCache + 80 + 88*i + 6 must equal 1 when there are
//      two or more players or the byte coreForceSmallCharacters (0x00486678) is
//      nonzero, and 0 otherwise.
// With no players the result is 1. Reads only; nothing is written.
// Inferred: the +5 byte caches a character race index (the function name says
// races) and the +6 byte a size class; neither is established by a caller.
// Unknown: what the player word at +84 is called in the shipped source.
extern "C" {
extern int nmPlayers;
extern signed char coreForceSmallCharacters;
extern unsigned char player[];
extern unsigned char armorCache[] __asm__("norrath_player_armor_cache_view_7cb770");
int playerCheckArmorCacheRaces__Fv() {
    const int count = nmPlayers;
    for (int i = 0; i < count; ++i) {
        const signed char* entry = reinterpret_cast<const signed char*>(armorCache + 80 + 88 * i);
        const int race = *reinterpret_cast<const int*>(player + 84 + 23984 * i);
        if (entry[5] != race + 1) return 0;
        const int small = (count >= 2 || coreForceSmallCharacters != 0) ? 1 : 0;
        if (entry[6] != small) return 0;
    }
    return 1;
}
}
