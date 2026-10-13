// Observed playerSelectionEnforce__Fv at 0x00279e38, 268 bytes (pinned
// SLUS_205.65). When the word needEnforce is zero it does nothing. Otherwise
// it re-initializes every player slot from the select screen's records and
// clears needEnforce. For slot i in 0..nmPlayers-1 (nmPlayers re-read each
// pass), with rec = playerSelectInfo + 41*i and tgt = targets + 636*rec[32]
// (both byte indexes are signed), it calls
//   playerInit(i, tgt[16], rec, b0, rec[34], rec[36], rec[35], rec + 37)
// where b0 is rec[33] when tgt[25] != tgt[18], and -1 when they are equal.
// The call's argument order follows the playerInit declaration of the
// session-roster unit.
// Inferred: tgt[16] is the value stored at +84 of the player record; rec + 37
// is the optional four-byte "tint" source.
// Unknown: what tgt[18] and tgt[25] are (they are compared, not interpreted),
// and the meaning of rec[33..36].
extern "C" {
extern int needEnforce;
extern int nmPlayers;
extern signed char playerSelectInfo[];
extern signed char targets[];

void playerInit__FiiPcScScScScT2(int index, int second, char* name, signed char first_byte,
                                 signed char second_byte, signed char third_byte,
                                 signed char fourth_byte, char* tint);

void playerSelectionEnforce__Fv() {
    if (needEnforce == 0) return;
    for (int i = 0; i < nmPlayers; ++i) {
        signed char* rec = playerSelectInfo + i * 41;
        const signed char* tgt = targets + rec[32] * 636;
        signed char first = rec[33];
        if ((tgt[25] ^ tgt[18]) == 0) first = -1;
        playerInit__FiiPcScScScScT2(i, tgt[16], reinterpret_cast<char*>(rec), first, rec[34], rec[36],
                                    rec[35], reinterpret_cast<char*>(rec + 37));
    }
    needEnforce = 0;
}
}
