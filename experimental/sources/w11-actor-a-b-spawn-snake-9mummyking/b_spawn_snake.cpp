// Observed bSpawnSnake__9MummyKing at 0x0023bfa0, 108 bytes. Returns 0 unless the signed word at object+1132
// is below the signed word at +808. Then it scans the first nmPlayers (signed global word 0x00484538) words
// of the array starting at object+1136 and returns 1 at the first word equal to zero; 0 when none is zero
// (also 0 when nmPlayers is not positive). The array holds one slot per player; what a slot holds (probably
// a spawned object) is inferred, not observed. Nothing here ran on the EE.
extern "C" int nmPlayers;

extern "C" int bSpawnSnake__9MummyKing(void* self) {
    const unsigned char* o = static_cast<const unsigned char*>(self);
    if (!(*reinterpret_cast<const int*>(o + 1132) < *reinterpret_cast<const int*>(o + 808))) return 0;
    const unsigned* slot = reinterpret_cast<const unsigned*>(o + 1136);
    for (int i = 0; i < nmPlayers; ++i)
        if (slot[i] == 0u) return 1;
    return 0;
}
