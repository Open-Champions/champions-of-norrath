// Observed playerClearSpellEffect at 0x001ab878, 104 bytes. For each index
// below nmPlayers (signed) it stores zero into word [index] of four
// four-word arrays at 0x004085c0 (lightIntensity), 0x00408560 (spellAlpha),
// 0x004085b0 (lightInc) and 0x004085d0 (spellAlphaInc), in that order.
// Nothing runs when nmPlayers is not positive.
// Inferred: the arrays hold per-player spell flash state; the names are the
// image's own local symbols, whose element type is not established.
// Unknown: which routines set the values, and the element type (words here).
extern "C" {
extern int nmPlayers;
extern int lightIntensity[4];
extern int spellAlpha[4];
extern int lightInc[4];
extern int spellAlphaInc[4];
}

extern "C" void playerClearSpellEffect__Fv() {
    int count = nmPlayers;
    for (int i = 0; i < count; ++i) {
        lightIntensity[i] = 0;
        spellAlpha[i] = 0;
        lightInc[i] = 0;
        spellAlphaInc[i] = 0;
    }
}
