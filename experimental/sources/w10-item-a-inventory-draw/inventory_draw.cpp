// Observed inventoryDraw__Fi at 0x001ac8a8, 236 bytes (int screen). `inventory`
// (0x00427288) is an array of 116-byte screen records; the word at +52 of a record
// is a player index and the word at +76 is a mode value of which only 2 is tested.
// With S = &inventory[screen] and P = player + S.player * 23984:
//   1. mode != 2: displayGold__FP12playerStructP12screenStruct(P, S), then
//      displayWeight__FP12playerStructP12screenStruct(P, S); the mode is read
//      again afterwards and anything but 2 ends the function.
//   2. otherwise (mode 2, before or after those calls): a tail call to
//      doHelpfulMessage__FiiP12screenStructiiPQ24item11description(screen,
//      S.player, S, -2, 1, 0). Its sixth argument is a null item pointer.
// Unknown: what the helpful message shows and what the mode values mean.
extern "C" {
extern unsigned char player[];
extern unsigned char inventory[] __attribute__((aligned(4)));
void displayGold__FP12playerStructP12screenStruct(unsigned char* record, unsigned char* screen);
void displayWeight__FP12playerStructP12screenStruct(unsigned char* record, unsigned char* screen);
void doHelpfulMessage__FiiP12screenStructiiPQ24item11description(int, int, unsigned char*, int, int, void*);

void inventoryDraw__Fi(int screen) {
    unsigned char* s = inventory + screen * 116;
    unsigned char* record = player + *reinterpret_cast<int*>(s + 52) * 23984;
    if (*reinterpret_cast<int*>(s + 76) != 2) {
        displayGold__FP12playerStructP12screenStruct(record, s);
        displayWeight__FP12playerStructP12screenStruct(record, s);
        if (*reinterpret_cast<int*>(s + 76) != 2) return;
    }
    doHelpfulMessage__FiiP12screenStructiiPQ24item11description(screen, *reinterpret_cast<int*>(s + 52), s, -2, 1, 0);
}
}
