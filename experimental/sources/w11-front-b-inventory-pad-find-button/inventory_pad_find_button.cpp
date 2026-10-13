// Observed inventoryPadFindButton__Fi at 0x001b1e50, 180 bytes. The argument is
// a button mask. Players are scanned from 0 while the index is below the word
// nmPlayers (0x00484538), which is re-read after each netPlayerIsLocal__Fi
// call that is reached. A player qualifies when the float at player record
// offset 172 is greater than 0.0 (record stride 23984 from `player`, 0x00408600)
// and netPlayerIsLocal__Fi(index) is nonzero. A qualifying player is the answer
// when the word at offset 12 of its 88-byte netLocalPad record (0x00332fd0) has
// any bit of the mask set. The first answer is returned; with none, -1. An empty
// player set (nmPlayers not above zero) returns -1 without calls.
// Unknown: what the float at record offset 172 holds (it behaves like a
// liveness or health test), and the layout of the pad record beyond the word.
extern "C" {
extern int nmPlayers;
extern unsigned char player[];
extern unsigned char netLocalPad[];
int netPlayerIsLocal__Fi(int index);
}

extern "C" int inventoryPadFindButton__Fi(int mask) {
    for (int i = 0; i < nmPlayers; ++i) {
        if (*reinterpret_cast<const float*>(player + 23984 * i + 172) > 0.0f) {
            if (netPlayerIsLocal__Fi(i) != 0) {
                if ((*reinterpret_cast<const int*>(netLocalPad + 88 * i + 12) & mask) != 0) return i;
            }
        }
    }
    return -1;
}
