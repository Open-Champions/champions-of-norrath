// Observed resetName__Fv at 0x00280978, 68 bytes, 16-byte frame. It stores 0 into `namePos` and clears the first
// 32 bytes of the current player's 41-byte playerSelectInfo record with memset. It returns nothing.
// Inferred: the first 32 bytes of the record are the typed name (the stats and look bytes at +32.. are kept).
// Unknown: the character encoding of the name and whether anything else observes namePos.
extern "C" {
extern int curPlayer;
extern int namePos;
extern unsigned char playerSelectInfo[];
void* memset(void* destination, int value, unsigned size);

void resetName__Fv() {
    namePos = 0;
    memset(playerSelectInfo + 41 * curPlayer, 0, 32);
}
}
