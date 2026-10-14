// Observed bSpawnMummy__9MummyKing at 0x0023c010, 72 bytes. Scans the first nmPlayers (signed global word
// 0x00484538) words of the array starting at object+1108 and returns 1 at the first word equal to zero.
// When none is zero (or nmPlayers is not positive) it returns 1 when the word at +1124 is zero, else 0.
// The array holds one slot per player; what a slot holds is inferred (not observed). Note that +1124 lies
// four words past the first slot, inside the range a four-player scan covers; the image reads it only after
// the scan. Nothing here ran on the EE.
extern "C" int nmPlayers;

extern "C" int bSpawnMummy__9MummyKing(void* self) {
    const unsigned char* o = static_cast<const unsigned char*>(self);
    const unsigned* slot = reinterpret_cast<const unsigned*>(o + 1108);
    for (int i = 0; i < nmPlayers; ++i)
        if (slot[i] == 0u) return 1;
    return *reinterpret_cast<const unsigned*>(o + 1124) == 0u ? 1 : 0;
}
