// Observed inventoryRun__Fi at 0x001ac998, 644 bytes (int screen). `inventory`
// (0x00427288) holds 116-byte screen records: +48 and +88 are flag words, +52 is a
// player index (-1 once closed) and +76 a mode value of which 0 and 2 are
// distinguished. S = &inventory[screen]; P(n) = player + n * 23984.
//   1. Shutdown: when the mode words of both records inventory[0] and inventory[1]
//      are 2, or localNmPlayersAlive is 1 and S's mode is 2, the function tail
//      calls invShutDown__Fi(screen).
//   2. Mode 2 (anything else tested after step 1): b = inventoryPadFindButton__Fi(
//      padAcceptButton). Nothing happens when b equals the player index of
//      inventory[0], inventory[1] or S, or b is negative. Otherwise it tail calls
//      msgInventoryExit__Fiii(screen, b, f) where f is 1 when the signed byte at
//      P(b)+88 is less than the signed byte at P(b)+89, else 0.
//   3. Mode 0: displayItems__FiiP12screenStructi(screen, S.player, S, d) with d = 3
//      when the word S+88 is 1, else 2. Then, with c = netLocalPad word at
//      88 * S.player + 12 (read after the call): proceed only if c has a bit of
//      padBackButton or bit 0x100, and the byte multiHelpMessageOn[S.player] is 0.
//      Then S+88 nonzero -> cleared, done; else S+48 nonzero -> cleared, done; else
//      scriptStart__FPc("*exitInventoryNotify") for every player index below
//      nmPlayers whose word at P(i)+4936 is 2 or more, then S.player = -1 and
//      S.mode = 2.
//   Any other mode value does nothing.
// Unknown: what the flag words at +48 and +88 mean and what the bytes at P+88/89
// are.
extern "C" {
extern unsigned char player[];
extern unsigned char inventory[] __attribute__((aligned(4)));
extern unsigned char netLocalPad[];
extern signed char multiHelpMessageOn[];
extern unsigned padBackButton;
extern int padAcceptButton;
extern int localNmPlayersAlive;
extern int nmPlayers;
void invShutDown__Fi(int);
int inventoryPadFindButton__Fi(int);
void msgInventoryExit__Fiii(int, int, int);
void displayItems__FiiP12screenStructi(int, int, unsigned char*, int);
void scriptStart__FPc(char*);

void inventoryRun__Fi(int screen) {
    unsigned char* s = inventory + screen * 116;
    int* first = reinterpret_cast<int*>(inventory);
    int* second = reinterpret_cast<int*>(inventory + 116);
    if ((first[19] == 2 && second[19] == 2) || (localNmPlayersAlive == 1 && *reinterpret_cast<int*>(s + 76) == 2)) {
        invShutDown__Fi(screen);
        return;
    }
    int mode = *reinterpret_cast<int*>(s + 76);
    if (mode == 2) {
        int b = inventoryPadFindButton__Fi(padAcceptButton);
        if (first[13] == b || second[13] == b || b < 0 || *reinterpret_cast<int*>(s + 52) == b) return;
        unsigned char* record = player + b * 23984;
        msgInventoryExit__Fiii(screen, b, static_cast<signed char>(record[88]) < static_cast<signed char>(record[89]));
    } else if (mode == 0) {
        displayItems__FiiP12screenStructi(screen, *reinterpret_cast<int*>(s + 52), s,
                                          *reinterpret_cast<int*>(s + 88) == 1 ? 3 : 2);
        int p = *reinterpret_cast<int*>(s + 52);
        unsigned c = *reinterpret_cast<unsigned*>(netLocalPad + 88 * p + 12);
        if ((c & padBackButton) == 0 && (c & 0x100) == 0) return;
        if (multiHelpMessageOn[p] != 0) return;
        if (*reinterpret_cast<int*>(s + 88) != 0) {
            *reinterpret_cast<int*>(s + 88) = 0;
        } else if (*reinterpret_cast<int*>(s + 48) != 0) {
            *reinterpret_cast<int*>(s + 48) = 0;
        } else {
            for (int i = 0; i < nmPlayers; i++) {
                if (*reinterpret_cast<int*>(player + i * 23984 + 4936) >= 2) scriptStart__FPc(const_cast<char*>("*exitInventoryNotify"));
            }
            *reinterpret_cast<int*>(s + 52) = -1;
            *reinterpret_cast<int*>(s + 76) = 2;
        }
    }
}
}
